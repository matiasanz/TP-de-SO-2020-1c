/*
 * proceso.c
 *
 *  Created on: 12 jul. 2020
 *      Author: utnso
 */

#include "proceso.h"

void proceso_inicializar(t_tipo_proceso tipo) {

	proceso.id_proceso = config_get_int_value(config, "ID_PROCESO");
	proceso.tipo_proceso = tipo;
}

bool proceso_debe_informar_suscripcion() {
	return proceso.tipo_proceso == GAMEBOY;
}

void log_proceso(t_log* un_logger, char* header_string, t_proceso proceso) {

	char *string = string_new();
	string_append_separador(&string, header_string);
	string_append_with_format(&string, " id proceso: %d \n", proceso_get_id(proceso));
	string_append_with_format(&string, " tipo: %s \n", proceso_get_nombre(proceso));
	log_info_and_destroy(un_logger, string);
}

uint32_t proceso_get_id(t_proceso proceso) {
	return proceso.id_proceso;
}

char* proceso_get_nombre(t_proceso proceso) {

	switch (proceso.tipo_proceso) {
	case BROKER:
		return BROKER_STRING;
	case GAMEBOY:
		return GAMEBOY_STRING;
	case GAMECARD:
		return GAMECARD_STRING;
	case TEAM:
		return TEAM_STRING;
	default:
		log_error(event_logger, "Tipo de proceso incorrecto: %d", proceso.tipo_proceso);
		return NULL;
	}
}
//TODO: refactorizar y usar string_equals_ignore_case
t_tipo_proceso proceso_obtener_tipo_segun_nombre(char* proceso_string) {

	if (strcmp(proceso_string, BROKER_STRING) == 0) {
		return BROKER;
	}
	if (strcmp(proceso_string, TEAM_STRING) == 0) {
		return TEAM;
	}
	if (strcmp(proceso_string, GAMECARD_STRING) == 0) {
		return GAMECARD;
	}
	if (strcmp(proceso_string, GAMEBOY_STRING) == 0) {
		return GAMEBOY;
	}
	log_event_parametro_incorrecto("proceso", proceso_string);
	exit(EXIT_FAILURE);
	return 0;
}

void log_event_parametro_incorrecto(char* tipo_error, char* contenido) {
	log_error(event_logger, "El %s %s  es inválido. Finalizando gameboy", tipo_error, contenido);
}
