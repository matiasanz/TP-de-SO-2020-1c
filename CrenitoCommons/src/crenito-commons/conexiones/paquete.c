/*
 * paquete.c
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#include "paquete.h"

t_paquete* paquete_crear(t_paquete_header header, t_buffer* buffer) {

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->header = header;
	paquete->buffer = buffer;

	return paquete;
}

t_paquete_header paquete_header_crear(t_codigo_operacion cod_op, t_id_proceso id_proceso,
		t_id_cola id_cola) {

	t_paquete_header header;
	header.codigo_operacion = cod_op;
	header.id_proceso = id_proceso;
	header.id_cola = id_cola;
	return header;

}

void paquete_destruir(t_paquete* paquete) {

	buffer_destruir(paquete->buffer);
	free(paquete);
}

void* paquete_serializar(t_paquete* paquete, int *bytes) {

	*bytes = sizeof(t_paquete_header) +
			 paquete->buffer->size +
	         sizeof(paquete->buffer->size);
	
	void * stream = malloc(*bytes);

	int desplazamiento = 0;

	memcpy(stream + desplazamiento, &(paquete->header), sizeof(t_paquete_header));
	desplazamiento += sizeof(t_paquete_header);

	memcpy(stream + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(stream + desplazamiento, paquete->buffer->stream,
			paquete->buffer->size);
	desplazamiento += paquete->buffer->size;

	return stream;
}

void* paquete_get_stream(t_paquete* paquete) {

	return buffer_get_stream(paquete -> buffer);
}

t_id_cola paquete_get_id_cola(t_paquete* paquete) {
	return paquete -> header.id_cola;
}
