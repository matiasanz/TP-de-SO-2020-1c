/*
 * hilo_appeared_pokemon.c
 *
 *  Created on: 31 may. 2020
 *      Author: C-renito Casero
 */

#include "../hilos-de-comunicacion/hilo_appeared_pokemon.h"
#include "../team.h"

void subscribir_y_escuchar_cola_appeared_pokemon(void (*callback)(t_id_cola, void*)) {

	conexion_appeared_pokemon = conexion_cliente_crear(APPEARED_POKEMON,
			config_get_int_value(config, "TIEMPO_RECONEXION"),
			(void*) callback);

	t_conexion* args = conexion_crear(conexion_broker,
			conexion_appeared_pokemon);

	pthread_create(&hilo_appeared_pokemon, NULL,
			(void*) subscribir_y_escuchar_cola, args);

	pthread_detach(hilo_appeared_pokemon);

}

void appeared_pokemon_recibido(t_mensaje_appeared_catch_pokemon* appeared_pokemon) {

	//	Log pedido en el enunciado (no borrar)
	mensaje_appeared_catch_pokemon_log(logger, appeared_pokemon, APPEARED_POKEMON_STRING);

	cr_list_add_and_signal(mensajesAPPEARED, appeared_pokemon);
}
