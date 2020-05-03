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
