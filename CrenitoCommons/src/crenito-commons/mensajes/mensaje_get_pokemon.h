/*
 * mensaje_get_pokemon.h
 *
 *  Created on: 24 may. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_GET_POKEMON_H_
#define SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_GET_POKEMON_H_

#include "../conexiones/buffer.h"
#include "mensaje_cache_metadata.h"
#include "mensaje_header.h"

typedef struct {
	t_mensaje_header mensaje_header;
	char* especie;
	uint32_t especie_lenght;
} t_mensaje_get_pokemon;

// Creación y destrucción
t_mensaje_get_pokemon* mensaje_get_pokemon_crear(char* especie);
void mensaje_get_pokemon_destruir(t_mensaje_get_pokemon* get_pokemon);

// Serialización
t_buffer* mensaje_get_pokemon_serializar(t_mensaje_get_pokemon* get_pokemon);
t_mensaje_get_pokemon* mensaje_get_pokemon_deserializar(void* stream);

// Cache
void* mensaje_get_pokemon_formato_cache(void* stream, t_mensaje_cache_metadata* metadata);
t_mensaje_get_pokemon* mensaje_get_pokemon_restaurar_desde_cache(void* cache, t_mensaje_header header);

// Log
void mensaje_get_pokemon_log(t_log* un_logger, t_mensaje_get_pokemon* get_pokemon);
char* mensaje_get_pokemon_to_string(t_mensaje_get_pokemon* get_pokemon);

// Getters
uint32_t mensaje_get_pokemon_get_id(t_mensaje_get_pokemon* msj);
uint32_t mensaje_get_pokemon_get_id_correlativo(t_mensaje_get_pokemon* msj);

// Setters
void mensaje_get_pokemon_set_id(t_mensaje_get_pokemon* msj, uint32_t id);
void mensaje_get_pokemon_set_id_correlativo(t_mensaje_get_pokemon* msj, uint32_t id_correlativo);

#endif /* SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_GET_POKEMON_H_ */
