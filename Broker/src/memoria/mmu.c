/*
 * memoria.c
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#include "mmu.h"

//Funciones Privadas
static t_particion* asignar_particion(int tamanio_contenido, t_id_cola id_cola, uint32_t id_mensaje);
static void escribir_memoria_principal(void* contenido, t_particion* particion);
static void* leer_memoria_principal(t_particion* particion);

void inicializar_memoria() {

	pthread_mutex_init(&mutex_acceso_memoria, NULL);
	pthread_mutex_init(&mutex_escritura_eliminacion_memoria, NULL);

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

t_mensaje_cache* guardar_en_memoria(void* msj_recibido, t_id_cola id_cola, t_mensaje_header msj_header) {

	t_mensaje_cache* msj_cache = mensaje_cache_crear();
	void* contenido_mensaje = compactar_contenido_mensaje(msj_recibido, msj_cache->metadata, id_cola);

	pthread_mutex_lock(&mutex_acceso_memoria);

	t_particion* particion = asignar_particion(mensaje_cache_get_tamanio_contenido(msj_cache), id_cola, msj_header.id);
	escribir_memoria_principal(contenido_mensaje, particion);

	mensaje_cache_set_particion(msj_cache, particion);
	log_almacenamiento_mensaje_en_memoria(particion);

	pthread_mutex_unlock(&mutex_acceso_memoria);

	log_debug_dump_cache();
	free(contenido_mensaje);

	return msj_cache;

}

void* restaurar_mensaje_desde_cache(t_mensaje_cache* msj, t_mensaje_header* header) {

	*header = mensaje_header_restaurar_desde_cache(msj);
	void* contenido_cache = leer_memoria_principal(msj -> particion);

	switch (particion_get_id_cola(msj -> particion)) {
	case NEW_POKEMON:
		return mensaje_new_pokemon_restaurar_desde_cache(contenido_cache, *header);
	case APPEARED_POKEMON:
	case CATCH_POKEMON:
		return mensaje_appeared_catch_pokemon_restaurar_desde_cache(contenido_cache, *header);
	case CAUGHT_POKEMON:
		return mensaje_caught_pokemon_restaurar_desde_cache(contenido_cache, *header);
	case GET_POKEMON:
		return mensaje_get_pokemon_restaurar_desde_cache(contenido_cache, *header);
	case LOCALIZED_POKEMON:
		return mensaje_localized_pokemon_restaurar_desde_cache(contenido_cache, *header);
	default:
		log_warning_cola(particion_get_id_cola(msj -> particion), "restaurar_mensaje_desde_cache");
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
		log_warning_cola(id_cola, "compactar_contenido_mensaje");
		return NULL;
	}
}

static t_particion* asignar_particion(int tamanio_contenido, t_id_cola id_cola, uint32_t id_mensaje) {

	cantidad_busquedas_fallidas = 0;
	int tamanio_particion = calcular_tamanio_particion(tamanio_contenido);
	t_particion* particion = obtener_particion_libre(tamanio_particion);

	while (!particion_encontrada(particion)) {

		if (debe_ejecutarse_compactacion()) {

			pthread_mutex_lock(&mutex_escritura_eliminacion_memoria);
			compactar_memoria();
			pthread_mutex_unlock(&mutex_escritura_eliminacion_memoria);

			particion = obtener_particion_libre(tamanio_particion);
		}

		if (!particion_encontrada(particion)) {
			algoritmo_reemplazo();
			particion = obtener_particion_libre(tamanio_particion);
		}
	}

	//Ver este if
	if (!esquema_de_memoria_particiones_dinamicas()) {
		memoria_actualizar_tamanio_disponible_sin_particionar(tamanio_particion);
	}
	particion_set_id_cola(particion, id_cola);
	particion_set_uso(particion);

	pthread_mutex_lock(&mutex_escritura_eliminacion_memoria);
	particion_set_id_mensaje(particion, id_mensaje);
	pthread_mutex_unlock(&mutex_escritura_eliminacion_memoria);

	return particion;
}

static void escribir_memoria_principal(void* contenido, t_particion* particion) {
	memcpy(particion_get_direccion_base_absoluta(particion), contenido, particion_get_tamanio(particion));
}

static void* leer_memoria_principal(t_particion* particion) {

	int tamanio_contenido = particion_get_tamanio(particion);
	void* contenido = malloc(tamanio_contenido);

	memcpy(contenido, particion_get_direccion_base_absoluta(particion), tamanio_contenido);
	particion_actualizar_fecha_ultimo_acceso(particion);

	return contenido;

}


