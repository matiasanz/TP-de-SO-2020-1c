/*
 * broker_utils.h
 *
 *  Created on: 14 jun. 2020
 *      Author: utnso
 */

#include "colas.h"

static void cola_set_id(t_cola_container* container, t_id_cola id_cola);
static void cola_buscar_y_eliminar_mensaje(uint32_t id_mensaje, t_cola_container* container);

t_cola_container* cola_crear(t_id_cola id_cola) {

	t_cola_container* container = malloc(sizeof(t_cola_container));

	container->cola = list_create();
	container->suscriptores = list_create();
	cola_set_id(container, id_cola);
	pthread_mutex_init(&container->mutex_mensajes, NULL);
	pthread_mutex_init(&container->mutex_suscriptores, NULL);

	return container;
}

static void cola_set_id(t_cola_container* container, t_id_cola id_cola) {

	container->id_cola = id_cola;

	t_id_cola* id_cola_ptr = malloc(sizeof(t_id_cola));
	*id_cola_ptr = id_cola;

	list_add(id_colas, id_cola_ptr);
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
		log_warning_cola(id_cola, "get_cola");
		return NULL;
	}
}

void cola_safe_buscar_y_eliminar_mensaje(uint32_t id_mensaje, t_id_cola id_cola) {

	t_cola_container* container = get_cola(id_cola);

	pthread_mutex_lock(&container->mutex_mensajes);
	cola_buscar_y_eliminar_mensaje(id_mensaje, container);
	pthread_mutex_unlock(&container->mutex_mensajes);

}

static void cola_buscar_y_eliminar_mensaje(uint32_t id_mensaje, t_cola_container* container) {

	bool id_mensaje_buscado(t_mensaje_cache* msj) {
		return particion_get_id_mensaje(msj->particion) == id_mensaje;
	}

	list_remove_and_destroy_by_condition(container->cola, (void*) id_mensaje_buscado, (void*) mensaje_cache_eliminar_de_cola_y_liberar_particion);
}

void cola_buscar_y_eliminar_mensajes(t_list* unos_mensajes, t_cola_container* container) {

	void buscar_y_eliminar(uint32_t* id_mensaje) {
		log_event_consolidacion_cola_eliminacion(*id_mensaje, cola_get_id(container));
		cola_buscar_y_eliminar_mensaje(*id_mensaje, container);
	}

	list_iterate(unos_mensajes, (void*) buscar_y_eliminar);
	//TODO: revisar si hay que destruir elementos
	list_destroy(unos_mensajes);
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

t_id_cola cola_get_id(t_cola_container* container) {
	return container->id_cola;
}

bool cola_esta_vacia(t_cola_container* container) {
	return list_is_empty(container->cola);
}

bool cola_inicia_flujo(t_id_cola id_cola) {
	return id_cola == CATCH_POKEMON || id_cola == GET_POKEMON || id_cola == NEW_POKEMON;
}

t_suscriptor* cola_buscar_suscriptor(uint32_t id_proceso, int id_socket, t_cola_container* container) {

	bool buscar_por_id(t_suscriptor* suscriptor) {
		return suscriptor_get_id_proceso(suscriptor) == id_proceso;
	}

	return list_find(container->suscriptores, (void*) buscar_por_id);
}
