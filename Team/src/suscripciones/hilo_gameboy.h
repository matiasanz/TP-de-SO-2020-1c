/*
 * hilo_gameboy.h
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#ifndef SRC_SUSCRIPCIONES_HILO_GAMEBOY_H_
#define SRC_SUSCRIPCIONES_HILO_GAMEBOY_H_

#include <crenito-commons/conexiones/conexiones.h>

pthread_t hilo_gameboy;

void conectar_gameboy(void (*callback)(t_id_cola, void*));

#endif /* SRC_SUSCRIPCIONES_HILO_GAMEBOY_H_ */
