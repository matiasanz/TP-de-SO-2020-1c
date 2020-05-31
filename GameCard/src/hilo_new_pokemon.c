/*
 * hilo_new_pokemon.c
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#include "hilo_new_pokemon.h"

void subscribir_y_escuchar_cola_new_pokemon() {

	conexion_new_pokemon = conexion_cliente_crear(NEW_POKEMON,
			config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION"),
			(void*) new_pokemon_recibido);

	t_conexion* args = conexion_crear(conexion_broker,
			conexion_new_pokemon);

	pthread_create(&hilo_new_pokemon, NULL,
			(void*) subscribir_y_escuchar_cola, args);

	pthread_detach(hilo_new_pokemon);
}

void new_pokemon_recibido(t_mensaje_new_pokemon* new_pokemon) {

	// no borrar
	mensaje_new_pokemon_log(logger, new_pokemon);
}
