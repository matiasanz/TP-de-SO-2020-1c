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
#include "mensaje_cache_metadata.h"
#include "mensaje_header.h"

typedef struct {
	t_mensaje_header mensaje_header;
	t_pokemon pokemon;
} t_mensaje_appeared_catch_pokemon;

// Creación y destrucción
t_mensaje_appeared_catch_pokemon* mensaje_appeared_catch_pokemon_crear(char* especie, uint32_t pos_x, uint32_t pos_y);
void mensaje_appeared_catch_pokemon_destruir(t_mensaje_appeared_catch_pokemon* appeared_catch_pokemon);

// Serializacion
t_buffer* mensaje_appeared_catch_pokemon_serializar(t_mensaje_appeared_catch_pokemon* appeared_catch_pokemon);
t_mensaje_appeared_catch_pokemon* mensaje_appeared_catch_pokemon_deserializar(void* stream);

// Cache
void* mensaje_appeared_catch_pokemon_formato_cache(void* stream, t_mensaje_cache_metadata* metadata);
t_mensaje_appeared_catch_pokemon* mensaje_appeared_catch_pokemon_restaurar_desde_cache(void* cache, t_mensaje_header header);

// Log
void mensaje_appeared_catch_pokemon_log(t_log* un_logger, t_mensaje_appeared_catch_pokemon* appeared_catch, char* tipo);
char* mensaje_appeared_catch_pokemon_to_string(t_mensaje_appeared_catch_pokemon* appeared_catch, char* tipo);

// Getters
uint32_t mensaje_appeared_catch_pokemon_get_id(t_mensaje_appeared_catch_pokemon* msj);
uint32_t mensaje_appeared_catch_pokemon_get_id_correlativo(t_mensaje_appeared_catch_pokemon* msj);
t_mensaje_header mensaje_appeared_catch_pokemon_get_header(t_mensaje_appeared_catch_pokemon* msj);

// Setters
void mensaje_appeared_catch_pokemon_set_id(t_mensaje_appeared_catch_pokemon* msj, uint32_t id);
void mensaje_appeared_catch_pokemon_set_id_correlativo(t_mensaje_appeared_catch_pokemon* msj, uint32_t id_correlativo);
void mensaje_appeared_catch_pokemon_set_header(t_mensaje_appeared_catch_pokemon* msj, t_mensaje_header header);

#endif /* SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_APPEARED_CATCH_POKEMON_H_ */
