/*
 * mensajes-utils.h
 *
 *  Created on: 1 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_UTILS_MENSAJES_UTILS_H_
#define SRC_UTILS_MENSAJES_UTILS_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "../hilos/hilo_catch_pokemon.h"
#include "../hilos/hilo_gameboy.h"
#include "../hilos/hilo_get_pokemon.h"
#include "../hilos/hilo_new_pokemon.h"

sem_t objetivos_gamecard;

void mensaje_recibido(t_id_cola id_cola, void* msj);

#endif /* SRC_UTILS_MENSAJES_UTILS_H_ */
