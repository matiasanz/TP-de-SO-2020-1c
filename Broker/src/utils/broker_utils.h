/*
 * broker_utils.h
 *
 *  Created on: 16 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_UTILS_BROKER_UTILS_H_
#define SRC_UTILS_BROKER_UTILS_H_

#include <crenito-commons/conexiones/conexiones.h>

pthread_mutex_t mutex_id_univoco;
uint32_t id_univoco;
uint32_t generar_id_univoco();

//timeval utils
long get_fecha_en_microsegundos(struct timeval fecha);
void string_append_timestamp(char** string);

#endif /* SRC_UTILS_BROKER_UTILS_H_ */
