/*
 * uitils.h
 *
 *  Created on: 20 abr. 2020
 *      Author: utnso
 */

#ifndef UTILS_H_
#define UTILS_H_

// En utils.h estan los headers y variables globales compartidas entre procesos
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<pthread.h>
#include<semaphore.h>

#include<commons/log.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/string.h>
#include <commons/collections/dictionary.h>

#define ERROR_SOCKET -1

#define LOG_HEADER_MENSAJE_RECIBIDO "MENSAJE RECIBIDO"

#define NEW_POKEMON_STRING "NEW_POKEMON"
#define APPEARED_POKEMON_STRING "APPEARED_POKEMON"
#define CATCH_POKEMON_STRING  "CATCH_POKEMON"
#define CAUGHT_POKEMON_STRING  "CAUGHT_POKEMON"
#define GET_POKEMON_STRING  "GET_POKEMON"
#define LOCALIZED_POKEMON_STRING  "LOCALIZED_POKEMON"

#define BROKER_STRING "BROKER"
#define GAMEBOY_STRING "GAMEBOY"
#define GAMECARD_STRING "GAMECARD"
#define TEAM_STRING "TEAM"
#define SUSCRIPTOR_STRING "SUSCRIPTOR"

t_config* config;

//Logger para uso exclusivo de informacion pedida por el tp
t_log* logger;
//Logger para grabar informacion adicional/errores
t_log* event_logger;

typedef enum {
	NEW_POKEMON = 1,
	APPEARED_POKEMON = 2,
	CATCH_POKEMON = 3,
	CAUGHT_POKEMON = 4,
	GET_POKEMON = 5,
	LOCALIZED_POKEMON = 6,
} t_id_cola;

typedef enum {
	BROKER = 1,
	GAMEBOY = 2,
	GAMECARD = 3,
	TEAM = 4
} t_tipo_proceso;

// dado un tipo de proceso devuelve el mismo representado por un string
char* get_nombre_proceso(t_tipo_proceso tipo_proceso);
// dado un id de mensaje/nombre de cola el mismo representado por un string
char* get_nombre_cola(t_id_cola id_cola);

void string_append_separador(char** string, char* texto);
char* get_separador_string(char* texto);

int conexion_exitosa(int indicador_conexion);
int error_conexion(int indicador_conexion);
t_tipo_proceso get_tipo_proceso(char* proceso);
t_id_cola get_id_mensaje(char* mensaje);

void log_info_and_destroy(t_log* un_logger, char* string);
void log_warning_and_destroy(t_log* un_logger, char* string);

// Logs Adicionales

// Logs Errores
void log_warning_socket(int socket, char* operacion);
void log_error_cola(int id_cola);
void log_warning_suscripcion(t_id_cola id_cola);
void log_warning_conexion_perdida(t_id_cola id_cola);
void log_warning_broker_desconectado(t_id_cola id_cola);

#endif /* UTILS_H_ */
