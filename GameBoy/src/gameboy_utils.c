/*
 * gameboy_utils.c
 *
 *  Created on: 10 jul. 2020
 *      Author: utnso
 */

#include "gameboy_utils.h"

t_tipo_proceso get_tipo_proceso(char* proceso_string) {

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

t_id_cola get_id_mensaje(char* mensaje_string) {

	if (strcmp(mensaje_string, NEW_POKEMON_STRING) == 0) {
		return NEW_POKEMON;
	}
	if (strcmp(mensaje_string, APPEARED_POKEMON_STRING) == 0) {
		return APPEARED_POKEMON;
	}
	if (strcmp(mensaje_string, CATCH_POKEMON_STRING) == 0) {
		return CATCH_POKEMON;
	}
	if (strcmp(mensaje_string, CAUGHT_POKEMON_STRING) == 0) {
		return CAUGHT_POKEMON;
	}
	if (strcmp(mensaje_string, GET_POKEMON_STRING) == 0) {
		return GET_POKEMON;
	}
	if (strcmp(mensaje_string, LOCALIZED_POKEMON_STRING) == 0) {
		return LOCALIZED_POKEMON;
	}
	log_event_parametro_incorrecto("mensaje_string", mensaje_string);
	exit(EXIT_FAILURE);
	return 0;
}
