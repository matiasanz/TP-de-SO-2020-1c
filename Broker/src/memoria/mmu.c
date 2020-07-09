/*
 * memoria.c
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#include "mmu.h"

//Funciones Privadas
static void* obtener_contenido_cache(t_mensaje_cache* mensaje_cache);
static t_particion* obtener_particion_libre(int tamanio_contenido);
static bool debe_ejecutarse_compactacion();
static void inicializar_buddy_system();
static void inicializar_particiones_dinamicas();

//TODO implementar
static void inicializar_buddy_system() {
}

//TODO: validar valores de configuracion y lanzar error si correspone
void inicializar_particiones_dinamicas() {
	//"FF" o "BF"
	char* algoritmo_libres = config_get_string_value(config, "ALGORITMO_PARTICION_LIBRE");

	algoritmo_particion_libre =
			string_equals_ignore_case(algoritmo_libres, "BF") ?
					(void*) pd_obtener_particion_libre_best_fit : (void*) pd_obtener_particion_libre_first_fit;

	//"FIFO" o "LRU"
	char* algoritmo_reemplazo_string = config_get_string_value(config, "ALGORITMO_REEMPLAZO");

	algoritmo_reemplazo =
			string_equals_ignore_case(algoritmo_reemplazo_string, "FIFO") ?
					(void*) pd_liberar_victima_fifo : (void*) pd_liberar_victima_lru;

	frecuencia_compactacion = config_get_int_value(config, "FRECUENCIA_COMPACTACION");
	particiones = list_create();
	cantidad_busquedas_fallidas = 0;
}

void inicializar_memoria() {

	pthread_mutex_init(&mutex_acceso_memoria, NULL);

	tamanio_total_memoria = config_get_int_value(config, "TAMANO_MEMORIA");
	memoria_principal = malloc(tamanio_total_memoria);
	tamanio_sin_particionar = tamanio_total_memoria;

	algoritmo_memoria = config_get_string_value(config, "ALGORITMO_MEMORIA");
	tamanio_minimo_particion = config_get_int_value(config, "TAMANO_MINIMO_PARTICION");

	if (esquema_de_memoria_particiones_dinamicas()) {
		inicializar_particiones_dinamicas();
	} else {
		inicializar_buddy_system();
	}
}

t_mensaje_cache* guardar_en_memoria(void* msj_recibido, t_id_cola id_cola) {

	t_mensaje_cache* msj_cache = mensaje_cache_crear();

	void* contenido = compactar_contenido_mensaje(msj_recibido, msj_cache->metadata, id_cola);

	int tamanio = calcular_tamanio_minimo_particion(mensaje_cache_get_tamanio_contenido(msj_cache));

	pthread_mutex_lock(&mutex_acceso_memoria);

	t_particion* particion = obtener_particion_libre(tamanio);

	while (!particion_encontrada(particion)) {

		if (debe_ejecutarse_compactacion()) {
			compactar_memoria();
			particion = obtener_particion_libre(tamanio);
		}

		if (!particion_encontrada(particion)) {
			algoritmo_reemplazo();
			particion = obtener_particion_libre(tamanio);
		}
	}

	particion_set_id_cola(particion, id_cola);
	particion_set_uso(particion);
	particion_set_id_mensaje(particion);

	memcpy(particion_get_direccion_contenido(particion), contenido, tamanio);
	free(contenido);
	mensaje_cache_set_particion(msj_cache, particion);
	pthread_mutex_unlock(&mutex_acceso_memoria);

	particion_log_almacenamiento(logger, particion);
	return msj_cache;
}

static t_particion* obtener_particion_libre(int tamanio_contenido) {

	int offset = 0;
	t_particion* particion = algoritmo_particion_libre(tamanio_contenido, &offset);

	if (particion_encontrada(particion)) return particion;

	if (hay_espacio_contiguo_diponible(tamanio_contenido)) {
		return particion_crear_y_ocupar(tamanio_contenido, offset);
	}

	cantidad_busquedas_fallidas += 1;
	return NULL;
}

void* restaurar_mensaje_desde_cache(t_mensaje_cache* msj) {

	t_mensaje_header header = mensaje_header_restaurar_desde_cache(msj);
	void* contenido_cache = obtener_contenido_cache(msj);

	switch (mensaje_cache_get_id_cola(msj)) {
	case NEW_POKEMON:
		return mensaje_new_pokemon_restaurar_desde_cache(contenido_cache, header);
	case APPEARED_POKEMON:
	case CATCH_POKEMON:
		return mensaje_appeared_catch_pokemon_restaurar_desde_cache(contenido_cache, header);
	case CAUGHT_POKEMON:
		return mensaje_caught_pokemon_restaurar_desde_cache(contenido_cache, header);
	case GET_POKEMON:
		return mensaje_get_pokemon_restaurar_desde_cache(contenido_cache, header);
	case LOCALIZED_POKEMON:
		return mensaje_localized_pokemon_restaurar_desde_cache(contenido_cache, header);
	default:
		log_error(event_logger, "No existe la cola: %d. Finalizando hilo", mensaje_cache_get_id_cola(msj));
		pthread_exit(NULL);
		return NULL;
	}
}

void* compactar_contenido_mensaje(void* msj_recibido, t_mensaje_cache_metadata* metadata, t_id_cola id_cola) {

	switch (id_cola) {
	case NEW_POKEMON:
		return mensaje_new_pokemon_formato_cache(msj_recibido, metadata);
	case APPEARED_POKEMON:
	case CATCH_POKEMON:
		return mensaje_appeared_catch_pokemon_formato_cache(msj_recibido, metadata);
	case CAUGHT_POKEMON:
		return mensaje_caught_pokemon_formato_cache(msj_recibido, metadata);
	case GET_POKEMON:
		return mensaje_get_pokemon_formato_cache(msj_recibido, metadata);
	case LOCALIZED_POKEMON:
		return mensaje_localized_pokemon_formato_cache(msj_recibido, metadata);
	default:
		log_error(event_logger, "No existe la cola: %d. Finalizando hilo", id_cola);
		pthread_exit(NULL);
		return NULL;
	}
}

static void* obtener_contenido_cache(t_mensaje_cache* mensaje_cache) {

	int tamanio_contenido = mensaje_cache_get_tamanio_contenido(mensaje_cache);
	void* contenido = malloc(tamanio_contenido);

	memcpy(contenido, particion_get_direccion_contenido(mensaje_cache->particion), tamanio_contenido);

	return contenido;

}

static bool debe_ejecutarse_compactacion() {

	if (!esquema_de_memoria_particiones_dinamicas())
		return false;

	if (frecuencia_compactacion == -1)
		return !hay_particiones_ocupadas();

	return frecuencia_compactacion <= cantidad_busquedas_fallidas;
}
