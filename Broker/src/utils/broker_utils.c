/*
 * broker_utils.c
 *
 *  Created on: 16 jun. 2020
 *      Author: utnso
 */

#include "broker_utils.h"

uint32_t generar_id_univoco() {

	uint32_t id_mensaje = 0;

	pthread_mutex_lock(&mutex_id_univoco);
	id_univoco += 1;
	id_mensaje = id_univoco;
	pthread_mutex_unlock(&mutex_id_univoco);

	return id_mensaje;
}

long get_fecha_en_microsegundos(struct timeval fecha) {
	return (fecha.tv_sec) * 1000 + (fecha.tv_usec) / 1000;
}

void string_append_timestamp(char** string) {

	time_t timer;
	char buffer[26];
	struct tm* tm_info;

	timer = time(NULL);
	tm_info = localtime(&timer);

	strftime(buffer, 26, "%d/%m/%Y %H:%M:%S", tm_info);
	string_append_with_format(string, "\n Dump: ");
	string_append_with_format(string, buffer);
	string_append_with_format(string, "\n");
}

t_mensaje_header mensaje_get_header(void* mensaje, t_id_cola id_cola) {

	switch (id_cola) {
	case NEW_POKEMON:
		return mensaje_new_pokemon_get_header(mensaje);
	case APPEARED_POKEMON:
		return mensaje_appeared_catch_pokemon_get_header(mensaje);
	case CATCH_POKEMON:
		return mensaje_appeared_catch_pokemon_get_header(mensaje);
	case CAUGHT_POKEMON:
		return mensaje_caught_pokemon_get_header(mensaje);
	case GET_POKEMON:
		return mensaje_get_pokemon_get_header(mensaje);
	case LOCALIZED_POKEMON:
		return mensaje_localized_pokemon_get_header(mensaje);
	default:
		log_warning_cola(id_cola, "mensaje_get_header");
		t_mensaje_header header;
		return header;
	}
}

void mensaje_set_header(void* mensaje, t_mensaje_header header, t_id_cola id_cola) {

	switch (id_cola) {
	case NEW_POKEMON:
		mensaje_new_pokemon_set_header(mensaje, header);
		break;
	case APPEARED_POKEMON:
	case CATCH_POKEMON:
		mensaje_appeared_catch_pokemon_set_header(mensaje, header);
		break;
	case CAUGHT_POKEMON:
		mensaje_caught_pokemon_set_header(mensaje, header);
		break;
	case GET_POKEMON:
		mensaje_get_pokemon_set_header(mensaje, header);
		break;
	case LOCALIZED_POKEMON:
		mensaje_localized_pokemon_set_header(mensaje, header);
		break;
	default:
		log_warning_cola(id_cola, "mensaje_get_header");
	}
}

void mensaje_destruir(void* deserializado, t_id_cola id_cola) {

	switch (id_cola) {
	case NEW_POKEMON:
		mensaje_new_pokemon_destruir(deserializado);
		break;
	case APPEARED_POKEMON:
	case CATCH_POKEMON:
		mensaje_appeared_catch_pokemon_destruir(deserializado);
		break;
	case CAUGHT_POKEMON:
		mensaje_caught_pokemon_destruir(deserializado);
		break;
	case GET_POKEMON:
		mensaje_get_pokemon_destruir(deserializado);
		break;
	case LOCALIZED_POKEMON:
		mensaje_localized_pokemon_destruir(deserializado);
		break;
	default:
		log_warning_cola(id_cola, "mensaje_destruir");
	}
}

uint32_t siguiente_potencia_de_dos(uint32_t un_numero) {

	int i = 0;
	for (--un_numero; un_numero > 0; un_numero >>= 1) {
		i++;
	}
	return 1 << i;
}

uint32_t logaritmo_base_dos(uint32_t un_numero) {

	uint32_t valor;
	for (valor = 0; un_numero > 1; valor++, un_numero >>= 1);

	return valor;
}

uint32_t calcular_base_dos(uint32_t un_exponente) {

	uint32_t i = 0;
	uint32_t base = 1;

	while (i < un_exponente) {
		base = base * 2;
		i++;
	}

	return base;
}
