/*
 * hilo_get_pokemon.c
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#include "hilo_get_pokemon.h"

void subscribir_y_escuchar_cola_get_pokemon() {

	conexion_get_pokemon = conexion_cliente_crear(GET_POKEMON,
			config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION"),
			(void*) get_pokemon_recibido);

	t_conexion* args = conexion_crear(conexion_broker,
			conexion_get_pokemon);

	pthread_create(&hilo_get_pokemon, NULL,
			(void*) subscribir_y_escuchar_cola, args);

	pthread_detach(hilo_get_pokemon);
}

void get_pokemon_recibido(t_mensaje_get_pokemon* get_pokemon) {

	// no borrar
	mensaje_get_pokemon_log(logger, get_pokemon);
}

