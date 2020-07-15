/*
 * mensaje_new_pokemon.h
 *
 *  Created on: 1 may. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_NEW_POKEMON_H_
#define SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_NEW_POKEMON_H_

#include "../modelo/pokemon.h"
#include "../conexiones/buffer.h"
#include "mensaje_cache_metadata.h"
#include "mensaje_header.h"

typedef struct {
	t_mensaje_header mensaje_header;
	t_pokemon pokemon;
	uint32_t cantidad;
} t_mensaje_new_pokemon;

// Creación y destrucción
t_mensaje_new_pokemon* mensaje_new_pokemon_crear(char* especie, uint32_t pos_x, uint32_t pos_y, uint32_t cantidad);
void mensaje_new_pokemon_destruir(t_mensaje_new_pokemon* new_pokemon);

// Serialización
t_buffer* mensaje_new_pokemon_serializar(t_mensaje_new_pokemon* new_pokemon);
t_mensaje_new_pokemon* mensaje_new_pokemon_deserializar(void* stream);

// Cache
void* mensaje_new_pokemon_formato_cache(void* stream, t_mensaje_cache_metadata* metadata);
t_mensaje_new_pokemon* mensaje_new_pokemon_restaurar_desde_cache(void* cache, t_mensaje_header header);

// Log
void mensaje_new_pokemon_log(t_log* un_logger, t_mensaje_new_pokemon* new_pokemon);
char* mensaje_new_pokemon_to_string(t_mensaje_new_pokemon* new_pokemon);

// Getters
uint32_t mensaje_new_pokemon_get_id(t_mensaje_new_pokemon* msj);
uint32_t mensaje_new_pokemon_get_id_correlativo(t_mensaje_new_pokemon* msj);
t_mensaje_header mensaje_new_pokemon_get_header(t_mensaje_new_pokemon* msj);

// Setters
void mensaje_new_pokemon_set_id(t_mensaje_new_pokemon* msj, uint32_t id);
void mensaje_new_pokemon_set_id_correlativo(t_mensaje_new_pokemon* msj, uint32_t id_correlativo);
void mensaje_new_pokemon_set_header(t_mensaje_new_pokemon* msj, t_mensaje_header header);

#endif /* SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_NEW_POKEMON_H_ */
