/*
 * mensaje_cache.h
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_MEMORIA_MENSAJE_CACHE_H_
#define SRC_MEMORIA_MENSAJE_CACHE_H_

#include "particion.h"

// Estructura adminitrativa de la memoria (contenido a poner en la cola de mensajes)
typedef struct {
	t_particion* particion;
	t_mensaje_cache_metadata* metadata;
} t_mensaje_cache;

t_mensaje_cache* mensaje_cache_crear();
void mensaje_cache_destruir(t_mensaje_cache*);

t_mensaje_header mensaje_header_restaurar_desde_cache(t_mensaje_cache* msj);
bool equals_from_id(t_mensaje_cache* msj, uint32_t id_buscado);

uint32_t mensaje_cache_get_id(t_mensaje_cache* msj);
t_id_cola mensaje_cache_get_id_cola(t_mensaje_cache* msj);
uint32_t mensaje_cache_get_tamanio_contenido(t_mensaje_cache* msj);

void mensaje_cache_set_particion(t_mensaje_cache* msj, t_particion* particion);
void mensaje_cache_set_suscriptor_confirmado(t_mensaje_cache* msj, t_suscriptor* suscriptor);
void mensaje_cache_set_suscriptor_enviado(t_mensaje_cache* msj, t_suscriptor* suscriptor);

#endif /* SRC_MEMORIA_MENSAJE_CACHE_H_ */
