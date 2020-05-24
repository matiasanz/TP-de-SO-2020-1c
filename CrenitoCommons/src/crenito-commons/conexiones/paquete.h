/*
 * paquete.h
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_PAQUETE_H_
#define SRC_CRENITO_COMMONS_PAQUETE_H_

#include "buffer.h"
#include "../utils.h"

typedef enum {
	SUBSCRIPCION = 1, MENSAJE = 2
} t_codigo_operacion;

// TAD estatico para manejar las subscripciones
typedef struct {
	t_codigo_operacion codigo_operacion;
	t_id_proceso id_proceso;
	t_tipo_cola_mensaje tipo_cola_mensaje;
}__attribute__((packed))
t_paquete_header;

// TAD para envios y recepcion de mensajes
typedef struct {
	t_paquete_header header;
	t_buffer* buffer;
} t_paquete;

// Funciones t_paquete*
t_paquete* paquete_crear(t_codigo_operacion cod_op, t_id_proceso id_proceso, t_tipo_cola_mensaje tipo_mensaje, t_buffer* buffer);
t_paquete_header crear_paquete_header(t_codigo_operacion cod_op, t_id_proceso id_proceso,
		t_tipo_cola_mensaje tipo_mensaje);
void paquete_destruir(t_paquete* paquete);

/* Recibe un paquete y un puntero donde guarda la cantidad de bytes del stream serializado
 * dichos bytes deben usarse como parametro al realizar el send */
void* paquete_serializar(t_paquete* paquete, int *bytes);

t_paquete_header* paquete_subscripcion_crear(t_id_proceso id_proceso,
		t_tipo_cola_mensaje nombre_cola);
void paquete_subscripcion_destruir(t_paquete_header* msj);

#endif /* SRC_CRENITO_COMMONS_PAQUETE_H_ */
