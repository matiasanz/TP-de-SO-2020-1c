/*
 * hilo_get_pokemon.h
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#ifndef SRC_HILO_GET_POKEMON_H_
#define SRC_HILO_GET_POKEMON_H_

#include <crenito-commons/conexiones/conexiones.h>

t_conexion_cliente* conexion_get_pokemon;
pthread_t hilo_get_pokemon;

void get_pokemon_recibido(t_mensaje_get_pokemon* get_pokemon);
void subscribir_y_escuchar_cola_get_pokemon(void(*callback)(void*));

#endif /* SRC_HILO_GET_POKEMON_H_ */
