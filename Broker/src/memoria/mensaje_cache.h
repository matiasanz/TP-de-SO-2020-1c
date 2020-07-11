/*
 * mensaje_cache.h
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_MEMORIA_MENSAJE_CACHE_H_
#define SRC_MEMORIA_MENSAJE_CACHE_H_

#include "particion.h"
#include "../utils/broker_log.h"

// Estructura adminitrativa de la memoria (contenido a poner en la cola de mensajes)
typedef struct {
	t_particion* particion;
	t_mensaje_cache_metadata* metadata;
	pthread_mutex_t mutex_edicion_mensaje;
} t_mensaje_cache;

t_mensaje_cache* mensaje_cache_crear();
void mensaje_cache_eliminar_de_cola(t_mensaje_cache* msj);
t_mensaje_header mensaje_header_restaurar_desde_cache(t_mensaje_cache* msj);

uint32_t mensaje_cache_get_id(t_mensaje_cache* msj);
t_id_cola mensaje_cache_get_id_cola(t_mensaje_cache* msj);
uint32_t mensaje_cache_get_tamanio_contenido(t_mensaje_cache* msj);

void mensaje_cache_set_particion(t_mensaje_cache* msj, t_particion* particion);
void mensaje_cache_agregar_suscriptor(t_mensaje_cache* msj, t_suscriptor* suscriptor, t_estado_envio estado_envio);
t_list* mensaje_cache_get_suscriptores(t_mensaje_cache* msj, t_estado_envio estado_envio);
bool todos_los_suscriptores_confirmaron(t_mensaje_cache* msj, int suscriptores_cola);

#endif /* SRC_MEMORIA_MENSAJE_CACHE_H_ */
