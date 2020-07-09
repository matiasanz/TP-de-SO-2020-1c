/*
 * utils.c
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#include"utils.h"

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
		return NULL;
	}
}

void string_append_separador(char** string, char* texto) {

	char* string_separador = get_separador_string(texto);
	string_append_with_format(string, string_separador);
	free(string_separador);
}

char* get_separador_string(char* texto) {

	return string_from_format(" \n *************   %s   ************* \n",
			texto);
}

int error_conexion(int indicador_conexion) {
	return indicador_conexion == ERROR_SOCKET;
}

int conexion_exitosa(int indicador_conexion) {
	return !error_conexion(indicador_conexion);
}

t_id_proceso get_id_proceso(char* proceso){
	if(strcmp(proceso,BROKER_STRING)==0){
		return BROKER;
	}
	if(strcmp(proceso,TEAM_STRING)==0){
		return TEAM;
	}
	if(strcmp(proceso,GAMECARD_STRING)==0){
		return GAMECARD;
	}
	if(strcmp(proceso,GAMEBOY_STRING)==0){
		return GAMEBOY;
	}
	log_error(event_logger,"este proceso no es conocido %s",proceso);
	return 0;
}

t_id_cola get_id_mensaje(char* mensaje){

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
	if(strcmp(mensaje,LOCALIZED_POKEMON_STRING)==0){
		return LOCALIZED_POKEMON;
	}
	log_error(event_logger,"este mensaje no es conocido %s",mensaje);
	return 0;
}

t_suscriptor* suscriptor_crear(uint32_t socket, uint32_t id_suscriptor) {

	t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));

	suscriptor->id_subcriptor = id_suscriptor;
	suscriptor->socket = socket;

	return suscriptor;
}

void suscriptor_destruir(t_suscriptor* suscriptor) {
	free(suscriptor);
}
