/*
 * memoria.c
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#include <crenito-commons/conexiones/conexiones.h>
#include "memoria.h"

//Funciones Privadas
static void* obtener_contenido_cache(t_mensaje_cache* mensaje_cache);

void inicializar_memoria() {

	uint32_t tamanio = config_get_int_value(config, "TAMANO_MEMORIA");
	memoria_principal = malloc(tamanio);
	pthread_mutex_init(&mutex_acceso_memoria, NULL);
	frecuencia_compactacion = config_get_int_value(config,
			"FRECUENCIA_COMPACTACION");
	tamanio_disponible = tamanio;

	//TODO: hacer esto solo si es particiones dinamicas
	particiones = dlist_create();
}

t_particion* guardar_en_cache(void* msj_recibido, t_id_cola id_cola) {

	t_particion* mensaje_cache = mensaje_cache_crear(id_cola);
	void* contenido = mensaje_cache_serializar(msj_recibido,
			mensaje_cache->metadata);
	//TODO: el id se podria generar durante la creacion pero hay que hacer que la funcion de mensaje_cache_serializar no lo pise
	mensaje_cache_generar_id(mensaje_cache);

	int tamanio = mensaje_cache_get_tamanio_contenido(mensaje_cache);

	//t_particion* particion = obtener_particion_libre(tamanio);
	//particion_set_uso(particion);

	t_particion* particion = NULL;

	t_particion* p1 = particion_crear(55, 0);
	particion_set_uso(p1);
	list_add(particiones, p1);
	p1->creacion = crear_time(2012, 2, 2);

	t_particion* p2 = particion_crear(2, 55);
	particion_set_uso(p1);
	p2->creacion = crear_time(2015, 2, 2);
	list_add(particiones, p2);

	t_particion* p3 = particion_crear(14, 57);
	particion_set_uso(p1);
	p3->creacion = crear_time(2012, 1, 2);
	list_add(particiones, p3);

	t_particion* p4 = particion_crear(13, 70);
	particion_set_uso(p1);
	p4->creacion = crear_time(2017, 2, 2);
	list_add(particiones, p4);

	//list_iterate(particiones, (void*) imprimir_particion);

	if (!particion_encontrada(particion)) {
		particion = liberar_memoria(tamanio);
	}

	if (!particion_encontrada(particion)) {
		log_error(event_logger,
				"no fue posible encontrar particion para el mensaje. Finalizando hilo");
		pthread_exit(NULL);
	}

	list_add(particiones, particion);
	imprimir_particion(particion);

	mensaje_cache_set_particion(mensaje_cache, particion);
	void* direccion_memoria = memoria_calcular_direccion(particion);
	memcpy(direccion_memoria, contenido, tamanio);

	list_iterate(particiones, (void*) imprimir_particion);
//	void* contenido_restaurado = malloc(tamanio);
//	void* contenido_restaurado = NULL;
	//	void* direccion_memoria = memoria_principal + particion -> inicio;

//	log_info(event_logger, "tamanio_particion: %d", particion_get_tamanio(particion));
//	log_info(event_logger, "tamanio_contenido: %d", tamanio);

//	memcpy(contenido_restaurado, memoria_principal, tamanio);

//	t_mensaje_appeared_catch_pokemon* appeared = mensaje_appeared_catch_pokemon_restaurar_desde_cache(contenido_restaurado, mensaje_cache -> metadata);

	return mensaje_cache;
}

void* mensaje_cache_restaurar_contenido(t_mensaje_cache* msj) {

//	void* deserializado;

	void* contenido_cache = obtener_contenido_cache(msj);

	switch (mensaje_cache_get_id_cola(msj)) {
//		case NEW_POKEMON:
//			return cola_new_pokemon;
	case APPEARED_POKEMON:
	case CATCH_POKEMON:
		return mensaje_appeared_catch_pokemon_restaurar_desde_cache(
				contenido_cache, msj->metadata);
//		case CAUGHT_POKEMON:
//			return cola_caught_pokemon;
//		case GET_POKEMON:
//			return cola_get_pokemon;
//		case LOCALIZED_POKEMON:
//			return cola_localized_pokemon;
	default:
		log_error(event_logger, "No existe la cola: %d. Finalizando hilo",
				mensaje_cache_get_id_cola(msj));
		pthread_exit(NULL);
		return NULL;
	}
}

void* mensaje_cache_serializar(void* msj_recibido, t_particion* particion) {

	switch (particion -> tipo) {
//		case NEW_POKEMON:
//			return cola_new_pokemon;
	case CATCH_POKEMON:
		return mensaje_appeared_catch_pokemon_formato_cache(msj_recibido,
				particion);
//		case CAUGHT_POKEMON:
//			return cola_caught_pokemon;
//		case GET_POKEMON:
//			return cola_get_pokemon;
//		case LOCALIZED_POKEMON:
//			return cola_localized_pokemon;
	default:
		log_error(event_logger, "No existe la cola: %d. Finalizando hilo",
				mensaje_metadata_get_id_cola(metadata));
		pthread_exit(NULL);
		return NULL;
	}
}

static void* obtener_contenido_cache(t_mensaje_cache* mensaje_cache) {

	int tamanio_contenido = mensaje_cache_get_tamanio_contenido(mensaje_cache);
	void* contenido = malloc(tamanio_contenido);
	void* direccion_memoria = memoria_calcular_direccion(
			mensaje_cache->particion);

	memcpy(contenido, direccion_memoria, tamanio_contenido);

	return contenido;

}

void* memoria_calcular_direccion(t_particion* particion) {

	return memoria_principal + particion_get_inicio(particion);
}


