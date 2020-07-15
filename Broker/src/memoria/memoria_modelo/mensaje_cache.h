/*
 * mensaje_cache.h
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_MEMORIA_MEMORIA_MODELO_MENSAJE_CACHE_H_
#define SRC_MEMORIA_MEMORIA_MODELO_MENSAJE_CACHE_H_

#include "particion.h"
#include "../../utils/broker_log.h"

// Estructura adminitrativa de la memoria (contenido a poner en la cola de mensajes)
typedef struct {
	t_particion* particion;
	t_mensaje_cache_metadata* metadata;
	pthread_mutex_t mutex_suscriptores_enviados;
	pthread_mutex_t mutex_suscriptores_confirmados;
	pthread_mutex_t mutex_suscriptores_fallidos;
} t_mensaje_cache;

t_mensaje_cache* mensaje_cache_crear(t_mensaje_header msj_header);
void mensaje_cache_eliminar_de_cola_y_liberar_particion(t_mensaje_cache* msj);
void mensaje_cache_eliminar_de_cola(t_mensaje_cache* msj);
t_mensaje_header mensaje_header_restaurar_desde_cache(t_mensaje_cache* msj);

uint32_t mensaje_cache_get_id(t_mensaje_cache* msj);
uint32_t mensaje_cache_get_id_correlativo(t_mensaje_cache* msj);
uint32_t mensaje_cache_get_tamanio_contenido(t_mensaje_cache* msj);

void mensaje_cache_set_particion(t_mensaje_cache* msj, t_particion* particion);
t_list* mensaje_cache_get_suscriptores(t_mensaje_cache* msj, t_estado_envio estado_envio);
bool todos_los_suscriptores_confirmaron(t_mensaje_cache* msj, int suscriptores_cola);

int mensaje_cache_get_cantidad_suscriptores_fallidos(t_mensaje_cache* msj);
void mensaje_cache_set_ack(t_mensaje_cache* msj, t_suscriptor* suscriptor, int ack, uint32_t id_mensaje);

t_suscriptor* mensaje_cache_get_primer_sucriptor_fallido(t_mensaje_cache* msj);
bool mensaje_cache_pendiente_confirmacion(t_mensaje_cache* msj, t_suscriptor* suscriptor_buscado);

#endif /* SRC_MEMORIA_MEMORIA_MODELO_MENSAJE_CACHE_H_ */
