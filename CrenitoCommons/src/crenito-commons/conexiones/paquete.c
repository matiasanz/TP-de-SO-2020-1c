/*
 * paquete.c
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#include "paquete.h"

t_paquete* paquete_crear(t_codigo_operacion cod_op, t_id_proceso id_proceso, t_tipo_cola_mensaje tipo_mensaje, t_buffer* buffer) {

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->header = crear_paquete_header(cod_op, id_proceso, tipo_mensaje);
	paquete->buffer = buffer;

	return paquete;
}

t_paquete_header crear_paquete_header(t_codigo_operacion cod_op, t_id_proceso id_proceso,
		t_tipo_cola_mensaje tipo_mensaje) {

	t_paquete_header header;
	header.codigo_operacion = cod_op;
	header.id_proceso = id_proceso;
	header.tipo_cola_mensaje = tipo_mensaje;
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

t_paquete_header* paquete_subscripcion_crear(
		t_id_proceso id_proceso, t_tipo_cola_mensaje nombre_cola) {

	t_paquete_header* msj = malloc(sizeof(t_paquete_header));

	msj->codigo_operacion = SUBSCRIPCION;
	msj->id_proceso = id_proceso;
	msj->tipo_cola_mensaje = nombre_cola;

	return msj;
}

void paquete_subscripcion_destruir(t_paquete_header* msj){

	free(msj);
}
