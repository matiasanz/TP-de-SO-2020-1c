/*
 * mensaje_get_pokemon.h
 *
 *  Created on: 24 may. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_GET_POKEMON_H_
#define SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_GET_POKEMON_H_

#include "../conexiones/buffer.h"

typedef struct {
	t_mensaje_id ids;
	char* especie;
	uint32_t especie_lenght;
} t_mensaje_get_pokemon;


t_mensaje_get_pokemon* mensaje_get_pokemon_crear(char* especie);
void mensaje_get_pokemon_destruir(t_mensaje_get_pokemon* get_pokemon);
t_buffer* mensaje_get_pokemon_serializar(t_mensaje_get_pokemon* get_pokemon);
t_mensaje_get_pokemon* mensaje_get_pokemon_deserializar(t_buffer* buffer);

#endif /* SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_GET_POKEMON_H_ */
