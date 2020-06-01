/*
 * hilo_appeared_pokemon.h
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#ifndef SRC_HILOS_HILO_APPEARED_POKEMON_H_
#define SRC_HILOS_HILO_APPEARED_POKEMON_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "../utils/mensajes-utils.h"

t_conexion_cliente* conexion_appeared_pokemon;
pthread_t hilo_appeared_pokemon;

void appeared_pokemon_recibido(t_mensaje_appeared_catch_pokemon* appeared_pokemon);
void subscribir_y_escuchar_cola_appeared_pokemon(void(*callback)(void*));

#endif /* SRC_HILOS_HILO_APPEARED_POKEMON_H_ */
