/*
 * modo_suscriptor.h
 *
 *  Created on: 5 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_MODO_SUSCRIPTOR_H_
#define SRC_MODO_SUSCRIPTOR_H_

#include "validaciones_gameboy.h"
#include <crenito-commons/conexiones/conexiones.h>

pthread_t hilo_suscriptor;

bool modo_suscriptor(char*proceso);

void procesar_modo_suscriptor(char* cola_mensaje_string, char* tiempo_conexion_string);

#endif /* SRC_MODO_SUSCRIPTOR_H_ */
