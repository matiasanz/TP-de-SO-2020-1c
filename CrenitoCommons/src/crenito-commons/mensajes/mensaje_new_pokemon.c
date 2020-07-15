/*
 * mensaje_new_pokemon.c
 *
 *  Created on: 1 may. 2020
 *      Author: utnso
 */
#include "mensaje_new_pokemon.h"

t_mensaje_new_pokemon* mensaje_new_pokemon_crear(char* especie, uint32_t pos_x, uint32_t pos_y, uint32_t cantidad) {

	t_mensaje_new_pokemon* new_pokemon = malloc(sizeof(t_mensaje_new_pokemon));

	mensaje_header_inicializar(&new_pokemon->mensaje_header);
	new_pokemon->pokemon = pokemon_crear(especie, pos_x, pos_y);
	new_pokemon->cantidad = cantidad;

	return new_pokemon;
}

void mensaje_new_pokemon_destruir(t_mensaje_new_pokemon* new_pokemon) {

	pokemon_destruir(new_pokemon->pokemon);
	free(new_pokemon);
}

t_buffer* mensaje_new_pokemon_serializar(t_mensaje_new_pokemon* new_pokemon) {

	int bytes_pokemon = new_pokemon->pokemon.especie_lenght + sizeof(new_pokemon->pokemon.especie_lenght)
			+ sizeof(new_pokemon->pokemon.posicion);

	int size = sizeof(new_pokemon->mensaje_header) + bytes_pokemon + sizeof(new_pokemon->cantidad);

	t_buffer* bfr = buffer_crear(size);
	int desplazamiento = 0;

	// header
	memcpy(bfr->stream + desplazamiento, &(new_pokemon->mensaje_header), sizeof(new_pokemon->mensaje_header));
	desplazamiento += sizeof(new_pokemon->mensaje_header);

	// pokemon
	void* pkm_serializado = pokemon_serializar(new_pokemon->pokemon, bytes_pokemon);
	memcpy(bfr->stream + desplazamiento, pkm_serializado, bytes_pokemon);
	free(pkm_serializado);
	desplazamiento += bytes_pokemon;

	//cantidad
	memcpy(bfr->stream + desplazamiento, &(new_pokemon->cantidad), sizeof(new_pokemon->cantidad));
	desplazamiento += sizeof(new_pokemon->cantidad);

	return bfr;
}

t_mensaje_new_pokemon* mensaje_new_pokemon_deserializar(void* stream) {

	t_mensaje_new_pokemon* msj = malloc(sizeof(t_mensaje_new_pokemon));
	int desplazamiento = 0;

	// header
	memcpy(&msj->mensaje_header, stream + desplazamiento, sizeof(msj->mensaje_header));
	desplazamiento += sizeof(msj->mensaje_header);

	//pokemon
	int bytes_pokemon = 0;
	msj->pokemon = pokemon_deserializar(stream + desplazamiento, &bytes_pokemon);
	desplazamiento += bytes_pokemon;

	//cantidad
	memcpy(&msj->cantidad, stream + desplazamiento, sizeof(msj->cantidad));
	desplazamiento += sizeof(msj->cantidad);

	return msj;
}

void* mensaje_new_pokemon_formato_cache(void* stream, t_mensaje_cache_metadata* metadata) {

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
	metadata->tamanio_contenido = sizeof(uint32_t) + especie_lenght_trim + sizeof(t_posicion) + sizeof(uint32_t);
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

	//cantidad
	memcpy(contenido + desplazamiento_contenido, stream + desplazamiento_stream, sizeof(uint32_t));
	desplazamiento_stream += sizeof(uint32_t);
	desplazamiento_contenido += sizeof(uint32_t);

	return contenido;
}

t_mensaje_new_pokemon* mensaje_new_pokemon_restaurar_desde_cache(void* cache, t_mensaje_header header) {

	t_mensaje_new_pokemon* msj = malloc(sizeof(t_mensaje_new_pokemon));

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

	//cantidad
	memcpy(&msj->cantidad, cache + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	return msj;
}

void mensaje_new_pokemon_log(t_log* un_logger, t_mensaje_new_pokemon* new_pokemon) {

	char* to_string = mensaje_new_pokemon_to_string(new_pokemon);
	log_info_and_destroy(un_logger, to_string);
}

char* mensaje_new_pokemon_to_string(t_mensaje_new_pokemon* new_pokemon) {

	char *string = string_new();

	string_append_mensaje_header(&string, new_pokemon->mensaje_header,
	NEW_POKEMON_STRING);
	string_append_with_format(&string, " cantidad: %d \n", new_pokemon->cantidad);
	string_append_pokemon(&string, new_pokemon->pokemon);
	string_append(&string, "\n");

	return string;
}

// Getters
uint32_t mensaje_new_pokemon_get_id(t_mensaje_new_pokemon* msj) {
	return msj->mensaje_header.id;
}

uint32_t mensaje_new_pokemon_get_id_correlativo(t_mensaje_new_pokemon* msj) {
	return msj->mensaje_header.id_correlativo;
}

t_mensaje_header mensaje_new_pokemon_get_header(t_mensaje_new_pokemon* msj) {
	return msj ->mensaje_header;
}

//Setters
void mensaje_new_pokemon_set_id(t_mensaje_new_pokemon* msj, uint32_t id) {
	msj->mensaje_header.id = id;
}

void mensaje_new_pokemon_set_id_correlativo(t_mensaje_new_pokemon* msj, uint32_t id_correlativo) {
	msj->mensaje_header.id_correlativo = id_correlativo;
}

void mensaje_new_pokemon_set_header(t_mensaje_new_pokemon* msj, t_mensaje_header header) {
	msj->mensaje_header = header;
}
