/*
 * t_mensaje_metadata.h
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_CACHE_METADATA_H_
#define SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_CACHE_METADATA_H_

#include "../utils.h"
#include "mensaje_header.h"
//Estructura con la información solicitada por el tp en la parte "Administración de Mensajes"
typedef struct {
	uint32_t id_correlativo;
	t_list* suscriptores_enviados;
	t_list* suscriptores_confirmados;
	uint32_t tamanio_contenido;
} t_mensaje_cache_metadata;

t_mensaje_cache_metadata* mensaje_metadata_crear();
void mensaje_metadata_destruir(t_mensaje_cache_metadata* msj);

#endif /* SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_CACHE_METADATA_H_ */
