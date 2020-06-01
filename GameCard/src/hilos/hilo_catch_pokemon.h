/*
 * hilo_catch_pokemon.h
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#ifndef SRC_HILOS_HILO_CATCH_POKEMON_H_
#define SRC_HILOS_HILO_CATCH_POKEMON_H_

#include <crenito-commons/conexiones/conexiones.h>

t_conexion_cliente* conexion_catch_pokemon;
pthread_t hilo_catch_pokemon;

void catch_pokemon_recibido(t_mensaje_appeared_catch_pokemon* catch_pokemon);
void subscribir_y_escuchar_cola_catch_pokemon(void(*callback)(void*));

#endif /* SRC_HILOS_HILO_CATCH_POKEMON_H_ */
