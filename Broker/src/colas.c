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
	pthread_mutex_init(&cola->mutex_mensajes, NULL);
	pthread_mutex_init(&cola->mutex_suscriptores, NULL);
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
		log_error_cola(id_cola);
		return NULL;
	}
}


//***************************************
bool cola_esta_vacia(t_cola_container* cola){
	return list_is_empty(cola->cola);
}

bool cola_tiene_suscriptores(t_cola_container* container){
	return cola_get_cantidad_suscriptores(container);
}

void cola_remove_and_destroy_by_id(uint32_t id_mensaje, t_cola_container* container){
	bool id_mensaje_buscado(t_mensaje_cache* msj) {
		return particion_get_id_mensaje(msj->particion) == id_mensaje;
	}

	list_remove_and_destroy_by_condition(container->cola, (void*) id_mensaje_buscado,
				(void*) mensaje_cache_eliminar_de_cola);
}
//**************************************

void cola_buscar_y_eliminar_mensaje(uint32_t id_mensaje, t_id_cola id_cola) {

	t_cola_container* container = get_cola(id_cola);

	pthread_mutex_lock(&container->mutex_mensajes);
	cola_remove_and_destroy_by_id(id_mensaje, container);
	pthread_mutex_unlock(&container->mutex_mensajes);
}

void encolar_mensaje(t_cola_container* container, t_mensaje_cache* msj) {

	pthread_mutex_lock(&container->mutex_mensajes);
	list_add(container->cola, msj);
	pthread_mutex_unlock(&container->mutex_mensajes);
}

int cola_get_cantidad_suscriptores(t_cola_container* container) {

	pthread_mutex_lock(&container->mutex_suscriptores);
	int cantidad_suscriptores = list_size(container->suscriptores);
	pthread_mutex_unlock(&container->mutex_suscriptores);

	return cantidad_suscriptores;
}


