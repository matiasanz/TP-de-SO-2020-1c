/*
 * paquete.h
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_PAQUETE_H_
#define SRC_CRENITO_COMMONS_PAQUETE_H_

#include <stdlib.h>
#include <string.h>

typedef enum {
	SUBSCRIPCION = 1, MENSAJE = 2
} t_codigo_operacion;

typedef enum {
	NEW_POKEMON = 1,
	APPEARED_POKEMON = 2,
	CATCH_POKEMON = 3,
	CAUGHT_POKEMON = 4,
	GET_POKEMON = 5,
	LOCALIZED_POKEMON = 6,
} t_tipo_cola_mensaje;

typedef enum {
	BROKER = 1, GAMEBOY = 2, GAMECARD = 3, TEAM = 4
} t_id_proceso;

// TAD estatico para manejar las subscripciones
typedef struct {
	t_codigo_operacion codigo_operacion;
	t_id_proceso id_proceso;
	t_tipo_cola_mensaje nombre_cola;
}__attribute__((packed))
t_mensaje_subscripcion;

// TAD dinamico para manejar el contenido y tamaño de mensajes
typedef struct {
	int size;
	void* stream;
} t_buffer;

// TAD para envios y recepcion de mensajes
typedef struct {
	t_codigo_operacion codigo_operacion;
	t_id_proceso id_proceso;
	t_buffer* buffer;
} t_paquete;

// Funciones t_paquete*
t_paquete* paquete_crear(int cod_op, t_id_proceso id_proceso, t_buffer* buffer);
void paquete_destruir(t_paquete* paquete);
void* paquete_serializar(t_paquete* paquete, int *bytes);

//Funciones t_buffer*
t_buffer* buffer_crear(int size);
void buffer_destruir(t_buffer* buffer);

// Estas 2 funciones deberian ir a un mensaje.h?
t_mensaje_subscripcion* mensaje_subscripcion_crear(
		t_id_proceso id_proceso, t_tipo_cola_mensaje nombre_cola);
void mensaje_subscripcion_destruir(t_mensaje_subscripcion* msj);

#endif /* SRC_CRENITO_COMMONS_PAQUETE_H_ */
