/*
 * utils.c
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#include"utils.h"

void mensaje_header_inicializar(t_mensaje_header* header) {

	header->id = 0;
	header-> id_correlativo = 0;
}

t_log* get_crnito_logger() {

	if (crenito_logger != NULL)
		return crenito_logger;

	return log_create("./log/crenito_commons.log", "crenito_commons", 1,
			LOG_LEVEL_INFO);
}

char* get_nombre_proceso(t_id_proceso id_proceso) {

	switch (id_proceso) {
	case BROKER:
		return BROKER_STRING;
	case GAMEBOY:
		return GAMEBOY_STRING;
	case GAMECARD:
		return GAMECARD_STRING;
	case TEAM:
		return TEAM_STRING;
	default:
		log_error(get_crnito_logger(), "Id de proceso incorrecto: %d", id_proceso);
		abort();
		return NULL;
	}
}

char* get_nombre_cola(t_id_cola id_cola) {

	switch (id_cola) {
	case NEW_POKEMON:
		return NEW_POKEMON_STRING;
	case APPEARED_POKEMON:
		return APPEARED_POKEMON_STRING;
	case CATCH_POKEMON:
		return CATCH_POKEMON_STRING;
	case CAUGHT_POKEMON:
		return CAUGHT_POKEMON_STRING;
	case GET_POKEMON:
		return GET_POKEMON_STRING;
	case LOCALIZED_POKEMON:
		return LOCALIZED_POKEMON_STRING;
	default:
		log_error(get_crnito_logger(), "No existe el tipo de cola: %d",
				id_cola);
		abort();
		return NULL;
	}
}
