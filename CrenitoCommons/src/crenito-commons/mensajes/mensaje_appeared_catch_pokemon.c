/*
 * mensaje_appeared_pokemon.c
 *
 *  Created on: 24 may. 2020
 *      Author: utnso
 */

#include "mensaje_appeared_catch_pokemon.h"

t_mensaje_appeared_catch_pokemon* mensaje_appeared_catch_pokemon_crear(char* especie, uint32_t pos_x,
		uint32_t pos_y) {

	t_mensaje_appeared_catch_pokemon* appeared_catch_pokemon = malloc(sizeof(t_mensaje_appeared_catch_pokemon));

	mensaje_header_inicializar(&appeared_catch_pokemon->mensaje_header);
	appeared_catch_pokemon->pokemon = pokemon_crear(especie, pos_x, pos_y);

	return appeared_catch_pokemon;
}

void mensaje_appeared_catch_pokemon_destruir(t_mensaje_appeared_catch_pokemon* appeared_pokemon) {

	pokemon_destruir(appeared_pokemon->pokemon);
	free(appeared_pokemon);
}

t_buffer* mensaje_appeared_catch_pokemon_serializar(t_mensaje_appeared_catch_pokemon* appeared_pokemon) {

	int bytes_pokemon = appeared_pokemon->pokemon.especie_lenght + sizeof(appeared_pokemon->pokemon.especie_lenght)
			+ sizeof(appeared_pokemon->pokemon.posicion);

	int size = sizeof(appeared_pokemon->mensaje_header) + bytes_pokemon;

	t_buffer* bfr = buffer_crear(size);
	int desplazamiento = 0;

	// header
	memcpy(bfr->stream + desplazamiento, &(appeared_pokemon->mensaje_header),
			sizeof(appeared_pokemon->mensaje_header));
	desplazamiento += sizeof(appeared_pokemon->mensaje_header);

	// pokemon
	void* pkm_serializado = pokemon_serializar(appeared_pokemon->pokemon, bytes_pokemon);
	memcpy(bfr->stream + desplazamiento, pkm_serializado, bytes_pokemon);
	free(pkm_serializado);
	desplazamiento += bytes_pokemon;

	return bfr;
}

t_mensaje_appeared_catch_pokemon* mensaje_appeared_catch_pokemon_deserializar(void* stream) {

	t_mensaje_appeared_catch_pokemon* msj = malloc(sizeof(t_mensaje_appeared_catch_pokemon));
	int desplazamiento = 0;

	// header
	memcpy(&msj->mensaje_header, stream + desplazamiento, sizeof(msj->mensaje_header));
	desplazamiento += sizeof(msj->mensaje_header);

	//pokemon
	int bytes_pokemon = 0;
	msj->pokemon = pokemon_deserializar(stream + desplazamiento, &bytes_pokemon);
	desplazamiento += bytes_pokemon;

	return msj;
}

void* mensaje_appeared_catch_pokemon_formato_cache(void* stream, t_mensaje_cache_metadata* metadata) {

	int desplazamiento_stream = 0;
	int desplazamiento_contenido = 0;

	// header
	t_mensaje_header header = mensaje_header_deserializar(stream);
	metadata->id_correlativo = header.id_correlativo;
	desplazamiento_stream += sizeof(t_mensaje_header);

	// especie lenght
	int especie_lenght = 0;
	memcpy(&especie_lenght, stream + desplazamiento_stream, sizeof(uint32_t));
	desplazamiento_stream += sizeof(uint32_t);
	int especie_lenght_trim = especie_lenght - 1; //en la cache se remueve el caracter de fin de cadena.

	//Contenido
	metadata->tamanio_contenido = sizeof(uint32_t) + especie_lenght_trim + sizeof(t_posicion);
	void* contenido = malloc(metadata->tamanio_contenido);

	//especie lenght
	memcpy(contenido, &especie_lenght_trim, sizeof(uint32_t));
	desplazamiento_contenido += sizeof(uint32_t);

	//especie
	memcpy(contenido + desplazamiento_contenido, stream + desplazamiento_stream, especie_lenght_trim);
	desplazamiento_stream += especie_lenght;
	desplazamiento_contenido += especie_lenght_trim;

	//posicion
	memcpy(contenido + desplazamiento_contenido, stream + desplazamiento_stream, sizeof(t_posicion));
	desplazamiento_stream += sizeof(t_posicion);
	desplazamiento_contenido += sizeof(t_posicion);

	return contenido;
}

t_mensaje_appeared_catch_pokemon* mensaje_appeared_catch_pokemon_restaurar_desde_cache(void* cache,
		t_mensaje_header header) {

	t_mensaje_appeared_catch_pokemon* msj = malloc(sizeof(t_mensaje_appeared_catch_pokemon));

	msj->mensaje_header = header;

	int desplazamiento = 0;

	t_pokemon pkm;
	//especie lenght
	int especie_lenght_trim = 0;
	memcpy(&especie_lenght_trim, cache + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	pkm.especie_lenght = especie_lenght_trim + 1; //en la cache se remueve el caracter de fin de cadena.

	//especie
	pkm.especie = malloc(pkm.especie_lenght);
	memcpy(pkm.especie, cache + desplazamiento, especie_lenght_trim);

	pkm.especie[especie_lenght_trim] = '\0';
	desplazamiento += especie_lenght_trim;

	//posicion
	memcpy(&pkm.posicion, cache + desplazamiento, sizeof(t_posicion));
	desplazamiento += sizeof(t_posicion);

	msj->pokemon = pkm;

	return msj;
}

void mensaje_appeared_catch_pokemon_log(t_log* un_logger, t_mensaje_appeared_catch_pokemon* appeared_catch,
		char* tipo) {

	char* to_string = mensaje_appeared_catch_pokemon_to_string(appeared_catch, tipo);
	log_info(un_logger, to_string);
	free(to_string);
}

char* mensaje_appeared_catch_pokemon_to_string(t_mensaje_appeared_catch_pokemon* appeared_catch, char* tipo) {

	char *string = string_new();

	string_append_mensaje_header(&string, appeared_catch->mensaje_header, tipo);
	string_append_pokemon(&string, appeared_catch->pokemon);

	string_append(&string, "\n");

	return string;
}

// Getters
uint32_t mensaje_appeared_catch_pokemon_get_id(t_mensaje_appeared_catch_pokemon* msj) {
	return msj->mensaje_header.id;
}

uint32_t mensaje_appeared_catch_pokemon_get_id_correlativo(t_mensaje_appeared_catch_pokemon* msj) {
	return msj->mensaje_header.id_correlativo;
}

//Setters
void mensaje_appeared_catch_pokemon_set_id(t_mensaje_appeared_catch_pokemon* msj, uint32_t id) {
	msj->mensaje_header.id = id;
}

void mensaje_appeared_catch_pokemon_set_id_correlativo(t_mensaje_appeared_catch_pokemon* msj,
		uint32_t id_correlativo) {
	msj->mensaje_header.id_correlativo = id_correlativo;
}
