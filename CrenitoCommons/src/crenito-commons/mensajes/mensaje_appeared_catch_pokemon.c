/*
 * mensaje_appeared_pokemon.c
 *
 *  Created on: 24 may. 2020
 *      Author: utnso
 */

#include "mensaje_appeared_catch_pokemon.h"

t_mensaje_appeared_catch_pokemon* mensaje_appeared_catch_pokemon_crear(char* especie, uint32_t pos_x, uint32_t pos_y) {

	t_mensaje_appeared_catch_pokemon* appeared_catch_pokemon = malloc(
			sizeof(t_mensaje_appeared_catch_pokemon));

	mensaje_id_inicializar(&appeared_catch_pokemon->ids);
	appeared_catch_pokemon->pokemon = pokemon_crear(especie, pos_x, pos_y);

	return appeared_catch_pokemon;
}

void mensaje_appeared_catch_pokemon_destruir(t_mensaje_appeared_catch_pokemon* appeared_pokemon) {

	pokemon_destruir(appeared_pokemon->pokemon);
	free(appeared_pokemon);
}

t_buffer* mensaje_appeared_catch_pokemon_serializar(t_mensaje_appeared_catch_pokemon* appeared_pokemon) {

	int bytes_pokemon = appeared_pokemon->pokemon.especie_lenght
			+ sizeof(appeared_pokemon->pokemon.especie_lenght)
			+ sizeof(appeared_pokemon->pokemon.posicion);

	int size = sizeof(appeared_pokemon->ids) 
	         + bytes_pokemon;

	t_buffer* bfr = buffer_crear(size);
	int desplazamiento = 0;

	// id
	memcpy(bfr->stream + desplazamiento, &(appeared_pokemon->ids),
			sizeof(appeared_pokemon->ids));
	desplazamiento += sizeof(appeared_pokemon->ids);

	// pokemon
	void* pkm_serializado = pokemon_serializar(appeared_pokemon->pokemon,
			bytes_pokemon);
	memcpy(bfr->stream + desplazamiento, pkm_serializado, bytes_pokemon);
	free(pkm_serializado);
	desplazamiento += bytes_pokemon;

	return bfr;
}

t_mensaje_appeared_catch_pokemon* mensaje_appeared_catch_pokemon_deserializar(t_buffer* buffer) {

	t_mensaje_appeared_catch_pokemon* msj = malloc(sizeof(t_mensaje_appeared_catch_pokemon));
	int desplazamiento = 0;

	// id
	memcpy(&msj->ids, buffer->stream + desplazamiento, sizeof(msj->ids));
	desplazamiento += sizeof(msj->ids);

	//pokemon
	int bytes_pokemon = 0;
	msj->pokemon = pokemon_deserializar(buffer->stream + desplazamiento,
			&bytes_pokemon);
	desplazamiento += bytes_pokemon;

	buffer_destruir(buffer);

	return msj;
}
