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
		log_error(event_logger, "Id de proceso incorrecto: %d", id_proceso);
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
		log_error(event_logger, "No existe el tipo de cola: %d",
				id_cola);
		abort();
		return NULL;
	}
}

int get_id_proceso(char* proceso){
	if(strcmp(proceso,BROKER_STRING)==0){
		return BROKER;
	}
	if(strcmp(proceso,TEAM_STRING)==0){
		return TEAM;
	}
	if(strcmp(proceso,GAMECARD_STRING)==0){
		return GAMECARD;
	}
}
int get_id_mensaje(char* mensaje){
	if(strcmp(mensaje,NEW_POKEMON_STRING)==0){
			return NEW_POKEMON;
	}
	if(strcmp(mensaje,APPEARED_POKEMON_STRING)==0){
			return APPEARED_POKEMON;
	}
	if(strcmp(mensaje,CATCH_POKEMON_STRING)==0){
			return CATCH_POKEMON;
	}
	if(strcmp(mensaje,CAUGHT_POKEMON_STRING)==0){
			return CAUGHT_POKEMON;
	}
	if(strcmp(mensaje,GET_POKEMON_STRING)==0){
			return GET_POKEMON;
	}

}
