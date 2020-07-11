/*
 * hilo_new_pokemon.c
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#include "hilo_new_pokemon.h"

void suscribir_y_escuchar_cola_new_pokemon(void (*callback)(t_id_cola, void*)) {

	conexion_new_pokemon = conexion_cliente_crear(NEW_POKEMON, id_proceso,
			config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION"), callback);

	t_conexion* args = conexion_crear(conexion_broker, conexion_new_pokemon);

	pthread_create(&hilo_new_pokemon, NULL, (void*) suscribir_y_escuchar_cola, args);

	pthread_detach(hilo_new_pokemon);
}

void new_pokemon_recibido(t_mensaje_new_pokemon* new_pokemon) {
	mensaje_new_pokemon_log(logger, new_pokemon);
	// no borrar

	pthread_t unHilo;
	pthread_create(&unHilo, NULL,(void*) gamecard_New_Pokemon, new_pokemon);
	pthread_detach(unHilo);
}

void gamecard_New_Pokemon_Reintento(t_mensaje_new_pokemon* unMsjNewPoke){
	sleep(TIEMPO_REINTENTO_OPERACION);
	gamecard_New_Pokemon(unMsjNewPoke);
}
