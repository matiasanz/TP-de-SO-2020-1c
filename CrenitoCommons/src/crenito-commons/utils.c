/*
 * utils.c
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#include"utils.h"

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
		log_warning(event_logger, "No existe la cola: %d. Funcion: %s. Finalizando hilo", id_cola, "get_nombre_cola");
		pthread_exit(NULL);
		return NULL;
	}
}

//TODO mover el bool a proceso.c y la parte de configuracion de lgs a log.c
bool mostrar_logs(){
	char*MOSTRAR_LOGS = "MOSTRAR_LOGS";
	return config_has_property(config, MOSTRAR_LOGS)
			&& config_get_int_value(config, MOSTRAR_LOGS);
} //si no lo valido y nos olvidamos de ponerlo, tira seg fault

char* get_log_path(char* TOKEN){
	return string_from_format("log/%s", config_get_string_value(config, TOKEN));
}

t_log* log_crear(char* PROCESO, char* FILENAME){

	char* LOG_PATH = get_log_path(FILENAME);
	t_log* log = log_create(LOG_PATH, PROCESO, mostrar_logs(), LOG_LEVEL_INFO);

	if(!log){
		FILE* f=fopen(LOG_PATH, "w+b");
		fclose(f);
		log = log_create(LOG_PATH, PROCESO, mostrar_logs(), LOG_LEVEL_INFO);
	}

	free(LOG_PATH);
	return log;
}

t_log* get_log_oficial(char* PROCESO){
	return log_crear(PROCESO, LOG_TOKEN);
}

t_log* get_log_event(char* PROCESO){
	return log_crear(PROCESO, EVENT_TOKEN);
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
	return indicador_conexion == ERROR_CONEXION;
}

int conexion_exitosa(int indicador_conexion) {
	return !error_conexion(indicador_conexion);
}
