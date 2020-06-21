/*
 * broker_utils.h
 *
 *  Created on: 16 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_BROKER_UTILS_H_
#define SRC_BROKER_UTILS_H_

#include <crenito-commons/conexiones/conexiones.h>

#define LOG_HEADER_MENSAJE_ALMACENADO "MENSAJE ALMACENADO"
#define LOG_HEADER_PARTICION_ELIMINADA "PARTICION ELIMINADA"
#define LOG_HEADER_COMPACTACION "ALGORITMO DE COMPACTACIÓN EJECUTADO"

pthread_mutex_t mutex_id_univoco;
uint32_t id_univoco;
uint32_t generar_id_univoco();

long get_fecha_en_microsegundos(struct timeval fecha);

//Log
void log_ejecucion_compactacion();

#endif /* SRC_BROKER_UTILS_H_ */
