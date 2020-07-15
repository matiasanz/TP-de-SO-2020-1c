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

#define ERROR_CONEXION -1

#define LOG_HEADER_MENSAJE_RECIBIDO "MENSAJE RECIBIDO"

#define NEW_POKEMON_STRING "NEW_POKEMON"
#define APPEARED_POKEMON_STRING "APPEARED_POKEMON"
#define CATCH_POKEMON_STRING  "CATCH_POKEMON"
#define CAUGHT_POKEMON_STRING  "CAUGHT_POKEMON"
#define GET_POKEMON_STRING  "GET_POKEMON"
#define LOCALIZED_POKEMON_STRING  "LOCALIZED_POKEMON"

//TODO: mover a proceso.c
#define BROKER_STRING "BROKER"
#define GAMEBOY_STRING "GAMEBOY"
#define GAMECARD_STRING "GAMECARD"
#define TEAM_STRING "TEAM"
#define SUSCRIPTOR_STRING "SUSCRIPTOR"

#define LOG_TOKEN "LOG_FILE"
#define EVENT_TOKEN "EVENT_FILE"

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

// dado un id de mensaje/nombre de cola el mismo representado por un string
char* get_nombre_cola(t_id_cola id_cola);

/* Funcion booleana que determina si los logs se muestran por pantalla
 * a partir del config.
 * Retorna true si el config contiene un token MOSTRAR_LOGS=1
 * En caso de no encontrarlo, por defecto devuelve false*/
bool mostrar_logs();

/* Crea un logger con la direccion que le indica el config,
 * quien a su vez le indica si debe mostrar los logs por pantalla.
 * Level info, con el nombre del proceso*/
t_log* get_log_oficial(char* PROCESO);
t_log* get_log_event(char* PROCESO);

void string_append_separador(char** string, char* texto);
char* get_separador_string(char* texto);

int conexion_exitosa(int indicador_conexion);
int error_conexion(int indicador_conexion);

t_id_cola get_id_mensaje(char* mensaje);

#endif /* UTILS_H_ */
