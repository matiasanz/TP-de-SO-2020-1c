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
	SUSCRIPCION = 1,
	MENSAJE = 2
} t_codigo_operacion;

typedef struct {
	t_codigo_operacion codigo_operacion;
	t_tipo_proceso tipo_proceso; //BROKER, GAMECARD, etc
	t_id_cola id_cola;
	int id_proceso; //Valor definido por archivo de configuración
}__attribute__((packed))
t_paquete_header;

// TAD para envios y recepcion de mensajes
typedef struct {
	t_paquete_header header;
	t_buffer* buffer;
} t_paquete;

// Funciones t_paquete*
t_paquete* paquete_crear(t_paquete_header header, t_buffer* buffer);
t_paquete_header paquete_header_crear(t_codigo_operacion cod_op, t_tipo_proceso tipo_proceso,
		t_id_cola id_cola, int id_proceso);
void paquete_destruir(t_paquete* paquete);

/* Recibe un paquete y un puntero donde guarda la cantidad de bytes del stream serializado
 * dichos bytes deben usarse como parametro al realizar el send */
void* paquete_serializar(t_paquete* paquete, int *bytes);

void* paquete_get_stream(t_paquete* paquete);
t_id_cola paquete_get_id_cola(t_paquete* paquete);


#endif /* SRC_CRENITO_COMMONS_PAQUETE_H_ */
