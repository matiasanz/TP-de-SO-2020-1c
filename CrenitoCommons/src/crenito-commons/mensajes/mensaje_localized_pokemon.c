/*
 * mensaje_localized_pokemon.c
 *
 *  Created on: 24 may. 2020
 *      Author: utnso
 */
#include "mensaje_localized_pokemon.h"

t_mensaje_localized_pokemon* mensaje_localized_pokemon_crear(char* especie, t_list* posiciones) {

	t_mensaje_localized_pokemon* localized_pokemon = malloc(
			sizeof(t_mensaje_localized_pokemon));

	mensaje_id_inicializar(&localized_pokemon->ids);

	localized_pokemon->especie_lenght = strlen(especie) + 1;
	localized_pokemon->especie = malloc(localized_pokemon->especie_lenght);
	strcpy(localized_pokemon->especie, especie);

	localized_pokemon->posiciones_lenght = list_size(posiciones);

	localized_pokemon->posiciones = list_create();
	list_add_all(localized_pokemon->posiciones, posiciones);

	return localized_pokemon;
}

void mensaje_localized_pokemon_destruir(t_mensaje_localized_pokemon* localized_pokemon) {
	
	free(localized_pokemon->especie);
	list_destroy(localized_pokemon->posiciones);
	free(localized_pokemon);
}

t_buffer* mensaje_localized_pokemon_serializar(t_mensaje_localized_pokemon* localized_pokemon) {

	int size = sizeof(localized_pokemon->ids)
			 + sizeof(localized_pokemon->especie_lenght)
			 + localized_pokemon->especie_lenght
			 + sizeof(localized_pokemon->posiciones_lenght)
			 + sizeof(t_posicion) * localized_pokemon->posiciones_lenght;

	t_buffer* bfr = buffer_crear(size);
	int desplazamiento = 0;

	// id
	memcpy(bfr->stream + desplazamiento, &localized_pokemon->ids,
			sizeof(localized_pokemon->ids));
	desplazamiento += sizeof(localized_pokemon->ids);

	//especie_lenght
	memcpy(bfr->stream + desplazamiento, &localized_pokemon->especie_lenght,
			sizeof(localized_pokemon->especie_lenght));
	desplazamiento += sizeof(localized_pokemon->especie_lenght);

	//especie
	memcpy(bfr->stream + desplazamiento, localized_pokemon->especie,
			localized_pokemon->especie_lenght);
	desplazamiento += localized_pokemon->especie_lenght;

	//posicion_length
	memcpy(bfr->stream + desplazamiento, &localized_pokemon->posiciones_lenght,
			sizeof(localized_pokemon->posiciones_lenght));
	desplazamiento += sizeof(localized_pokemon->posiciones_lenght);

	//posiciones
	int i = 0;
	for (i = 0; i < localized_pokemon->posiciones_lenght; ++i) {
		
		t_posicion* posicion = list_get(localized_pokemon->posiciones, i);
		memcpy(bfr->stream + desplazamiento, &posicion, sizeof(t_posicion));
		desplazamiento += sizeof(t_posicion);
	}

	return bfr;
}

t_mensaje_localized_pokemon* mensaje_localized_pokemon_deserializar(t_buffer* buffer) {

	t_mensaje_localized_pokemon* msj = malloc(sizeof(t_mensaje_localized_pokemon));
	int desplazamiento = 0;

	// id
	memcpy(&msj->ids, buffer->stream + desplazamiento, sizeof(msj->ids));
	desplazamiento += sizeof(msj->ids);

	//especie_lenght
	memcpy(&msj->especie_lenght, buffer->stream + desplazamiento,
			sizeof(msj->especie_lenght));
	desplazamiento += sizeof(msj->especie_lenght);

	//especie
	char* especie = strdup(buffer -> stream + desplazamiento);
	msj -> especie = especie;
	desplazamiento += msj -> especie_lenght;

	//posicion_lenght
	memcpy(&msj->posiciones_lenght, buffer->stream + desplazamiento,
			sizeof(msj->posiciones_lenght));
	desplazamiento += sizeof(msj->posiciones_lenght);

	//posiciones
	msj ->posiciones = list_create();
	t_posicion* posicion;

	int i = 0;
	for (i = 0; i < msj->posiciones_lenght; ++i) {

		memcpy(&posicion, buffer->stream + desplazamiento, sizeof(t_posicion));
		list_add(msj ->posiciones, posicion);
		desplazamiento += sizeof(t_posicion);
	}

	free(posicion);
	buffer_destruir(buffer);

	return msj;
}

// Getters
uint32_t mensaje_localized_pokemon_get_id(t_mensaje_localized_pokemon* msj){
	return msj->ids.id;
}

uint32_t mensaje_localized_pokemon_get_id_correlativo(t_mensaje_localized_pokemon* msj){
	return msj->ids.id_correlativo;
}

//Setters
void mensaje_localized_pokemon_set_id(t_mensaje_localized_pokemon* msj, uint32_t id){
	msj->ids.id = id;
}

void mensaje_localized_pokemon_set_id_correlativo(t_mensaje_localized_pokemon* msj, uint32_t id_correlativo){
	msj->ids.id_correlativo = id_correlativo;
}
