/*
 * broker_utils.c
 *
 *  Created on: 16 jun. 2020
 *      Author: utnso
 */

#include "broker_utils.h"

uint32_t generar_id_univoco() {

	uint32_t id_mensaje = 0;

	pthread_mutex_lock(&mutex_id_univoco);
	id_univoco += 1;
	id_mensaje = id_univoco;
	pthread_mutex_unlock(&mutex_id_univoco);

	return id_mensaje;
}

long get_fecha_en_microsegundos(struct timeval fecha) {
	return (fecha.tv_sec) * 1000 + (fecha.tv_usec) / 1000;
}

void log_ejecucion_compactacion() {
	log_info(logger, get_separador_string(LOG_HEADER_COMPACTACION));
}
