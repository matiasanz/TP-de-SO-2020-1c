/*
 * gameboy_utils.h
 *
 *  Created on: 10 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_GAMEBOY_UTILS_H_
#define SRC_GAMEBOY_UTILS_H_

#include <crenito-commons/utils.h>
#include "gameboy_logs.h"

t_tipo_proceso get_tipo_proceso(char* proceso_string);
t_id_cola get_id_mensaje(char* mensaje);

#endif /* SRC_GAMEBOY_UTILS_H_ */
