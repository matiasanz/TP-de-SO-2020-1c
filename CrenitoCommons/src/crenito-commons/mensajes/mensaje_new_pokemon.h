/*
 * mensaje_new_pokemon.h
 *
 *  Created on: 1 may. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_NEW_POKEMON_H_
#define SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_NEW_POKEMON_H_

#include <stdint.h>
#include "../conexiones/paquete.h"

typedef struct {
	uint32_t x;
	uint32_t y;
} t_posicion;

typedef struct {
	uint32_t id_correlativo;
	char* especie;
	uint32_t especie_lenght;
	t_posicion* posicion;
	uint32_t cantidad;
} t_mensaje_new_pokemon;

t_mensaje_new_pokemon* mensaje_new_pokemon_crear();
void mensaje_new_pokemon_destruir(t_mensaje_new_pokemon* new_pokemon);
t_buffer* mensaje_new_pokemon_serializar(t_mensaje_new_pokemon* new_pokemon);
t_mensaje_new_pokemon* mensaje_new_pokemon_deserializar(t_buffer* buffer);

#endif /* SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_NEW_POKEMON_H_ */
