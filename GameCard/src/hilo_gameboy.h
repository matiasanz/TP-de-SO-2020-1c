/*
 * hilo_gameboy.h
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#ifndef SRC_HILO_GAMEBOY_H_
#define SRC_HILO_GAMEBOY_H_

#include <crenito-commons/conexiones/conexiones.h>

pthread_t hilo_gameboy;

void conectar_gameboy(void(*callback)(void*));


#endif /* SRC_HILO_GAMEBOY_H_ */
