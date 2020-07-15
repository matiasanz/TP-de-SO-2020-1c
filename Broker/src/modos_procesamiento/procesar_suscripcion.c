/*
 * procesar_subscripcion.c
 *
 *  Created on: 14 jun. 2020
 *      Author: utnso
 */

#include "procesar_suscripcion.h"

//Funciones privadas
static t_suscriptor* suscribir_proceso(int socket, int id_proceso, t_cola_container* container);
static void enviar_mensajes_cacheados(t_cola_container* container, t_suscriptor* suscriptor);

void procesar_suscripcion(int* socket, t_paquete_header header) {

	t_cola_container* container = get_cola(header.id_cola);
	t_suscriptor* suscriptor = suscribir_proceso(*socket, proceso_get_id(header.proceso), container);

	uint32_t id_suscriptor = suscriptor->id_proceso;
	log_nuevo_suscriptor(header);

	socket_send(*socket, &id_suscriptor, sizeof(id_suscriptor));

	pthread_mutex_lock(&mutex_acceso_memoria);
	enviar_mensajes_cacheados(container, suscriptor);
	pthread_mutex_unlock(&mutex_acceso_memoria);

	free(socket);
}

static t_suscriptor* suscribir_proceso(int socket, int id_proceso, t_cola_container* container) {

	pthread_mutex_lock(&container->mutex_suscriptores);

	t_suscriptor* suscriptor = cola_buscar_suscriptor(id_proceso, socket, container);

	if (!suscriptor_existe(suscriptor)) {
		suscriptor = suscriptor_crear(socket, id_proceso);
		list_add(container->suscriptores, suscriptor);
	} else {
		suscriptor_set_socket(suscriptor, socket);
	}

	pthread_mutex_unlock(&container->mutex_suscriptores);

	return suscriptor;
}

static void enviar_mensajes_cacheados(t_cola_container* container, t_suscriptor* suscriptor) {

	//TODO: filtrar mensajes que no hayan sido enviados al suscriptor
	bool mensajes_en_uso(t_mensaje_cache* msj) {
		return particion_esta_ocupada(msj->particion);
	}

	pthread_mutex_lock(&container->mutex_mensajes);
	t_list* a_enviar = list_filter(container->cola, (void*) mensajes_en_uso);
	pthread_mutex_unlock(&container->mutex_mensajes);

	void enviar(t_mensaje_cache* msj) {
		particion_actualizar_fecha_ultimo_acceso(msj->particion);
		//TODO: ver si se puede usar un hilo
		restaurar_y_enviar_mensaje_a_suscriptor(msj, suscriptor, cola_get_id(container));
	}

	list_iterate(a_enviar, (void*) enviar);
	list_destroy(a_enviar);
}
