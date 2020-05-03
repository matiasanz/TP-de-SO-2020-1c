/*
 * buffer.h
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_BUFFER_H_
#define SRC_CRENITO_COMMONS_BUFFER_H_

#include "../utils.h"

// TAD dinamico para manejar el contenido y tamaño de mensajes
typedef struct {
	uint32_t size;
	void* stream;
} t_buffer;


t_buffer* buffer_crear(int size);
void buffer_destruir(t_buffer* buffer);

#endif /* SRC_CRENITO_COMMONS_BUFFER_H_ */
