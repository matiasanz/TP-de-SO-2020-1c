/*
 * broker_utils.h
 *
 *  Created on: 14 jun. 2020
 *      Author: utnso
 */

#include "colas.h"

t_cola_container* cola_crear() {

	t_cola_container* cola = malloc(sizeof(t_cola_container));
	cola->cola = list_create();
	cola->suscriptores = list_create();
	pthread_mutex_init(&cola->mutex, NULL);

	return cola;
}

t_cola_container* get_cola(t_id_cola id_cola) {

	switch (id_cola) {
	case NEW_POKEMON:
		return cola_new_pokemon;
	case APPEARED_POKEMON:
		return cola_appeared_pokemon;
	case CATCH_POKEMON:
		return cola_catch_pokemon;
	case CAUGHT_POKEMON:
		return cola_caught_pokemon;
	case GET_POKEMON:
		return cola_get_pokemon;
	case LOCALIZED_POKEMON:
		return cola_localized_pokemon;
	default:
		log_error(event_logger, "No existe la cola: %d. Finalizando hilo", id_cola);
		pthread_exit(NULL);
		return NULL;
	}
}

void cola_eliminar_mensaje(uint32_t id_mensaje, t_id_cola id_cola) {

	t_cola_container* container = get_cola(id_cola);
	bool found = false;
	uint32_t index;

	for (int i = 0; i < list_size(container->cola); ++i) {

		t_mensaje_cache* candidato = list_get(container->cola, i);
		if (equals_from_id(candidato, id_mensaje)) {
			found = true;
			index = i;
			break;
		}
	}

	if (found) {
		pthread_mutex_lock(&container->mutex);
		list_remove_and_destroy_element(container->cola, index, (void*) mensaje_cache_destruir);
		pthread_mutex_unlock(&container->mutex);
	}
}

void encolar_mensaje(t_cola_container* container, t_mensaje_cache* msj) {

	pthread_mutex_lock(&container->mutex);
	list_add(container->cola, msj);
	pthread_mutex_unlock(&container->mutex);
}
