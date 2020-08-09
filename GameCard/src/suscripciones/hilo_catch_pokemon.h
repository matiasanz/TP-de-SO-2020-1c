/*
 * hilo_catch_pokemon.h
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#ifndef SRC_SUSCRIPCIONES_HILO_CATCH_POKEMON_H_
#define SRC_SUSCRIPCIONES_HILO_CATCH_POKEMON_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "../gamecard.h"

t_conexion_cliente* conexion_catch_pokemon;
pthread_t hilo_catch_pokemon;

void catch_pokemon_recibido(t_mensaje_appeared_catch_pokemon* catch_pokemon);
void suscribir_y_escuchar_cola_catch_pokemon(void (*callback)(t_id_cola, void*));

#endif /* SRC_SUSCRIPCIONES_HILO_CATCH_POKEMON_H_ */
