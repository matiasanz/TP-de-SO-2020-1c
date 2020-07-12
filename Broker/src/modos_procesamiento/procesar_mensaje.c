/*
 * procesar_mensaje.c
 *
 *  Created on: 14 jun. 2020
 *      Author: utnso
 */
#include "procesar_mensaje.h"

//Funciones privadas
static void replicar_mensaje(t_cola_container* container, t_mensaje_cache* msj);

void procesar_mensaje(int socket, t_paquete_header header) {

	int size = 0;
	void* msj_recibido = socket_recibir_mensaje(socket, &size);
	log_mensaje_recibido(header.id_cola, msj_recibido);

	t_mensaje_cache* msj_cache = guardar_en_memoria(msj_recibido, header.id_cola);

	crear_hilo_y_enviar_id_univoco(mensaje_cache_get_id(msj_cache), socket, header);

	t_cola_container* container = get_cola(header.id_cola);

	encolar_mensaje(container, msj_cache);

	replicar_mensaje(container, msj_cache);
}

static void replicar_mensaje(t_cola_container* container, t_mensaje_cache* mensaje_cache) {

	pthread_mutex_lock(&container->mutex_suscriptores);
	t_list* suscriptores = list_duplicate(container->suscriptores);
	pthread_mutex_unlock(&container->mutex_suscriptores);

	void replicar(t_suscriptor* suscriptor) {
		crear_hilo_y_enviar_mensaje_a_suscriptor(mensaje_cache, suscriptor_duplicar(suscriptor));
	}

	list_iterate(suscriptores, (void*) replicar);
	list_destroy(suscriptores);
}
