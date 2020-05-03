/*
 * paquete.c
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#include "paquete.h"

t_paquete* paquete_crear(int cod_op, t_id_proceso id_proceso, t_buffer* buffer) {

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = cod_op;
	paquete->codigo_operacion = id_proceso;
	paquete->buffer = buffer;

	return paquete;
}

void paquete_destruir(t_paquete* paquete) {

	buffer_destruir(paquete->buffer);
	free(paquete);
}

void* paquete_serializar(t_paquete* paquete, int *bytes) {

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

t_paquete_subscripcion* paquete_subscripcion_crear(
		t_id_proceso id_proceso, t_tipo_cola_mensaje nombre_cola) {

	t_paquete_subscripcion* msj = malloc(sizeof(t_paquete_subscripcion));

	msj->codigo_operacion = SUBSCRIPCION;
	msj->nombre_cola = nombre_cola;
	msj->id_proceso = id_proceso;

	return msj;
}

void paquete_subscripcion_destruir(t_paquete_subscripcion* msj){

	free(msj);
}
