/*
 * utils.c
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#include"utils.h"

char* get_nombre_proceso(t_tipo_proceso tipo_proceso) {

	switch (tipo_proceso) {
	case BROKER:
		return BROKER_STRING;
	case GAMEBOY:
		return GAMEBOY_STRING;
	case GAMECARD:
		return GAMECARD_STRING;
	case TEAM:
		return TEAM_STRING;
	default:
		log_error(event_logger, "Tipo de proceso incorrecto: %d", tipo_proceso);
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
		log_error_cola(id_cola);
		return NULL;
	}
}

void string_append_separador(char** string, char* texto) {

	char* string_separador = get_separador_string(texto);
	string_append_with_format(string, string_separador);
	free(string_separador);
}

char* get_separador_string(char* texto) {

	return string_from_format(" \n *************   %s   ************* \n", texto);
}

int error_conexion(int indicador_conexion) {
	return indicador_conexion == ERROR_SOCKET;
}

int conexion_exitosa(int indicador_conexion) {
	return !error_conexion(indicador_conexion);
}

void log_debug_and_destroy(t_log* un_logger, char* string) {

	log_debug(un_logger, string);
	free(string);
}

void log_info_and_destroy(t_log* un_logger, char* string) {

	log_info(un_logger, string);
	free(string);
}

void log_warning_and_destroy(t_log* un_logger, char* string) {

	log_warning(un_logger, string);
	free(string);
}

//Logs Adicionales

//Logs Errores
void log_warning_socket(int socket, char* operacion){
	log_warning(event_logger,
				"Error al realizar la operación %s, socket: %d", operacion, socket);
}

void log_error_cola(int id_cola) {
	log_error(event_logger, "No existe la cola: %d. Finalizando hilo", id_cola);
	pthread_exit(NULL);
}

void log_warning_suscripcion(t_id_cola id_cola) {
	log_warning(event_logger, "No se pudo conectar al proceso %s, cancelando subscripción %s \n",
	BROKER_STRING, get_nombre_cola(id_cola));

}

void log_warning_conexion_perdida(t_id_cola id_cola) {
	log_warning(event_logger, "Se perdió la conexión con el %s, cancelando escucha sobre la cola %s",
	BROKER_STRING, get_nombre_cola(id_cola));
}

void log_warning_broker_desconectado(t_id_cola id_cola) {
	log_warning(event_logger, "el %s está desconectado, cancelando subscripción %s",
	BROKER_STRING, get_nombre_cola(id_cola));
}
