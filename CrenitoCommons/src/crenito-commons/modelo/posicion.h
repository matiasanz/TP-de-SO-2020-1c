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

char* posicion_to_string(t_posicion posicion);
char* posicion_list_to_string(t_list* posiciones);
void string_append_posicion(char** string, t_posicion posicion);
void string_append_posicion_list(char** string, t_list* posiciones);

#endif /* SRC_CRENITO_COMMONS_MODELO_POSICION_H_ */
