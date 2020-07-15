
#include "crenito-commons_log.h"

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

	t_log* un_logger = proceso_debe_informar_reconexion() ? logger : event_logger;

	log_info(un_logger, "Inicio reintento de conexión cola %s. Se intentará conectar en %d segundos", get_nombre_cola(id_cola), segundos);
}

void log_resultado_proceso_reconexion(t_id_cola id_cola, int un_socket) {

	t_log* un_logger = proceso_debe_informar_reconexion() ? logger : event_logger;

	log_info(un_logger, "Resultado del reintento de conexión cola %s: %s \n", get_nombre_cola(id_cola), error_conexion(un_socket) ? "Fallido" : "Exitoso");
}

//GameBoy
void log_suscripcion(t_id_cola id_cola) {

	if (proceso_debe_informar_suscripcion())
		log_info(logger, "El Proceso %s se suscribió a la cola %s", GAMEBOY_STRING, get_nombre_cola(id_cola));
}

//Broker
void log_proceso(t_log* un_logger, char* header_string, t_proceso proceso) {

	char *string = string_new();
	string_append_separador(&string, header_string);
	string_append_with_format(&string, " id proceso: %d \n", proceso_get_id(proceso));
	string_append_with_format(&string, " tipo: %s \n", proceso_get_nombre(proceso));
	log_info_and_destroy(un_logger, string);
}

//Logs Errores
void log_warning_socket(int socket, char* operacion) {
	log_warning(event_logger, "Error al realizar la operación %s, socket: %d", operacion, socket);
}

void log_warning_cola(int id_cola, char* funcion) {
	log_warning(event_logger, "No existe la cola: %d. Funcion: %s. Finalizando hilo", id_cola, funcion);
	pthread_exit(NULL);
}

void log_event_parametro_incorrecto(char* tipo_error, char* contenido) {
	log_error(event_logger, "El %s %s  es inválido. Finalizando %s", tipo_error, contenido, GAMEBOY_STRING);
}


















//************************** Helgrind

















//#include "crenito-commons_log.h"
//
//void log_debug_and_destroy(t_log* un_logger, char* string) {
//}
//
//void log_info_and_destroy(t_log* un_logger, char* string) {
//}
//
//void log_warning_and_destroy(t_log* un_logger, char* string) {
//}
//
//void log_inicio_proceso_reconexion(t_id_cola id_cola, int segundos) {
//}
//
//void log_resultado_proceso_reconexion(t_id_cola id_cola, int un_socket) {
//}
//
//void log_suscripcion(t_id_cola id_cola) {
//}
//
//void log_proceso(t_log* un_logger, char* header_string, t_proceso proceso) {
//}
//
//void log_warning_socket(int socket, char* operacion) {
//}
//
//void log_warning_cola(int id_cola, char* funcion) {
//}
//
//void log_event_parametro_incorrecto(char* tipo_error, char* contenido) {
//}
