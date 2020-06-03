/*
 * hilo_localized_pokemon.c
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#include "../hilos_comunicacion/hilo_localized_pokemon.h"

void subscribir_y_escuchar_cola_localized_pokemon(void(*callback)(void*)) {

	conexion_localized_pokemon = conexion_cliente_crear(LOCALIZED_POKEMON,
			config_get_int_value(config, "TIEMPO_RECONEXION"),
			callback);

	t_conexion* args_s = conexion_crear(conexion_broker,
			conexion_localized_pokemon);

	pthread_create(&hilo_localized_pokemon, NULL,
			(void*) subscribir_y_escuchar_cola, args_s);

	pthread_detach(hilo_localized_pokemon);
}

void localized_pokemon_recibido(t_mensaje_localized_pokemon* localized_pokemon) {

	//	Log pedido en el enunciado (no borrar)
	mensaje_localized_pokemon_log(logger, localized_pokemon);
}
