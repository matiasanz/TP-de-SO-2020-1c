/*
 * hilo_localized_pokemon.h
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#ifndef SRC_SUSCRIPCIONES_HILO_LOCALIZED_POKEMON_H_
#define SRC_SUSCRIPCIONES_HILO_LOCALIZED_POKEMON_H_

#include <crenito-commons/conexiones/conexiones.h>

t_conexion_cliente* conexion_localized_pokemon;
pthread_t hilo_localized_pokemon;

void localized_pokemon_recibido(t_mensaje_localized_pokemon* localized_pokemon);
void suscribir_y_escuchar_cola_localized_pokemon(void (*callback)(t_id_cola, void*));
void mensaje_localized_pokemon_procesar(t_mensaje_localized_pokemon* mensaje);

#endif /* SRC_SUSCRIPCIONES_HILO_LOCALIZED_POKEMON_H_ */
