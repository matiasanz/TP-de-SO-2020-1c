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

char* get_separador_string(char* texto) {

	return string_from_format(" \n *************   %s   ************* \n",
			texto);
}

char* mensaje_header_to_string(t_mensaje_header header, char* tipo) {

	char *string = string_new();

	string_append_with_format(&string,
			get_separador_string(LOG_HEADER_MENSAJE_RECIBIDO));
	string_append_with_format(&string, " mensaje: %s \n", tipo);
	string_append_with_format(&string, " id: %d, id_correlativo: %d \n",
			header.id, header.id_correlativo);

	return string;
}

int error_conexion(int indicador_conexion) {
	return indicador_conexion == ERROR_SOCKET;
}

int conexion_exitosa(int indicador_conexion) {
	return !error_conexion(indicador_conexion);
}