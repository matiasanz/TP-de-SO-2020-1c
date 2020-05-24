/*
 * mensaje_localized_pokemon.h
 *
 *  Created on: 24 may. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_LOCALIZED_POKEMON_H_
#define SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_LOCALIZED_POKEMON_H_

#include "../conexiones/buffer.h"
#include "../modelo/posicion.h"
#include <commons/collections/list.h>

typedef struct {
	t_mensaje_id ids;
	char* especie;
	uint32_t especie_lenght;
	uint32_t posiciones_lenght;
	t_list* posiciones;
} t_mensaje_localized_pokemon;

t_mensaje_localized_pokemon* mensaje_localized_pokemon_crear(char* especie, t_list* posiciones);
void mensaje_localized_pokemon_destruir(t_mensaje_localized_pokemon* localized_pokemon);
t_buffer* mensaje_localized_pokemon_serializar(t_mensaje_localized_pokemon* localized_pokemon);
t_mensaje_localized_pokemon* mensaje_localized_pokemon_deserializar(t_buffer* buffer);

#endif /* SRC_CRENITO_COMMONS_MENSAJES_MENSAJE_LOCALIZED_POKEMON_H_ */
