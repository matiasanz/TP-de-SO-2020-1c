/*
 * mensaje_caught_pokemon.c
 *
 *  Created on: 25 may. 2020
 *      Author: utnso
 */
#include "mensaje_caught_pokemon.h"

t_mensaje_caught_pokemon* mensaje_caught_pokemon_crear(uint32_t atrapado) {

	t_mensaje_caught_pokemon* caught_pokemon = malloc(sizeof(t_mensaje_caught_pokemon));

	mensaje_header_inicializar(&caught_pokemon->mensaje_header);
	caught_pokemon->atrapado = atrapado;

	return caught_pokemon;
}

void mensaje_caught_pokemon_destruir(t_mensaje_caught_pokemon* caught_pokemon) {

	free(caught_pokemon);
}

t_buffer* mensaje_caught_pokemon_serializar(t_mensaje_caught_pokemon* caught_pokemon) {

	int size = sizeof(caught_pokemon->mensaje_header) + sizeof(caught_pokemon->atrapado);

	t_buffer* bfr = buffer_crear(size);
	int desplazamiento = 0;

	// header
	memcpy(bfr->stream + desplazamiento, &(caught_pokemon->mensaje_header),
			sizeof(caught_pokemon->mensaje_header));
	desplazamiento += sizeof(caught_pokemon->mensaje_header);

	//atrapado
	memcpy(bfr->stream + desplazamiento, &(caught_pokemon->atrapado), sizeof(caught_pokemon->atrapado));
	desplazamiento += sizeof(caught_pokemon->atrapado);

	return bfr;
}

t_mensaje_caught_pokemon* mensaje_caught_pokemon_deserializar(void* stream) {

	t_mensaje_caught_pokemon* msj = malloc(sizeof(t_mensaje_caught_pokemon));
	int desplazamiento = 0;

	// header
	memcpy(&msj->mensaje_header, stream + desplazamiento, sizeof(msj->mensaje_header));
	desplazamiento += sizeof(msj->mensaje_header);

	//atrapado
	memcpy(&msj->atrapado, stream + desplazamiento, sizeof(msj->atrapado));
	desplazamiento += sizeof(msj->atrapado);

	return msj;
}

void* mensaje_caught_pokemon_formato_cache(void* stream, t_mensaje_cache_metadata* metadata) {

	int desplazamiento = 0;

	// header
	t_mensaje_header header = mensaje_header_deserializar(stream);
	metadata->id_correlativo = header.id_correlativo;
	desplazamiento += sizeof(t_mensaje_header);

	//Contenido
	metadata->tamanio_contenido = sizeof(uint32_t);
	void* contenido = malloc(metadata->tamanio_contenido);

	//atrapado
	memcpy(contenido, stream + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	return contenido;
}

t_mensaje_caught_pokemon* mensaje_caught_pokemon_restaurar_desde_cache(void* cache, t_mensaje_header header) {

	t_mensaje_caught_pokemon* msj = malloc(sizeof(t_mensaje_caught_pokemon));

	msj->mensaje_header = header;

	int desplazamiento = 0;

	//atrapado
	memcpy(&msj->atrapado, cache + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	return msj;
}

void mensaje_caught_pokemon_log(t_log* un_logger, t_mensaje_caught_pokemon* caught_pokemon) {

	char* to_string = mensaje_caught_pokemon_to_string(caught_pokemon);
	log_info_and_destroy(un_logger, to_string);
}

char* mensaje_caught_pokemon_to_string(t_mensaje_caught_pokemon* caught_pokemon) {

	char *string = string_new();

	string_append_mensaje_header(&string, caught_pokemon->mensaje_header,
	CAUGHT_POKEMON_STRING);

	string_append_with_format(&string, " atrapado: %s",
			(caught_pokemon->atrapado? "OK": "FAIL"));

	string_append(&string, "\n");

	return string;
}

// Getters
uint32_t mensaje_caught_pokemon_get_id(t_mensaje_caught_pokemon* msj) {
	return msj->mensaje_header.id;
}

t_mensaje_header mensaje_caught_pokemon_get_header(t_mensaje_caught_pokemon* msj) {
	return msj ->mensaje_header;
}

//Setters
void mensaje_caught_pokemon_set_id(t_mensaje_caught_pokemon* msj, uint32_t id) {
	msj->mensaje_header.id = id;
}

uint32_t mensaje_caught_pokemon_get_id_correlativo(t_mensaje_caught_pokemon* msj) {
	return msj->mensaje_header.id_correlativo;
}


void mensaje_caught_pokemon_set_id_correlativo(t_mensaje_caught_pokemon* msj, uint32_t id_correlativo) {
	msj->mensaje_header.id_correlativo = id_correlativo;
}

void mensaje_caught_pokemon_set_header(t_mensaje_caught_pokemon* msj, t_mensaje_header header) {
	msj->mensaje_header = header;
}
