/*
 * t_mensaje_metadata.h
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_CACHE_METADATA_H_
#define SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_CACHE_METADATA_H_

#include "../crenito-commons_log.h"
#include "../utils.h"
#include "mensaje_header.h"
#include "../modelo/suscriptor.h"

//Estructura con la información solicitada por el tp en la parte "Administración de Mensajes"
typedef struct {
	uint32_t id_correlativo;
	t_list* suscriptores_enviados;
	t_list* suscriptores_confirmados;
	t_list* suscriptores_fallidos;
	uint32_t tamanio_contenido;
} t_mensaje_cache_metadata;

typedef enum {
	ENVIADO = 1,
	CONFIRMADO = 2,
	FALLIDO = 3,
} t_estado_envio;

t_mensaje_cache_metadata* mensaje_metadata_crear();
void mensaje_metadata_destruir(t_mensaje_cache_metadata* metadata);

void mensaje_cache_metadata_set_id_correlativo(t_mensaje_cache_metadata* metadata, uint32_t id_correlativo);

#endif /* SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_CACHE_METADATA_H_ */
