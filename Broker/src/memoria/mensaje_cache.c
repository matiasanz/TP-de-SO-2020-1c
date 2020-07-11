/*
 * memoria_cache.c
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#include "mensaje_cache.h"

static bool debe_asignar_id_correlativo_por_primera_vez(t_mensaje_cache* msj, t_id_cola id_cola);

t_mensaje_cache* mensaje_cache_crear() {

	t_mensaje_cache* mensaje_cache = malloc(sizeof(t_mensaje_cache));

	mensaje_cache->particion = NULL;
	pthread_mutex_init(&mensaje_cache->mutex_edicion_mensaje, NULL);
	mensaje_cache->metadata = mensaje_metadata_crear();

	return mensaje_cache;
}

void mensaje_cache_eliminar_de_cola(t_mensaje_cache* msj) {

	particion_liberar(msj->particion);
	pthread_mutex_lock(&msj->mutex_edicion_mensaje);
	mensaje_metadata_destruir(msj->metadata);
	pthread_mutex_unlock(&msj->mutex_edicion_mensaje);
	free(msj);
}

bool todos_los_suscriptores_confirmaron(t_mensaje_cache* msj, int suscriptores_cola) {

	pthread_mutex_lock(&msj->mutex_edicion_mensaje);
	int suscriptores_confirmados = list_size(mensaje_cache_get_suscriptores(msj, CONFIRMADO));
	pthread_mutex_unlock(&msj->mutex_edicion_mensaje);

	return suscriptores_confirmados == suscriptores_cola;
}

uint32_t mensaje_cache_get_id(t_mensaje_cache* msj) {
	return particion_get_id_mensaje(msj->particion);
}

uint32_t mensaje_cache_get_id_correlativo(t_mensaje_cache* msj) {
	return msj->metadata->id_correlativo;
}

t_id_cola mensaje_cache_get_id_cola(t_mensaje_cache* msj) {
	return particion_get_id_cola(msj->particion);
}

uint32_t mensaje_cache_get_tamanio_contenido(t_mensaje_cache* msj) {
	return msj->metadata->tamanio_contenido;
}

void mensaje_cache_set_particion(t_mensaje_cache* msj, t_particion* particion) {
	msj->particion = particion;
}

void mensaje_cache_set_id_correlativo(t_mensaje_cache* msj, t_id_cola id_cola) {

	if (debe_asignar_id_correlativo_por_primera_vez(msj, id_cola)) {
		mensaje_cache_metadata_set_id_correlativo(msj->metadata, mensaje_cache_get_id(msj));
	}
}

void mensaje_cache_agregar_suscriptor(t_mensaje_cache* msj, t_suscriptor* suscriptor, t_estado_envio estado_envio) {

	pthread_mutex_lock(&msj->mutex_edicion_mensaje);
	t_suscriptor* copy = suscriptor_duplicar(suscriptor);
	list_add(mensaje_cache_get_suscriptores(msj, estado_envio), copy);
	pthread_mutex_unlock(&msj->mutex_edicion_mensaje);
}

t_list* mensaje_cache_get_suscriptores(t_mensaje_cache* msj, t_estado_envio estado_envio) {

	switch (estado_envio) {
	case ENVIADO:
		return msj->metadata->suscriptores_enviados;
	case CONFIRMADO:
		return msj->metadata->suscriptores_confirmados;
	case FALLIDO:
		return msj->metadata->suscriptores_fallidos;
	default:
		log_error_get_suscriptores(mensaje_cache_get_id(msj));
		return NULL;
	}
}

t_mensaje_header mensaje_header_restaurar_desde_cache(t_mensaje_cache* msj) {

	t_mensaje_header header;
	header.id = mensaje_cache_get_id(msj);
	header.id_correlativo = msj->metadata->id_correlativo;

	return header;
}

static bool debe_asignar_id_correlativo_por_primera_vez(t_mensaje_cache* msj, t_id_cola id_cola) {

	if (mensaje_cache_get_id_correlativo(msj) != 0)	return false;
	return id_cola == CATCH_POKEMON || id_cola == GET_POKEMON || id_cola == NEW_POKEMON;
}
