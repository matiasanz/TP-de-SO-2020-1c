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
static t_list* obtener_particiones_dump();
static char* get_dump_cache_string();
static void log_debug_dump_cache();
static void destruir_particiones_dummy(t_list* particiones_dump);

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

	memcpy(particion_get_direccion_base_absoluta(particion), contenido, tamanio);
	free(contenido);
	mensaje_cache_set_particion(msj_cache, particion);
	mensaje_cache_set_id_correlativo(msj_cache, id_cola);
	pthread_mutex_unlock(&mutex_acceso_memoria);

	log_almacenamiento_mensaje_en_particion(logger, particion);
	log_debug_dump_cache();

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
		log_error_cola(mensaje_cache_get_id_cola(msj));
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
		log_error_cola(id_cola);
		return NULL;
	}
}

//Dump
void log_dump_cache(int signum) {

	char* string = get_dump_cache_string();
	log_info_and_destroy(dump_logger, string);
}

static void* obtener_contenido_cache(t_mensaje_cache* mensaje_cache) {

	int tamanio_contenido = mensaje_cache_get_tamanio_contenido(mensaje_cache);
	void* contenido = malloc(tamanio_contenido);

	memcpy(contenido, particion_get_direccion_base_absoluta(mensaje_cache->particion), tamanio_contenido);

	return contenido;

}

static bool debe_ejecutarse_compactacion() {

	if (!esquema_de_memoria_particiones_dinamicas())
		return false;

	if (frecuencia_compactacion == -1)
		return !hay_particiones_ocupadas();

	return frecuencia_compactacion <= cantidad_busquedas_fallidas;
}

static t_list* obtener_particiones_dump() {
	return esquema_de_memoria_particiones_dinamicas() ?
			pd_obtener_particiones_dump() : 
			bs_obtner_particiones_dump();
}

static char* get_dump_cache_string() {

	char* string = string_new();
	string_append_with_format(&string, "\n");
	string_append_with_format(&string, SEPARADOR_DUMP_CACHE);
	string_append_timestamp(&string);

	pthread_mutex_lock(&mutex_acceso_memoria);
	t_list* particiones_dump = obtener_particiones_dump();
	for (int i = 0; i < list_size(particiones_dump); ++i) {
		string_append_with_format(&string, particion_to_string_dump(list_get(particiones_dump, i), i + 1));
	}
	pthread_mutex_unlock(&mutex_acceso_memoria);

	string_append_with_format(&string, SEPARADOR_DUMP_CACHE);
	destruir_particiones_dummy(particiones_dump);

	return string;
}

static void log_debug_dump_cache() {

	char* string = get_dump_cache_string();
	log_debug_and_destroy(event_logger, string);
}

static void destruir_particiones_dummy(t_list* particiones_dump) {

	/*TODO:
	 *  En particiones dinámicas solo puede haber 1 dummy. Si pasa lo mismo en BS cambiar este codigo por
	 *  list_remove_and_destroy_by_condition
	 */
	t_list* particiones_dummy = list_filter(particiones_dump, (void*) particion_es_dummy);
	list_iterate(particiones_dummy, (void*) particion_destruir);

	list_destroy(particiones_dump);
}
