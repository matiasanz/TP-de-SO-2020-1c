/*
 * memoria_cache.c
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#include "mensaje_cache.h"

static void mensaje_cache_agregar_suscriptor_enviado(t_mensaje_cache* msj, t_suscriptor* suscriptor, uint32_t id_mensaje);
static void mensaje_cache_agregar_suscriptor_confirmado(t_mensaje_cache* msj, t_suscriptor* suscriptor, uint32_t id_mensaje);
static void mensaje_cache_agregar_suscriptor_fallido(t_mensaje_cache* msj, t_suscriptor* suscriptor, uint32_t id_mensaje);

t_mensaje_cache* mensaje_cache_crear(t_mensaje_header msj_header) {

	t_mensaje_cache* mensaje_cache = malloc(sizeof(t_mensaje_cache));

	mensaje_cache->particion = NULL;

	pthread_mutex_init(&mensaje_cache->mutex_suscriptores_enviados, NULL);
	pthread_mutex_init(&mensaje_cache->mutex_suscriptores_confirmados, NULL);
	pthread_mutex_init(&mensaje_cache->mutex_suscriptores_fallidos, NULL);

	mensaje_cache->metadata = mensaje_metadata_crear(msj_header.id_correlativo);

	return mensaje_cache;
}

void mensaje_cache_eliminar_de_cola_y_liberar_particion(t_mensaje_cache* msj) {

	particion_liberar(msj->particion);
	mensaje_metadata_destruir(msj->metadata);

	free(msj);
}

bool todos_los_suscriptores_confirmaron(t_mensaje_cache* msj, int suscriptores_cola) {

	pthread_mutex_lock(&msj->mutex_suscriptores_confirmados);
	int suscriptores_confirmados = list_size(msj ->metadata->suscriptores_confirmados);
	pthread_mutex_unlock(&msj->mutex_suscriptores_confirmados);

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

static void mensaje_cache_agregar_suscriptor_enviado(t_mensaje_cache* msj, t_suscriptor* suscriptor, uint32_t id_mensaje) {

	pthread_mutex_lock(&msj->mutex_suscriptores_enviados);
	list_add(msj->metadata->suscriptores_enviados, suscriptor_duplicar(suscriptor));
	pthread_mutex_unlock(&msj->mutex_suscriptores_enviados);

	log_envio_mensaje_a_suscriptor(id_mensaje, suscriptor_get_id_proceso(suscriptor));
}

static void mensaje_cache_agregar_suscriptor_confirmado(t_mensaje_cache* msj, t_suscriptor* suscriptor, uint32_t id_mensaje) {

	pthread_mutex_lock(&msj->mutex_suscriptores_confirmados);
	list_add(msj->metadata->suscriptores_confirmados, suscriptor_duplicar(suscriptor));
	pthread_mutex_unlock(&msj->mutex_suscriptores_confirmados);

	log_confirmacion_mensaje_suscriptor(id_mensaje, suscriptor_get_id_proceso(suscriptor));
}

static void mensaje_cache_agregar_suscriptor_fallido(t_mensaje_cache* msj, t_suscriptor* suscriptor, uint32_t id_mensaje) {

	pthread_mutex_lock(&msj->mutex_suscriptores_fallidos);
	list_add(msj->metadata->suscriptores_fallidos, suscriptor_duplicar(suscriptor));
	pthread_mutex_unlock(&msj->mutex_suscriptores_fallidos);

	log_warning_envio_mensaje(id_mensaje, suscriptor_get_id_proceso(suscriptor));
}

t_mensaje_header mensaje_header_restaurar_desde_cache(t_mensaje_cache* msj) {

	t_mensaje_header header;
	header.id = mensaje_cache_get_id(msj);
	header.id_correlativo = msj->metadata->id_correlativo;

	return header;
}

int mensaje_cache_get_cantidad_suscriptores_fallidos(t_mensaje_cache* msj) {

	pthread_mutex_lock(&msj->mutex_suscriptores_fallidos);
	int cantidad_suscriptores_fallidos = list_size(msj->metadata->suscriptores_fallidos);
	pthread_mutex_unlock(&msj->mutex_suscriptores_fallidos);

	return cantidad_suscriptores_fallidos;
}

void mensaje_cache_set_ack(t_mensaje_cache* msj, t_suscriptor* suscriptor, uint32_t ack, uint32_t id_mensaje) {

	mensaje_cache_agregar_suscriptor_enviado(msj, suscriptor, id_mensaje);

	if (conexion_exitosa(ack)) {
		mensaje_cache_agregar_suscriptor_confirmado(msj, suscriptor, id_mensaje);
	} else {
		mensaje_cache_agregar_suscriptor_fallido(msj, suscriptor, id_mensaje);
	}
}

t_suscriptor* mensaje_cache_get_primer_sucriptor_fallido(t_mensaje_cache* msj) {

	t_suscriptor* suscriptor;

	pthread_mutex_lock(&msj->mutex_suscriptores_fallidos);
    suscriptor = list_remove(msj ->metadata ->suscriptores_fallidos, 0);
	pthread_mutex_unlock(&msj->mutex_suscriptores_fallidos);

	return suscriptor;
}

bool mensaje_cache_pendiente_confirmacion(t_mensaje_cache* msj, t_suscriptor* suscriptor_buscado) {

	bool buscar_por_id_proceso(t_suscriptor* suscriptor_confirmado) {
		return suscriptor_get_id_proceso(suscriptor_confirmado) == suscriptor_get_id_proceso(suscriptor_buscado);
	}

	t_suscriptor* suscriptor_encontrado;

	pthread_mutex_lock(&msj->mutex_suscriptores_confirmados);
	suscriptor_encontrado = list_find(msj->metadata->suscriptores_confirmados, (void*)buscar_por_id_proceso);
	pthread_mutex_unlock(&msj->mutex_suscriptores_confirmados);

	return NULL == suscriptor_encontrado;
}
