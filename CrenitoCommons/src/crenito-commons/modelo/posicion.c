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

void posicion_destruir(t_posicion* posicion) {

	free(posicion);
}

char* posicion_to_string(t_posicion posicion) {

	return string_from_format(" (%d, %d)", posicion.pos_x, posicion.pos_y);
}

char* posicion_list_to_string(t_list* posiciones) {

	char *string = string_new();

	string_append_with_format(&string, " posiciones: %d", list_size(posiciones));

	if (list_is_empty(posiciones)) return string;

	string_append_with_format(&string, " -->", list_size(posiciones));

	void append_posicion(t_posicion* posicion){
		string_append_posicion(&string, *posicion);
	}

	list_iterate(posiciones, (void*) append_posicion);

	return string;
}

void string_append_posicion_list(char** string, t_list* posiciones) {

	char* string_pos_list = posicion_list_to_string(posiciones);
	string_append_with_format(string, string_pos_list);
	free(string_pos_list);
}

void string_append_posicion(char** string, t_posicion posicion) {

	char * string_pos = string_from_format(" (%d, %d)", posicion.pos_x, posicion.pos_y);
	string_append_with_format(string, string_pos);
	free(string_pos);
}
