/*
 * paquete.c
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#include "paquete.h"

t_buffer* cr_crear_buffer(int size) {

	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = size;

	buffer->stream = malloc(size);

	return buffer;
}

void cr_destruir_buffer(t_buffer* buffer) {

	free(buffer->stream);
	free(buffer);
}

t_paquete* cr_crear_paquete(int cod_op, t_buffer* buffer) {

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = cod_op;
	paquete->buffer = buffer;

	return paquete;
}

void cr_destruir_paquete(t_paquete* paquete) {

	cr_destruir_buffer(paquete->buffer);
	free(paquete);
}

void* cr_serializar_paquete(t_paquete* paquete, int *bytes) {

	*bytes = paquete->buffer->size + 2 * sizeof(int);
	void * stream = malloc(*bytes);

	int desplazamiento = 0;

	memcpy(stream + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(stream + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(stream + desplazamiento, paquete->buffer->stream,
			paquete->buffer->size);
	desplazamiento += paquete->buffer->size;

	return stream;
}
