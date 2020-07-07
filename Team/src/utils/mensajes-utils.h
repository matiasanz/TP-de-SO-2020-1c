/*
 * mensajes-utils.h
 *
 *  Created on: 1 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_UTILS_MENSAJES_UTILS_H_
#define SRC_UTILS_MENSAJES_UTILS_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "../hilos-de-comunicacion/hilo_appeared_pokemon.h"
#include "../hilos-de-comunicacion/hilo_caught_pokemon.h"
#include "../hilos-de-comunicacion/hilo_gameboy.h"
#include "../hilos-de-comunicacion/hilo_localized_pokemon.h"
#include <stdbool.h>

bool ESPERO_MENSAJES;

pthread_mutex_t mutex_esperoMensajes;

void dejar_de_recibir();

void mensaje_recibido(t_id_cola id_cola, void* msj);

#endif /* SRC_UTILS_MENSAJES_UTILS_H_ */
