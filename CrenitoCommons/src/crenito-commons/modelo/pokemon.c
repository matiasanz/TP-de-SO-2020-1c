/*
 * pokemon.c
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */
#include "pokemon.h"

t_pokemon pokemon_crear(char* especie, uint32_t pos_x, uint32_t pos_y) {
	{
		t_pokemon pkm;

		pkm.especie_lenght = strlen(especie) + 1;
		pkm.especie = malloc(pkm.especie_lenght);
		strcpy(pkm.especie, especie);
		pkm.posicion = posicion_crear(pos_x, pos_y);

		return pkm;
	}
}

void pokemon_destruir(t_pokemon pokemon) {
	free(pokemon.especie);
}

void* pokemon_serializar(t_pokemon pkm, int bytes_pokemon) {

	int desplazamiento = 0;
	void * stream = malloc(bytes_pokemon);

	//especie_lenght
	memcpy(stream + desplazamiento, &(pkm.especie_lenght),
			sizeof(pkm.especie_lenght));
	desplazamiento += sizeof(pkm.especie_lenght);

	//especie
	memcpy(stream + desplazamiento, pkm.especie, pkm.especie_lenght);
	desplazamiento += pkm.especie_lenght;

	//posicion
	memcpy(stream + desplazamiento, &(pkm.posicion), sizeof(pkm.posicion));
	desplazamiento += sizeof(pkm.posicion);

	return stream;

}

t_pokemon pokemon_deserializar(void* stream, int* bytes_pokemon) {

	t_pokemon pkm;
	int desplazamiento = 0;

	//especie_lenght
	memcpy(&pkm.especie_lenght, stream + desplazamiento,
			sizeof(pkm.especie_lenght));
	desplazamiento += sizeof(pkm.especie_lenght);

	//especie
	char* especie = strdup(stream + desplazamiento);
	pkm.especie = especie;
	desplazamiento += pkm.especie_lenght;

	//posicion
	memcpy(&pkm.posicion, stream + desplazamiento,
			sizeof(pkm.posicion));
	desplazamiento += sizeof(pkm.posicion);

	*bytes_pokemon = desplazamiento;

	return pkm;

}

void pokemon_log(t_log* un_logger, t_pokemon pokemon){

	log_info(un_logger, "especie: %s", pokemon.especie);
	posicion_log(un_logger, pokemon.posicion);

}
