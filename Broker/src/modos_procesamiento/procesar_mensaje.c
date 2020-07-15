/*
 * procesar_mensaje.c
 *
 *  Created on: 14 jun. 2020
 *      Author: utnso
 */
#include "procesar_mensaje.h"

//Funciones privadas
static void replicar_mensaje(t_cola_container* container, t_mensaje_cache* mensaje_cache, t_mensaje_header* msj_header, void* msj_deserializado);
static void* deserializar_y_asignar_ids(void* msj_recibido, t_mensaje_header* header, t_id_cola id_cola);
static t_mensaje_header obtener_mensaje_header_y_asignar_id(void* mensaje, t_id_cola id_cola);
static bool debe_asignar_id_correlativo_por_primera_vez(uint32_t id_correlativo_actual, t_id_cola id_cola);

void procesar_mensaje(int* socket, t_paquete_header pqt_header) {

	int size;
	void* msj_recibido = socket_recibir_mensaje(*socket, &size);

	t_mensaje_header* msj_header = malloc(sizeof(t_mensaje_header));
	void* msj_deserializado = deserializar_y_asignar_ids(msj_recibido, msj_header, pqt_header.id_cola);

	t_mensaje_cache* msj_cache = guardar_en_memoria(msj_recibido, pqt_header.id_cola, *msj_header);
	mensaje_cache_set_id_correlativo(msj_cache, msj_header -> id_correlativo);
	
	//Debug
	//enviar_id_mensaje(enviar_id_mensaje_args_crear(msj_header -> id, socket, pqt_header));
	crear_hilo_y_enviar_id_univoco(msj_header -> id, socket, pqt_header);

	t_cola_container* container = get_cola(pqt_header.id_cola);

	encolar_mensaje(container, msj_cache);

	replicar_mensaje(container, msj_cache, msj_header, msj_deserializado);
}

static void replicar_mensaje(t_cola_container* container, t_mensaje_cache* mensaje_cache, t_mensaje_header* msj_header, void* msj_deserializado) {

	pthread_mutex_lock(&container->mutex_suscriptores);
	t_list* suscriptores = list_duplicate(container->suscriptores);
	pthread_mutex_unlock(&container->mutex_suscriptores);

	void replicar(t_suscriptor* suscriptor) {
		//TODO Verificar si al reenviar deberia actualizar LRU
		crear_hilo_y_enviar_mensaje_a_suscriptor(mensaje_cache, suscriptor, msj_header, msj_deserializado, cola_get_id(container));
		//Debug
		//enviar_mensaje_a_suscriptor(mensaje_cache, suscriptor_duplicar(suscriptor), msj_header, msj_deserializado, cola_get_, id(container));
		//mensaje_destruir(msj_deserializado, cola_get_id(container));

	}

	list_iterate(suscriptores, (void*) replicar);

	list_destroy(suscriptores);
}

static void* deserializar_y_asignar_ids(void* msj_recibido, t_mensaje_header* header, t_id_cola id_cola) {

	void* msj_deserializado = deserializar(msj_recibido, id_cola);
	*header = obtener_mensaje_header_y_asignar_id(msj_deserializado, id_cola);
	mensaje_set_header(msj_deserializado, *header, id_cola);
	log_mensaje_recibido(msj_deserializado, id_cola);

	return msj_deserializado;
}

static t_mensaje_header obtener_mensaje_header_y_asignar_id(void* mensaje, t_id_cola id_cola) {

	t_mensaje_header header = mensaje_get_header(mensaje, id_cola);

	header.id = generar_id_univoco();

	if (debe_asignar_id_correlativo_por_primera_vez(header.id_correlativo, id_cola)) {
		header.id_correlativo = header.id;
	}

	return header;
}

static bool debe_asignar_id_correlativo_por_primera_vez(uint32_t id_correlativo_actual, t_id_cola id_cola) {
	return !id_correlativo_actual && cola_inicia_flujo(id_cola);
}
