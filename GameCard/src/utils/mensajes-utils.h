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

void gamecard_enviar_localized(t_mensaje_get_pokemon* mensajeGet, t_list*posiciones);

void gamecard_enviar_appeared(t_mensaje_new_pokemon* mensajeNew);

#endif /* SRC_UTILS_MENSAJES_UTILS_H_ */
