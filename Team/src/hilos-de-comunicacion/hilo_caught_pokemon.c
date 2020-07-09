/*
 * hilo_caught_pokemon.c
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#include "../hilos-de-comunicacion/hilo_caught_pokemon.h"
#include "../team.h"

void suscribir_y_escuchar_cola_caught_pokemon(void (*callback)(t_id_cola, void*)) {

	conexion_caught_pokemon = conexion_cliente_crear(CAUGHT_POKEMON, id_proceso,
			config_get_int_value(config, "TIEMPO_RECONEXION"), callback);

	t_conexion* args_s = conexion_crear(conexion_broker, conexion_caught_pokemon);

	pthread_create(&hilo_caught_pokemon, NULL, (void*) suscribir_y_escuchar_cola, args_s);

	pthread_detach(hilo_caught_pokemon);
}

void caught_pokemon_recibido(t_mensaje_caught_pokemon* caught_pokemon) {
	cr_list_add_and_signal(mensajesCAUGHT, caught_pokemon);
}
