/*
 * uitils.h
 *
 *  Created on: 20 abr. 2020
 *      Author: utnso
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>

//Estructura administrativa para ids de mensajes
typedef struct {
	uint32_t id;
	uint32_t id_correlativo;
} t_mensaje_id;

void mensaje_id_inicializar(t_mensaje_id* ids);
#endif /* UTILS_H_ */
