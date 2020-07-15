/*
 * broker_utils.h
 *
 *  Created on: 14 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_COLAS_H_
#define SRC_COLAS_H_

#include "memoria/memoria_modelo/mensaje_cache.h"
#include "utils/broker_utils.h"

// TAD cola
typedef struct {
	t_id_cola id_cola;
	t_list* cola;
	t_list* suscriptores;
	pthread_mutex_t mutex_mensajes;
	pthread_mutex_t mutex_suscriptores;
} t_cola_container;

//Las 6 colas de mensajes
t_cola_container* cola_new_pokemon;
t_cola_container* cola_appeared_pokemon;
t_cola_container* cola_catch_pokemon;
t_cola_container* cola_caught_pokemon;
t_cola_container* cola_get_pokemon;
t_cola_container* cola_localized_pokemon;

t_list* id_colas;

t_cola_container* cola_crear(t_id_cola id_cola);
t_cola_container* get_cola(t_id_cola id_cola);

void cola_buscar_y_eliminar_mensajes(t_list* unos_mensajes, t_cola_container* container);
void cola_safe_buscar_y_eliminar_mensaje(uint32_t id_mensaje, t_id_cola id_cola);

void encolar_mensaje(t_cola_container* container, t_mensaje_cache* msj);
int cola_get_cantidad_suscriptores(t_cola_container* container);
t_id_cola cola_get_id(t_cola_container* container);
bool cola_esta_vacia(t_cola_container* container);
bool cola_inicia_flujo(t_id_cola id_cola);
t_suscriptor* cola_buscar_suscriptor(uint32_t id_proceso, int id_socket, t_cola_container* container);

#endif /* SRC_COLAS_H_ */
