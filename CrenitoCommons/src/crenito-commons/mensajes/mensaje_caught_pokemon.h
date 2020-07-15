/*
 * mensaje_caught_pokemon.h
 *
 *  Created on: 25 may. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_CAUGHT_POKEMON_H_
#define SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_CAUGHT_POKEMON_H_

#include "../conexiones/buffer.h"
#include "mensaje_cache_metadata.h"
#include "mensaje_header.h"

/* Este mensaje podría ser empaquetado sin serialziar pero es preferible que todos
*  sigan la misma interfaz */
typedef struct {
	t_mensaje_header mensaje_header;
	uint32_t atrapado;  // 1 o 0
} t_mensaje_caught_pokemon;

// Creación y destrucción
t_mensaje_caught_pokemon* mensaje_caught_pokemon_crear(uint32_t atrapado);
void mensaje_caught_pokemon_destruir(t_mensaje_caught_pokemon* caught_pokemon);

// Serialización
t_buffer* mensaje_caught_pokemon_serializar(t_mensaje_caught_pokemon* caught_pokemon);
t_mensaje_caught_pokemon* mensaje_caught_pokemon_deserializar(void* stream);

// Cache
void* mensaje_caught_pokemon_formato_cache(void* stream, t_mensaje_cache_metadata* metadata);
t_mensaje_caught_pokemon* mensaje_caught_pokemon_restaurar_desde_cache(void* cache, t_mensaje_header header);

// Log
void mensaje_caught_pokemon_log(t_log* un_logger, t_mensaje_caught_pokemon* caught_pokemon);
char* mensaje_caught_pokemon_to_string(t_mensaje_caught_pokemon* caught_pokemon);

// Getters
uint32_t mensaje_caught_pokemon_get_id(t_mensaje_caught_pokemon* msj);
uint32_t mensaje_caught_pokemon_get_id_correlativo(t_mensaje_caught_pokemon* msj);
t_mensaje_header mensaje_caught_pokemon_get_header(t_mensaje_caught_pokemon* msj);

// Setters
void mensaje_caught_pokemon_set_id(t_mensaje_caught_pokemon* msj, uint32_t id);
void mensaje_caught_pokemon_set_id_correlativo(t_mensaje_caught_pokemon* msj, uint32_t id_correlativo);
void mensaje_caught_pokemon_set_header(t_mensaje_caught_pokemon* msj, t_mensaje_header header);

#endif /* SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_CAUGHT_POKEMON_H_ */
