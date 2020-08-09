/*
 * hilo_get_pokemon.c
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#include "hilo_get_pokemon.h"

void suscribir_y_escuchar_cola_get_pokemon(void (*callback)(t_id_cola, void*)) {

	conexion_get_pokemon = conexion_cliente_crear(GET_POKEMON,
			config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION"), callback);

	t_conexion* args = conexion_crear(conexion_broker, conexion_get_pokemon);

	pthread_create(&hilo_get_pokemon, NULL, (void*) suscribir_y_escuchar_cola, args);
	pthread_detach(hilo_get_pokemon);
}

void get_pokemon_recibido(t_mensaje_get_pokemon* get_pokemon) {

	pthread_mutex_lock(&mutexLogger);
	mensaje_get_pokemon_log(logger, get_pokemon);
	pthread_mutex_unlock(&mutexLogger);

	pthread_t unHilo;
	pthread_create(&unHilo, NULL,(void*) gamecard_procesar_Get_Pokemon, get_pokemon);
	pthread_detach(unHilo);
}

