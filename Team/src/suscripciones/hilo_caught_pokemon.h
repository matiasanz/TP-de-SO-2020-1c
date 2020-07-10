/*
 * hilo_caught_pokemon.h
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#ifndef SRC_SUSCRIPCIONES_HILO_CAUGHT_POKEMON_H_
#define SRC_SUSCRIPCIONES_HILO_CAUGHT_POKEMON_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "../utils/mensajes-utils.h"

t_conexion_cliente* conexion_caught_pokemon;
pthread_t hilo_caught_pokemon;

void caught_pokemon_recibido(t_mensaje_caught_pokemon* caught_pokemon);
void suscribir_y_escuchar_cola_caught_pokemon(void (*callback)(t_id_cola, void*));

#endif /* SRC_SUSCRIPCIONES_HILO_CAUGHT_POKEMON_H_ */
