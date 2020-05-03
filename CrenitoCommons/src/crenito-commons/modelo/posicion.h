/*
 * posicion.h
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MODELO_POSICION_H_
#define SRC_CRENITO_COMMONS_MODELO_POSICION_H_

#include <stdint.h>

typedef struct {
	uint32_t pos_x;
	uint32_t pos_y;
} t_posicion;

t_posicion posicion_crear(uint32_t pos_x, uint32_t pos_y);

#endif /* SRC_CRENITO_COMMONS_MODELO_POSICION_H_ */
