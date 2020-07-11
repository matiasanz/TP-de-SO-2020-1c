/*
 * broker_utils.h
 *
 *  Created on: 14 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_COLAS_H_
#define SRC_COLAS_H_

#include "utils/broker_utils.h"
#include "memoria/mensaje_cache.h"

// TAD cola
typedef struct {
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

t_cola_container* cola_crear();
t_cola_container* get_cola(t_id_cola id_cola);

void cola_buscar_y_eliminar_mensaje(uint32_t id_mensaje, t_id_cola id_cola);
void encolar_mensaje(t_cola_container* container, t_mensaje_cache* msj);
int cola_get_cantidad_suscriptores(t_cola_container* container);

#endif /* SRC_COLAS_H_ */
