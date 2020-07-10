/*
 * broker_utils.c
 *
 *  Created on: 16 jun. 2020
 *      Author: utnso
 */

#include "broker_utils.h"

static void log_proceso(char* header_string, t_paquete_header header);
static void log_info_and_destroy(t_log* un_logger, char* string);
static void log_warning_and_destroy(t_log* un_logger, char* string);
static void string_append_mensaje_serializado(char** string, t_id_cola id_cola, void* msj_recibido);
static void string_append_cola(char** string, t_id_cola id_cola);
static void log_envio_mensaje(char* header_string, uint32_t id_mensaje, uint32_t id_suscriptor);
static void string_append_contenido_envio_mensaje(char** string, char* header_string, uint32_t id_mensaje,
		uint32_t id_suscriptor);

uint32_t generar_id_univoco() {

	uint32_t id_mensaje = 0;

	pthread_mutex_lock(&mutex_id_univoco);
	id_univoco += 1;
	id_mensaje = id_univoco;
	pthread_mutex_unlock(&mutex_id_univoco);

	return id_mensaje;
}

long get_fecha_en_microsegundos(struct timeval fecha) {
	return (fecha.tv_sec) * 1000 + (fecha.tv_usec) / 1000;
}

//Logs obligatorios
void log_conexion_proceso(t_paquete_header paquete_header) {
	log_proceso(LOG_HEADER_CONEXION_PROCESO, paquete_header);
}

void log_nuevo_suscriptor(t_paquete_header paquete_header) {

	char *header_string = string_new();
	string_append_with_format(&header_string, LOG_HEADER_NUEVO_SUSCRIPTOR,
			get_nombre_cola(paquete_header.id_cola));
	log_proceso(header_string, paquete_header);
	free(header_string);
}

void log_mensaje_recibido(t_id_cola id_cola, void* msj_recibido) {

	char *string = string_new();
	string_append_mensaje_serializado(&string, id_cola, msj_recibido);
	string_append_cola(&string, id_cola);
	log_info_and_destroy(logger, string);
}

void log_envio_mensaje_a_suscriptor(uint32_t id_mensaje, uint32_t id_suscriptor) {
	log_envio_mensaje(LOG_HEADER_ENVIO_MSJ_SUSCRIPTOR, id_mensaje, id_suscriptor);
}

void log_confirmacion_mensaje_suscriptor(uint32_t id_mensaje, uint32_t id_suscriptor) {
	log_envio_mensaje(LOG_HEADER_CONFIRMACION_MSJ_SUSCRIPTOR, id_mensaje, id_suscriptor);
}

void log_ejecucion_compactacion() {
	log_info(logger, get_separador_string(LOG_HEADER_COMPACTACION));
}

//Logs adicionales
void log_inicio_consolidacion_colas(char* nombre_cola, int cantidad_mensajes) {
	log_info(event_logger, "Inicio consolidacion cola %s, cantidad de mensajes: %d \n", nombre_cola,
			cantidad_mensajes);
}

void log_consolidacion_cola_eliminacion(uint32_t id_mensaje, t_id_cola id_cola) {
	log_info(event_logger, "Consolidacion cola %s, mensaje id eliminado: %d \n", get_nombre_cola(id_cola),
			id_mensaje);
}

void log_inicio_proceso() {
	log_info(event_logger, "%s iniciado exitosamente \n", BROKER_STRING);
}

//Logs Errores
void log_error_atender_cliente(int socket, t_paquete_header header) {

	log_error(event_logger,
			"El codigo de operacion %d recibido desde el socket: %d por el proceso con id: %d (%s) es incorrecto \n",
			header.codigo_operacion, socket, header.id_proceso, get_nombre_proceso(header.tipo_proceso));
}

void log_error_conexion_proceso() {
	log_error(event_logger, "Error al recibir el header del mensaje. Finalizando hilo \n");
}

void log_error_inicio_proceso() {
	log_error(event_logger, "Se produjo un error de conexion al iniciar el %s. Finzalizando proceso \n",
	BROKER_STRING);
}

void log_error_cola(int id_cola) {
	log_error(event_logger, "No existe la cola: %d. Finalizando hilo", id_cola);
	pthread_exit(NULL);
}

void log_error_get_suscriptores(uint32_t id_mensaje) {
	log_error(event_logger, "Error al obtener suscriptores asociados al mensaje con id: %d\n", id_mensaje);
}

void log_warning_envio_mensaje(uint32_t id_mensaje, uint32_t id_suscriptor) {

	char *string = string_new();

	string_append_contenido_envio_mensaje(&string, LOG_HEADER_WARNING_MSJ_SUSCRIPTOR, id_mensaje, id_suscriptor);
	log_warning_and_destroy(event_logger, string);
}

// Funciones Privadas
static void log_proceso(char* header_string, t_paquete_header header) {

	char *string = string_new();
	string_append_separador(&string, header_string);
	string_append_with_format(&string, " id proceso: %d \n", header.id_proceso);
	string_append_with_format(&string, " tipo: %s \n", get_nombre_proceso(header.tipo_proceso));
	log_info_and_destroy(logger, string);
}

static void log_info_and_destroy(t_log* un_logger, char* string) {

	log_info(un_logger, string);
	free(string);
}

static void log_warning_and_destroy(t_log* un_logger, char* string) {
	log_warning(un_logger, string);
	free(string);
}

static void string_append_mensaje_serializado(char** string, t_id_cola id_cola, void* msj_recibido) {

	void* deserializado;

	switch (id_cola) {
	case NEW_POKEMON:
		deserializado = mensaje_new_pokemon_deserializar(msj_recibido);
		string_append(string, mensaje_new_pokemon_to_string(deserializado));
		mensaje_new_pokemon_destruir(deserializado);
		break;
	case APPEARED_POKEMON:
		deserializado = mensaje_appeared_catch_pokemon_deserializar(msj_recibido);
		string_append(string, mensaje_appeared_catch_pokemon_to_string(deserializado, APPEARED_POKEMON_STRING));
		mensaje_appeared_catch_pokemon_destruir(deserializado);
		break;
	case CATCH_POKEMON:
		deserializado = mensaje_appeared_catch_pokemon_deserializar(msj_recibido);
		string_append(string, mensaje_appeared_catch_pokemon_to_string(deserializado, CATCH_POKEMON_STRING));
		mensaje_appeared_catch_pokemon_destruir(deserializado);
		break;
	case CAUGHT_POKEMON:
		deserializado = mensaje_caught_pokemon_deserializar(msj_recibido);
		string_append(string, mensaje_caught_pokemon_to_string(deserializado));
		mensaje_caught_pokemon_destruir(deserializado);
		break;
	case GET_POKEMON:
		deserializado = mensaje_get_pokemon_deserializar(msj_recibido);
		string_append(string, mensaje_get_pokemon_to_string(deserializado));
		mensaje_get_pokemon_destruir(deserializado);
		break;
	case LOCALIZED_POKEMON:
		deserializado = mensaje_localized_pokemon_deserializar(msj_recibido);
		string_append(string, mensaje_localized_pokemon_to_string(deserializado));
		mensaje_localized_pokemon_destruir(deserializado);
		break;
	default:
		log_error_cola(id_cola);
	}
}

static void log_envio_mensaje(char* header_string, uint32_t id_mensaje, uint32_t id_suscriptor) {

	char *string = string_new();

	string_append_contenido_envio_mensaje(&string, header_string, id_mensaje, id_suscriptor);
	log_info_and_destroy(logger, string);
}

static void string_append_cola(char** string, t_id_cola id_cola) {
	string_append_with_format(string, " cola: %s \n", get_nombre_cola(id_cola));
}

static void string_append_contenido_envio_mensaje(char** string, char* header_string, uint32_t id_mensaje,
		uint32_t id_suscriptor) {

	string_append_separador(string, header_string);
	string_append_with_format(string, " id mensaje: %d \n", id_mensaje);
	string_append_with_format(string, " id proceso: %d \n", id_suscriptor);
}
