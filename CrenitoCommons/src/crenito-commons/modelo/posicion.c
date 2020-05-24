/*
 * posicion.c
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#include "posicion.h"

t_posicion posicion_crear(uint32_t pos_x, uint32_t pos_y) {

	t_posicion posicion;
	posicion.pos_x = pos_x;
	posicion.pos_y = pos_y;

	return posicion;
}

t_posicion* posicion_crear_ptr(uint32_t pos_x, uint32_t pos_y) {

	t_posicion* posicion = malloc(sizeof(t_posicion));

	*posicion = posicion_crear(pos_x, pos_y);

	return posicion;
}
