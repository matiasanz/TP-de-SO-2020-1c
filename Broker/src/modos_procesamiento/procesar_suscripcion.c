/*
 * procesar_subscripcion.c
 *
 *  Created on: 14 jun. 2020
 *      Author: utnso
 */

#include "procesar_suscripcion.h"

//Funciones privadas
static int suscribir_proceso(int socket, t_id_cola id_cola);

void procesar_suscripcion(int socket, t_paquete_header header) {

	int id_subscriptor = suscribir_proceso(socket, header.id_cola);

	log_info(logger, "Un proceso %s se suscribió a la cola %s. El id_subscriptor generado es: %d \n",
			get_nombre_proceso(header.id_proceso), get_nombre_cola(header.id_cola), id_subscriptor);

	socket_send(socket, &id_subscriptor, sizeof(id_subscriptor));
}

static int suscribir_proceso(int socket, t_id_cola id_cola) {

	t_cola_container* container = get_cola(id_cola);

	pthread_mutex_lock(&container->mutex);
	t_suscriptor* suscriptor = suscriptor_crear(socket, list_size(container->suscriptores) + 1);
	list_add(container->suscriptores, suscriptor);
	pthread_mutex_unlock(&container->mutex);

	return suscriptor->id_subcriptor;
}
