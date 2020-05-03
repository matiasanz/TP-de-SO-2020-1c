/*
 * mensaje_new_pokemon.c
 *
 *  Created on: 1 may. 2020
 *      Author: utnso
 */
#include "mensaje_new_pokemon.h"

t_mensaje_new_pokemon* mensaje_new_pokemon_crear(char* especie, uint32_t pos_x,
		uint32_t pos_y, uint32_t cantidad) {

	t_mensaje_new_pokemon* new_pokemon = malloc(sizeof(t_mensaje_new_pokemon));

	new_pokemon->pokemon = pokemon_crear(especie, pos_x, pos_y);
	new_pokemon->cantidad = cantidad;

	return new_pokemon;
}

void mensaje_new_pokemon_destruir(t_mensaje_new_pokemon* new_pokemon) {

	pokemon_destruir(new_pokemon->pokemon);
	free(new_pokemon);
}

t_buffer* mensaje_new_pokemon_serializar(t_mensaje_new_pokemon* new_pokemon) {

	int size = sizeof(new_pokemon->ids) +
			   new_pokemon-> pokemon.especie_lenght +
			   sizeof(new_pokemon->pokemon.especie_lenght) +
			   sizeof(new_pokemon->cantidad) +
			   sizeof(new_pokemon->pokemon.posicion);

	t_buffer* bfr = buffer_crear(size);
	int desplazamiento = 0;

	// id
	memcpy(bfr->stream + desplazamiento, &(new_pokemon->ids),
			sizeof(new_pokemon->ids));
	desplazamiento += sizeof(new_pokemon->ids);

	t_pokemon pkm = new_pokemon->pokemon;
	//especie_lenght
	memcpy(bfr->stream + desplazamiento, &(pkm.especie_lenght),
			sizeof(pkm.especie_lenght));
	desplazamiento += sizeof(pkm.especie_lenght);

	//especie
	memcpy(bfr->stream + desplazamiento, pkm.especie, pkm.especie_lenght);
	desplazamiento += pkm.especie_lenght;

	//posicion
	memcpy(bfr->stream + desplazamiento, &(pkm.posicion), sizeof(pkm.posicion));
	desplazamiento += sizeof(pkm.posicion);

	//cantidad
	memcpy(bfr->stream + desplazamiento, &(new_pokemon->cantidad),
			sizeof(new_pokemon->cantidad));
	desplazamiento += sizeof(new_pokemon->cantidad);

	return bfr;
}

t_mensaje_new_pokemon* mensaje_new_pokemon_deserializar(t_buffer* buffer) {

	t_mensaje_new_pokemon* msj = malloc(sizeof(t_mensaje_new_pokemon));
	int desplazamiento = 0;

	// id
	memcpy(&msj->ids, buffer->stream + desplazamiento, sizeof(msj->ids));
	desplazamiento += sizeof(msj->ids);

	t_pokemon pkm = msj->pokemon;
	//especie_lenght
	memcpy(&pkm.especie_lenght, buffer->stream + desplazamiento,
			sizeof(pkm.especie_lenght));
	desplazamiento += sizeof(pkm.especie_lenght);

	//especie
	char* especie = strdup(buffer->stream + desplazamiento);
	pkm.especie = especie;
	desplazamiento += pkm.especie_lenght;

	//posicion
	memcpy(&pkm.posicion, buffer->stream + desplazamiento,
			sizeof(pkm.posicion));
	desplazamiento += sizeof(pkm.posicion);

	//cantidad
	memcpy(&msj->cantidad, buffer->stream + desplazamiento,
			sizeof(msj->cantidad));
	desplazamiento += sizeof(msj->cantidad);

	msj->pokemon = pkm;
	
	buffer_destruir(buffer);
	
	return msj;
}
