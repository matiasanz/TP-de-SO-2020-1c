/*
 * mensaje_header.h
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_HEADER_H_
#define SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_HEADER_H_

#define LOG_HEADER_MENSAJE_RECIBIDO "MENSAJE RECIBIDO"

#include "../utils.h"

//Estructura administrativa para ids de mensajes
typedef struct {
	uint32_t id;
	uint32_t id_correlativo;
} t_mensaje_header;

// Inicializa los ids para prevenir errores con valgrind
void mensaje_header_inicializar(t_mensaje_header* header);

t_mensaje_header mensaje_header_deserializar(void* stream);

char* mensaje_header_to_string(t_mensaje_header header, char* tipo);
void string_append_mensaje_header(char** string, t_mensaje_header header, char* tipo);

#endif /* SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_HEADER_H_ */
