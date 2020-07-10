/*
 * mensajes-utils.h
 *
 *  Created on: 1 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_UTILS_MENSAJES_UTILS_H_
#define SRC_UTILS_MENSAJES_UTILS_H_

#include <crenito-commons/conexiones/conexiones.h>
#include <stdbool.h>
#include "../listas/boolean_semaphore.h"
#include "../suscripciones/hilo_appeared_pokemon.h"
#include "../suscripciones/hilo_caught_pokemon.h"
#include "../suscripciones/hilo_gameboy.h"
#include "../suscripciones/hilo_localized_pokemon.h"

//bool ESPERO_MENSAJES;

pthread_mutex_t mutex_esperoMensajes;

boolean_sem_t BOOLSEM_EsperoMensajes;

void dejar_de_recibir();

void mensaje_recibido(t_id_cola id_cola, void* msj);

#endif /* SRC_UTILS_MENSAJES_UTILS_H_ */
