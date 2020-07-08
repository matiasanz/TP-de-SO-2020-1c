/*
 * gameboy.h
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#ifndef SRC_GAMEBOY_H_
#define SRC_GAMEBOY_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "validaciones_gameboy.h"
#include "mensaje.h"
#include "modo_suscriptor.h"

void inicializar();
void inicializar_logs();
void inicializar_config();

char*TIEMPO_CONEXION_STRING;

#endif /* SRC_GAMEBOY_H_ */
