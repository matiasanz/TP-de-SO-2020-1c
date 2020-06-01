/*
 * hilo_caught_pokemon.h
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#ifndef SRC_HILO_CAUGHT_POKEMON_H_
#define SRC_HILO_CAUGHT_POKEMON_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "mensajes-utils.h"

t_conexion_cliente* conexion_caught_pokemon;
pthread_t hilo_caught_pokemon;

void caught_pokemon_recibido(t_mensaje_caught_pokemon* caught_pokemon);
void subscribir_y_escuchar_cola_caught_pokemon();

#endif /* SRC_HILO_CAUGHT_POKEMON_H_ */
