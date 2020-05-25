/*
 * mensaje_caught_pokemon.c
 *
 *  Created on: 25 may. 2020
 *      Author: utnso
 */
#include "mensaje_caught_pokemon.h"

t_mensaje_caught_pokemon* mensaje_caught_pokemon_crear(uint32_t atrapado) {

	t_mensaje_caught_pokemon* caught_pokemon = malloc(sizeof(t_mensaje_caught_pokemon));

	mensaje_id_inicializar(&caught_pokemon->ids);
	caught_pokemon->atrapado = atrapado;

	return caught_pokemon;
}

void mensaje_caught_pokemon_destruir(t_mensaje_caught_pokemon* caught_pokemon) {

	free(caught_pokemon);
}

t_buffer* mensaje_caught_pokemon_serializar(t_mensaje_caught_pokemon* caught_pokemon) {

	int size = sizeof(caught_pokemon->ids) 
	         + sizeof(caught_pokemon->atrapado);

	t_buffer* bfr = buffer_crear(size);
	int desplazamiento = 0;

	// id
	memcpy(bfr->stream + desplazamiento, &(caught_pokemon->ids),
			sizeof(caught_pokemon->ids));
	desplazamiento += sizeof(caught_pokemon->ids);

	//atrapado
	memcpy(bfr->stream + desplazamiento, &(caught_pokemon->atrapado),
			sizeof(caught_pokemon->atrapado));
	desplazamiento += sizeof(caught_pokemon->atrapado);

	return bfr;
}

t_mensaje_caught_pokemon* mensaje_caught_pokemon_deserializar(t_buffer* buffer) {

	t_mensaje_caught_pokemon* msj = malloc(sizeof(t_mensaje_caught_pokemon));
	int desplazamiento = 0;

	// id
	memcpy(&msj->ids, buffer->stream + desplazamiento, sizeof(msj->ids));
	desplazamiento += sizeof(msj->ids);

	//atrapado
	memcpy(&msj->atrapado, buffer->stream + desplazamiento,
			sizeof(msj->atrapado));
	desplazamiento += sizeof(msj->atrapado);

	buffer_destruir(buffer);

	return msj;
}
