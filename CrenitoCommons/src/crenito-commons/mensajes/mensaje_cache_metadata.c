/*
 * mensaje_metadata.c
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#include "mensaje_cache_metadata.h"

t_mensaje_cache_metadata* mensaje_metadata_crear() {

	t_mensaje_cache_metadata* metadata = malloc(sizeof(t_mensaje_cache_metadata));

	metadata->id_correlativo = 0;
	metadata->suscriptores_confirmados = list_create();
	metadata->suscriptores_enviados = list_create();
	metadata->tamanio_contenido = 0;

	return metadata;
}

void mensaje_metadata_destruir(t_mensaje_cache_metadata* msj) {
	free(msj);
}
