/*
 * mensaje_appeared_pokemon.h
 *
 *  Created on: 24 may. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_APPEARED_POKEMON_H_
#define SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_APPEARED_POKEMON_H_

#include "../modelo/pokemon.h"
#include "../conexiones/buffer.h"

typedef struct {
	t_mensaje_id ids;
	t_pokemon pokemon;
} t_mensaje_appeared_pokemon;

t_mensaje_appeared_pokemon* mensaje_appeared_pokemon_crear(char* especie, uint32_t pos_x, uint32_t pos_y);
void mensaje_appeared_pokemon_destruir(t_mensaje_appeared_pokemon* appeared_pokemon);
t_buffer* mensaje_appeared_pokemon_serializar(t_mensaje_appeared_pokemon* appeared_pokemon);
t_mensaje_appeared_pokemon* mensaje_appeared_pokemon_deserializar(t_buffer* buffer);


#endif /* SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_APPEARED_POKEMON_H_ */
