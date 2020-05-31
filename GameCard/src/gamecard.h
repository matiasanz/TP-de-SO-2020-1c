/*
 * gamecard.h
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_GAMECARD_H_
#define SRC_GAMECARD_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "hilo_catch_pokemon.h"
#include "hilo_get_pokemon.h"
#include "hilo_new_pokemon.h"

sem_t objetivos_gamecard;

void inicializar();
void inicializar_config();
void inicializar_logs();
void inicializar_conexiones();
void subscribir_y_escuchar_cola_catch_pokemon();
void subscribir_y_escuchar_cola_get_pokemon();
void subscribir_y_escuchar_cola_new_pokemon();


#endif /* SRC_GAMECARD_H_ */
