/*
 * hilo_localized_pokemon.h
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#ifndef SRC_HILO_LOCALIZED_POKEMON_H_
#define SRC_HILO_LOCALIZED_POKEMON_H_

#include <crenito-commons/conexiones/conexiones.h>

t_conexion_cliente* conexion_localized_pokemon;
pthread_t hilo_localized_pokemon;

void localized_pokemon_recibido(t_mensaje_localized_pokemon* localized_pokemon);
void subscribir_y_escuchar_cola_localized_pokemon(void(*callback)(void*));

#endif /* SRC_HILO_LOCALIZED_POKEMON_H_ */
