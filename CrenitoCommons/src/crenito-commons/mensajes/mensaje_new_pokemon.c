/*
 * mensaje_new_pokemon.c
 *
 *  Created on: 1 may. 2020
 *      Author: utnso
 */
#include "mensaje_new_pokemon.h"

t_mensaje_new_pokemon* mensaje_new_pokemon_crear() {

	t_mensaje_new_pokemon* new_pokemon = malloc(sizeof(t_mensaje_new_pokemon));
	new_pokemon->posicion = malloc(sizeof(t_posicion));
	new_pokemon->id_correlativo = 0;

	return new_pokemon;
}

void mensaje_new_pokemon_destruir(t_mensaje_new_pokemon* new_pokemon) {

	free(new_pokemon->posicion);
	free(new_pokemon->especie);
	free(new_pokemon);
}

t_buffer* mensaje_new_pokemon_serializar(t_mensaje_new_pokemon* new_pokemon) {

	int size = sizeof(new_pokemon->id_correlativo) +
			   new_pokemon->especie_lenght +
			   sizeof(new_pokemon->especie_lenght) +
			   sizeof(new_pokemon->cantidad) +
			   sizeof(new_pokemon->posicion);

	t_buffer* bfr = buffer_crear(size);

	int desplazamiento = 0;

	//especie_lenght
	memcpy(bfr->stream + desplazamiento, &(new_pokemon->id_correlativo),
			sizeof(new_pokemon->id_correlativo));
	desplazamiento += sizeof(new_pokemon->id_correlativo);

	//especie_lenght
	memcpy(bfr->stream + desplazamiento, &(new_pokemon->especie_lenght),
			sizeof(new_pokemon->especie_lenght));
	desplazamiento += sizeof(new_pokemon->especie_lenght);

	//especie
	memcpy(bfr->stream + desplazamiento, new_pokemon->especie,
			new_pokemon->especie_lenght);
	desplazamiento += new_pokemon->especie_lenght;

	//posicion
	memcpy(bfr->stream + desplazamiento, &(new_pokemon->posicion),
			sizeof(new_pokemon->posicion));
	desplazamiento += sizeof(new_pokemon->posicion);

	//cantidad
	memcpy(bfr->stream + desplazamiento, &(new_pokemon->cantidad),
			sizeof(new_pokemon->cantidad));
	desplazamiento += sizeof(new_pokemon->cantidad);

	return bfr;
}

t_mensaje_new_pokemon* mensaje_new_pokemon_deserializar(t_buffer* buffer) {

	t_mensaje_new_pokemon* new_pokemon = mensaje_new_pokemon_crear();

	int desplazamiento = 0;

	// id_correlativo
	memcpy(&new_pokemon->id_correlativo, buffer->stream + desplazamiento,
			sizeof(new_pokemon->id_correlativo));
	desplazamiento += sizeof(new_pokemon->id_correlativo);

	//especie_lenght
	memcpy(&new_pokemon->especie_lenght, buffer->stream + desplazamiento,
			sizeof(new_pokemon->especie_lenght));
	desplazamiento += sizeof(new_pokemon->especie_lenght);

	//especie
	char* especie = strdup(buffer->stream + desplazamiento);
	new_pokemon->especie = especie;
	desplazamiento += new_pokemon->especie_lenght;

	//posicion
	memcpy(&new_pokemon->posicion, buffer->stream + desplazamiento,
			sizeof(new_pokemon->posicion));
	desplazamiento += sizeof(new_pokemon->posicion);

	//cantidad
	memcpy(&new_pokemon->cantidad, buffer->stream + desplazamiento,
			sizeof(new_pokemon->cantidad));
	desplazamiento += sizeof(new_pokemon->cantidad);

	buffer_destruir(buffer);
	return new_pokemon;
}
