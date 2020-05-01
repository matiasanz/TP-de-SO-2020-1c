/*
 * conexiones.h
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_
#define SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_

#include "paquete.h"
#include "socket.h"

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

int subscripcion_cola(t_id_proceso id_proceso, t_tipo_cola_mensaje nombre_cola,
		char* ip, char* puerto);
char* get_nombre_proceso(t_id_proceso id);
char* get_nombre_cola_mensaje(t_tipo_cola_mensaje tipo_cola_mensaje);
int subscripcion_exitosa(int estado);

#endif /* SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_ */
