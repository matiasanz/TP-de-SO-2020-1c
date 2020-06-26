/*
 * mensaje_header.c
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#include "mensaje_header.h"

void mensaje_header_inicializar(t_mensaje_header* header) {

	header->id = 0;
	header->id_correlativo = 0;
}

t_mensaje_header mensaje_header_deserializar(void* stream) {

	// header
	t_mensaje_header header;

	memcpy(&header, stream, sizeof(t_mensaje_header));

	return header;
}

void string_append_mensaje_header(char** string, t_mensaje_header header,
		char* tipo) {

	char* string_header = mensaje_header_to_string(header, tipo);
	string_append_with_format(string, string_header);
	free(string_header);
}

char* mensaje_header_to_string(t_mensaje_header header, char* tipo) {

	char *string = string_new();

	string_append_separador(&string, LOG_HEADER_MENSAJE_RECIBIDO);
	string_append_with_format(&string, " mensaje: %s \n", tipo);
	string_append_with_format(&string, " id: %d, id_correlativo: %d \n",
			header.id, header.id_correlativo);

	return string;
}
