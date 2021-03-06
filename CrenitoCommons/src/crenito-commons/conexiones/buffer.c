/*
 * buffer.c
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#include "buffer.h"

t_buffer* buffer_crear(int size) {

	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = size;

	buffer->stream = malloc(size);

	return buffer;
}

void buffer_destruir(t_buffer* buffer) {
	free(buffer -> stream);
	free(buffer);
}

void buffer_set_stream(t_buffer* buffer, void* stream){

	buffer -> stream = stream;
}

void* buffer_get_stream(t_buffer* buffer){
	return buffer -> stream;
}
