/*
 * memoria_cache.c
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#include "mensaje_cache.h"

t_mensaje_cache* mensaje_cache_crear() {

	t_mensaje_cache* mensaje_cache = malloc(sizeof(t_mensaje_cache));

	mensaje_cache->particion = NULL;
	mensaje_cache->metadata = mensaje_metadata_crear();

	return mensaje_cache;
}

void mensaje_cache_destruir(t_mensaje_cache* msj) {

	particion_log_eliminacion(logger, msj->particion);
	particion_liberar(msj->particion);
	mensaje_metadata_destruir(msj->metadata);
	free(msj);
}

bool equals_from_id(t_mensaje_cache* msj, uint32_t id_buscado) {
	return particion_get_id_mensaje(msj->particion) == id_buscado;
}

uint32_t mensaje_cache_get_id(t_mensaje_cache* msj) {
	return msj->particion->id_mensaje;
}

t_id_cola mensaje_cache_get_id_cola(t_mensaje_cache* msj) {
	return msj->particion->id_cola;
}

uint32_t mensaje_cache_get_tamanio_contenido(t_mensaje_cache* msj) {
	return msj->metadata->tamanio_contenido;
}

void mensaje_cache_set_particion(t_mensaje_cache* msj, t_particion* particion) {
	msj->particion = particion;
}

t_mensaje_header mensaje_header_restaurar_desde_cache(t_mensaje_cache* msj) {

	t_mensaje_header header;
	header.id = mensaje_cache_get_id(msj);
	header.id_correlativo = msj->metadata->id_correlativo;

	return header;
}
