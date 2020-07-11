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

void string_append_timestamp(char** string) {

	time_t timer;
	char buffer[26];
	struct tm* tm_info;

	timer = time(NULL);
	tm_info = localtime(&timer);

	strftime(buffer, 26, "%d/%m/%Y %H:%M:%S", tm_info);
	string_append_with_format(string, "\n Dump: ");
	string_append_with_format(string, buffer);
	string_append_with_format(string, "\n");
}

