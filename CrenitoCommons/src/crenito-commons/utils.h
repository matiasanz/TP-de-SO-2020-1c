/*
 * uitils.h
 *
 *  Created on: 20 abr. 2020
 *      Author: utnso
 */

#ifndef UTILS_H_
#define UTILS_H_

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>

#define SUBSCRIPCION_EXITO 0
#define SUBSCRIPCION_ERROR 1

#define NEW_POKEMON_STRING "NEW_POKEMON"
#define APPEARED_POKEMON_STRING "APPEARED_POKEMON"
#define CATCH_POKEMON_STRING  "CATCH_POKEMON"
#define CAUGHT_POKEMON_STRING  "CAUGHT_POKEMON"
#define GET_POKEMON_STRING  "GET_POKEMON"
#define LOCALIZED_POKEMON_STRING  "LOCALIZED_POKEMON"

#define BROKER_STRING "BROKER"
#define GAMEBOY_STRING "GAME_BOY"
#define GAMECARD_STRING "GAME_CARD"
#define TEAM_STRING "TEAM"

// logger para las commons
t_log* crenito_logger;

//Estructura administrativa para ids de mensajes
typedef struct {
	uint32_t id;
	uint32_t id_correlativo;
} t_mensaje_id;

typedef enum {
	NEW_POKEMON = 1,
	APPEARED_POKEMON = 2,
	CATCH_POKEMON = 3,
	CAUGHT_POKEMON = 4,
	GET_POKEMON = 5,
	LOCALIZED_POKEMON = 6,
} t_tipo_cola_mensaje;

typedef enum {
	BROKER = 1, GAMEBOY = 2, GAMECARD = 3, TEAM = 4,SUSCRIPTOR=5
} t_id_proceso;

// Inicializa los ids para prevenir errores con valgrind
void mensaje_id_inicializar(t_mensaje_id* ids);
// singleton para el logger de las commons
t_log* get_crnito_logger();
// dado un id de proceso devuelve el mismo representado por un string
char* get_nombre_proceso(t_id_proceso id);
// dado un id de mensaje/nombre de cola el mismo representado por un string
char* get_nombre_cola_mensaje(t_tipo_cola_mensaje tipo_cola_mensaje);
#endif /* UTILS_H_ */
