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

void procesar_suscripcion(int socket, t_paquete_header header) {

	t_cola_container* container = get_cola(header.id_cola);
	t_suscriptor* suscriptor = suscribir_proceso(socket, header.id_proceso, container);

	uint32_t id_suscriptor = suscriptor->id_proceso;
	log_nuevo_suscriptor(header);

	socket_send(socket, &id_suscriptor, sizeof(id_suscriptor));

	enviar_mensajes_cacheados(container, suscriptor);
}

static t_suscriptor* suscribir_proceso(int socket, int id_proceso, t_cola_container* container) {

	pthread_mutex_lock(&container->mutex_suscriptores);
	t_suscriptor* suscriptor = suscriptor_crear(socket, id_proceso);
	list_add(container->suscriptores, suscriptor);
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
		mensaje_cache_agregar_suscriptor(msj, suscriptor, ENVIADO);
		crear_hilo_y_enviar_mensaje_a_suscriptor(msj, suscriptor_duplicar(suscriptor));
	}

	list_iterate(a_enviar, (void*) enviar);
	list_destroy(a_enviar);
}
