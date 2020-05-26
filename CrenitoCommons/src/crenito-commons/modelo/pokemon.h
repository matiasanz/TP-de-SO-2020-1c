/*
 * pokemon.h
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MODELO_POKEMON_H_
#define SRC_CRENITO_COMMONS_MODELO_POKEMON_H_

#include "../utils.h"
#include "posicion.h"
#include "pokemon.h"


typedef struct {
	char* especie;
	uint32_t especie_lenght;
	t_posicion posicion;
} t_pokemon;

t_pokemon pokemon_crear(char* especie, uint32_t pos_x, uint32_t pos_y);
void pokemon_destruir(t_pokemon pokemon);
void* pokemon_serializar(t_pokemon pokemon, int bytes_pokemon);
t_pokemon pokemon_deserializar(void* stream, int* bytes_pokemon);

#endif /* SRC_CRENITO_COMMONS_MODELO_POKEMON_H_ */
