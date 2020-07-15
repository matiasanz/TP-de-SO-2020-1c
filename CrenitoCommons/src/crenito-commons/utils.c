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
		log_warning_cola(id_cola, "get_nombre_cola");
		return NULL;
	}
}

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

//Logs Obligatorios
//Team
void log_inicio_proceso_reconexion(t_id_cola id_cola, int segundos) {
	log_info(logger, "Inicio reintento de conexión cola %s. Se intentará conectar en %d segundos",
			get_nombre_cola(id_cola), segundos);
}

void log_resultado_proceso_reconexion(t_id_cola id_cola, char* resultado) {
	log_info(logger, "Resultado del reintento de conexión cola %s: %s \n", get_nombre_cola(id_cola), resultado);
}

//GameBoy
//TODO mover todos los logs a un archivo aparte y preguntar acá adentro si debe loggearse
void log_suscripcion(t_id_cola id_cola) {
	log_info(logger, "El Proceso %s se suscribió a la cola %s", GAMEBOY_STRING, get_nombre_cola(id_cola));
}

//Logs Errores
void log_warning_socket(int socket, char* operacion) {
	log_warning(event_logger, "Error al realizar la operación %s, socket: %d", operacion, socket);
}

void log_warning_cola(int id_cola, char* funcion) {
	log_warning(event_logger, "No existe la cola: %d. Funcion: %s. Finalizando hilo", id_cola, funcion);
	pthread_exit(NULL);
}






















//************************** Helgrind











//void log_inicio_proceso_reconexion(t_id_cola id_cola, int segundos) {
//}
//
//void log_resultado_proceso_reconexion(t_id_cola id_cola, char* resultado) {
//}
//
//void log_suscripcion(t_id_cola id_cola) {
//}
//
//void log_warning_socket(int socket, char* operacion) {
//}
//
//void log_warning_cola(int id_cola, char* funcion) {
//}
