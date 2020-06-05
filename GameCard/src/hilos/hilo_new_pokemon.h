/*
 * hilo_new_pokemon.h
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#ifndef SRC_HILOS_HILO_NEW_POKEMON_H_
#define SRC_HILOS_HILO_NEW_POKEMON_H_

#include <crenito-commons/conexiones/conexiones.h>

t_conexion_cliente* conexion_new_pokemon;
pthread_t hilo_new_pokemon;

void new_pokemon_recibido(t_mensaje_new_pokemon* new_pokemon);
void subscribir_y_escuchar_cola_new_pokemon(void (*callback)(t_id_cola, void*));

#endif /* SRC_HILOS_HILO_NEW_POKEMON_H_ */
