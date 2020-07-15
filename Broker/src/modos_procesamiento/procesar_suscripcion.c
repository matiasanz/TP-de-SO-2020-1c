/*
 * procesar_subscripcion.c
 *
 *  Created on: 14 jun. 2020
 *      Author: utnso
 */

#include "procesar_suscripcion.h"

//Funciones privadas
static t_suscriptor* suscribir_proceso(int socket, t_proceso proceso, t_cola_container* container);
static void enviar_mensajes_cacheados(t_cola_container* container, t_suscriptor* suscriptor);

void procesar_suscripcion(int* socket, t_paquete_header header) {

	t_cola_container* container = get_cola(header.id_cola);
	t_suscriptor* suscriptor = suscribir_proceso(*socket, paquete_header_get_proceso(header), container);

	uint32_t id_suscriptor = suscriptor->id_proceso;

	socket_send(*socket, &id_suscriptor, sizeof(id_suscriptor));

	pthread_mutex_lock(&mutex_acceso_memoria);
	enviar_mensajes_cacheados(container, suscriptor);
	pthread_mutex_unlock(&mutex_acceso_memoria);

	free(socket);
}

static t_suscriptor* suscribir_proceso(int socket, t_proceso proceso, t_cola_container* container) {

	pthread_mutex_lock(&container->mutex_suscriptores);

	t_suscriptor* suscriptor = cola_buscar_suscriptor(proceso_get_id(proceso), socket, container);

	if (!suscriptor_existe(suscriptor)) {
		suscriptor = suscriptor_crear(socket, proceso_get_id(proceso));
		list_add(container->suscriptores, suscriptor);
		log_nuevo_suscriptor(proceso, cola_get_id(container));
	} else {
		suscriptor_set_socket(suscriptor, socket);
	}

	pthread_mutex_unlock(&container->mutex_suscriptores);

	return suscriptor;
}

static void enviar_mensajes_cacheados(t_cola_container* container, t_suscriptor* suscriptor) {

	bool mensajes_pendientes_confirmacion(t_mensaje_cache* msj) {
		return particion_esta_ocupada(msj->particion) && mensaje_cache_pendiente_confirmacion(msj, suscriptor);
	}

	pthread_mutex_lock(&container->mutex_mensajes);
	t_list* a_enviar = list_filter(container->cola, (void*) mensajes_pendientes_confirmacion);
	pthread_mutex_unlock(&container->mutex_mensajes);

	void enviar(t_mensaje_cache* msj) {
		//TODO: ver si se puede usar un hilo
		restaurar_y_enviar_mensaje_a_suscriptor(msj, suscriptor, cola_get_id(container));
	}

	list_iterate(a_enviar, (void*) enviar);
	list_destroy(a_enviar);
}
