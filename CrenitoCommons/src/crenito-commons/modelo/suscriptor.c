/*
 * suscriptor.c
 *
 *  Created on: 10 jul. 2020
 *      Author: utnso
 */

#include "suscriptor.h"

t_suscriptor* suscriptor_crear(int socket, uint32_t id_suscriptor) {

	t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));

	suscriptor->id_proceso = id_suscriptor;
	suscriptor->socket = socket;

	return suscriptor;
}

t_suscriptor* suscriptor_duplicar(t_suscriptor* suscriptor) {
	return suscriptor_crear(suscriptor -> socket, suscriptor ->id_proceso);
}

void suscriptor_destruir(t_suscriptor* suscriptor) {
	free(suscriptor);
}

uint32_t suscriptor_get_id_proceso(t_suscriptor* suscriptor) {
	return suscriptor ->id_proceso;
}

int suscriptor_get_socket(t_suscriptor* suscriptor) {
	return suscriptor -> socket;
}

void suscriptor_set_socket(t_suscriptor* suscriptor, int socket){
	suscriptor -> socket = socket;
}
