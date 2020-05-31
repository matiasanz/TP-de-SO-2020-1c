/*
 * posicion.h
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MODELO_POSICION_H_
#define SRC_CRENITO_COMMONS_MODELO_POSICION_H_

#include "../utils.h"

typedef struct {
	uint32_t pos_x;
	uint32_t pos_y;
} t_posicion;

t_posicion posicion_crear(uint32_t pos_x, uint32_t pos_y);
t_posicion* posicion_crear_ptr(uint32_t pos_x, uint32_t pos_y);
void posicion_destruir(t_posicion* posicion);

void posicion_log(t_log* un_logger, t_posicion posicion);
void posicion_list_log(t_log* un_logger, t_list* posiciones);

#endif /* SRC_CRENITO_COMMONS_MODELO_POSICION_H_ */
