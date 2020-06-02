/*
 * gameboy.h
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#ifndef SRC_GAMEBOY_H_
#define SRC_GAMEBOY_H_

#include "crenito-commons/conexiones/conexiones.h"

t_conexion_server conexion_broker;
t_conexion_server conexion_team;
t_conexion_server conexion_gamecard;

void inicializar_logs();
void inicializar_config();s

#endif /* SRC_GAMEBOY_H_ */
