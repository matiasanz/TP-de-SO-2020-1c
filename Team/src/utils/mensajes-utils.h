/*
 * mensajes-utils.h
 *
 *  Created on: 1 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_UTILS_MENSAJES_UTILS_H_
#define SRC_UTILS_MENSAJES_UTILS_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "../hilos_comunicacion/hilo_appeared_pokemon.h"
#include "../hilos_comunicacion/hilo_caught_pokemon.h"
#include "../hilos_comunicacion/hilo_gameboy.h"
#include "../hilos_comunicacion/hilo_localized_pokemon.h"

sem_t objetivos_team;

void mensaje_recibido(t_id_cola id_cola, void* msj);

#endif /* SRC_UTILS_MENSAJES_UTILS_H_ */
