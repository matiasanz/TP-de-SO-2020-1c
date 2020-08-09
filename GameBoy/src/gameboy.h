/*
 * gameboy.h
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#ifndef SRC_GAMEBOY_H_
#define SRC_GAMEBOY_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "modos-procesamiento/mensaje.h"
#include "modos-procesamiento/modo_suscriptor.h"
#include "utils/validaciones_gameboy.h"

void inicializar();
void inicializar_logs();
void inicializar_config();

char*TIEMPO_CONEXION_STRING;

#endif /* SRC_GAMEBOY_H_ */
