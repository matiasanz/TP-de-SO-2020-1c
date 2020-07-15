/*
 * gameboy_utils.h
 *
 *  Created on: 10 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_GAMEBOY_UTILS_H_
#define SRC_GAMEBOY_UTILS_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "gameboy_logs.h"

t_id_cola get_id_mensaje(char* mensaje);
t_tipo_proceso proceso_obtener_tipo_segun_nombre(char* proceso_string);

#endif /* SRC_GAMEBOY_UTILS_H_ */
