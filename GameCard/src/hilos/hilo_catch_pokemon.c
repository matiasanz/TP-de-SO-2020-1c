/*
 * hilo_catch_pokemon.c
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#include "hilo_catch_pokemon.h"

void subscribir_y_escuchar_cola_catch_pokemon(void (*callback)(t_id_cola, void*)) {

	conexion_catch_pokemon = conexion_cliente_crear(CATCH_POKEMON,
			config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION"),
			callback);

	t_conexion* args = conexion_crear(conexion_broker,
			conexion_catch_pokemon);

	pthread_create(&hilo_catch_pokemon, NULL,
			(void*) subscribir_y_escuchar_cola, args);

	pthread_detach(hilo_catch_pokemon);
}

void catch_pokemon_recibido(t_mensaje_appeared_catch_pokemon* catch_pokemon) {

	//	no borrar
	mensaje_appeared_catch_pokemon_log(logger, catch_pokemon, CATCH_POKEMON_STRING);
}
