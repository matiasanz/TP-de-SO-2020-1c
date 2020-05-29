/*
 * mensaje_appeared_pokemon.h
 *
 *  Created on: 24 may. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_APPEARED_CATCH_POKEMON_H_
#define SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_APPEARED_CATCH_POKEMON_H_

#include "../modelo/pokemon.h"
#include "../conexiones/buffer.h"

typedef struct {
	t_mensaje_header mensaje_header;
	t_pokemon pokemon;
} t_mensaje_appeared_catch_pokemon;

t_mensaje_appeared_catch_pokemon* mensaje_appeared_catch_pokemon_crear(char* especie, uint32_t pos_x, uint32_t pos_y);
void mensaje_appeared_catch_pokemon_destruir(t_mensaje_appeared_catch_pokemon* appeared_catch_pokemon);
t_buffer* mensaje_appeared_catch_pokemon_serializar(t_mensaje_appeared_catch_pokemon* appeared_catch_pokemon);
t_mensaje_appeared_catch_pokemon* mensaje_appeared_catch_pokemon_deserializar(void* stream);

//Getters
uint32_t mensaje_appeared_catch_pokemon_get_id(t_mensaje_appeared_catch_pokemon* msj);
uint32_t mensaje_appeared_catch_pokemon_get_id_correlativo(t_mensaje_appeared_catch_pokemon* msj);

//Setters
void mensaje_appeared_catch_pokemon_set_id(t_mensaje_appeared_catch_pokemon* msj, uint32_t id);
void mensaje_appeared_catch_pokemon_set_id_correlativo(t_mensaje_appeared_catch_pokemon* msj, uint32_t id_correlativo);

#endif /* SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_APPEARED_CATCH_POKEMON_H_ */
