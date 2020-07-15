/*
 * suscriptor.h
 *
 *  Created on: 10 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MODELO_SUSCRIPTOR_H_
#define SRC_CRENITO_COMMONS_MODELO_SUSCRIPTOR_H_

#include "../utils.h"

/*
 * TAD para almacenar los datos luego de realizar una subscripción
 */
typedef struct {
	uint32_t id_proceso;
	int socket;
} t_suscriptor;

t_suscriptor* suscriptor_crear(int socket, uint32_t id_subscriptor);
t_suscriptor* suscriptor_duplicar(t_suscriptor* subscriptor);
void suscriptor_destruir(t_suscriptor* subscriptor);

bool suscriptor_existe(t_suscriptor* suscriptor);

uint32_t suscriptor_get_id_proceso(t_suscriptor* suscriptor);
int suscriptor_get_socket(t_suscriptor* suscriptor);

void suscriptor_set_socket(t_suscriptor* suscriptor, int socket);

#endif /* SRC_CRENITO_COMMONS_MODELO_SUSCRIPTOR_H_ */
