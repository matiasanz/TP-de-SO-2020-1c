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

	mensaje_id_inicializar(&new_pokemon->ids);
	new_pokemon->pokemon = pokemon_crear(especie, pos_x, pos_y);
	new_pokemon->cantidad = cantidad;

	return new_pokemon;
}

void mensaje_new_pokemon_destruir(t_mensaje_new_pokemon* new_pokemon) {

	pokemon_destruir(new_pokemon->pokemon);
	free(new_pokemon);
}

t_buffer* mensaje_new_pokemon_serializar(t_mensaje_new_pokemon* new_pokemon) {

	int bytes_pokemon = new_pokemon-> pokemon.especie_lenght +
			            sizeof(new_pokemon->pokemon.especie_lenght) +
						sizeof(new_pokemon->pokemon.posicion);

	int size = sizeof(new_pokemon->ids) +
			   bytes_pokemon +
			   sizeof(new_pokemon->cantidad);


	t_buffer* bfr = buffer_crear(size);
	int desplazamiento = 0;

	// id
	memcpy(bfr->stream + desplazamiento, &(new_pokemon->ids),
			sizeof(new_pokemon->ids));
	desplazamiento += sizeof(new_pokemon->ids);

	// pokemon
	void* pkm_serializado = pokemon_serializar(new_pokemon->pokemon, bytes_pokemon);
	memcpy(bfr->stream + desplazamiento, pkm_serializado, bytes_pokemon);
	free(pkm_serializado);
	desplazamiento += bytes_pokemon;

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

	//pokemon
	int bytes_pokemon = 0;
	msj->pokemon = pokemon_deserializar(buffer -> stream + desplazamiento, &bytes_pokemon);
	desplazamiento += bytes_pokemon;

	//cantidad
	memcpy(&msj->cantidad, buffer->stream + desplazamiento,
			sizeof(msj->cantidad));
	desplazamiento += sizeof(msj->cantidad);
	
	buffer_destruir(buffer);
	
	return msj;
}

// Getters
uint32_t mensaje_new_pokemon_get_id(t_mensaje_new_pokemon* msj){
	return msj->ids.id;
}

uint32_t mensaje_new_pokemon_get_id_correlativo(t_mensaje_new_pokemon* msj){
	return msj->ids.id_correlativo;
}

//Setters
void mensaje_new_pokemon_set_id(t_mensaje_new_pokemon* msj, uint32_t id){
	msj->ids.id = id;
}

void mensaje_new_pokemon_set_id_correlativo(t_mensaje_new_pokemon* msj, uint32_t id_correlativo){
	msj->ids.id_correlativo = id_correlativo;
}
