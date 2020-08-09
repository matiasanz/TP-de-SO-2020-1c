/*
 * modo_suscriptor.c
 *
 *  Created on: 5 jul. 2020
 *      Author: utnso
 */

#include "modo_suscriptor.h"

static void mensaje_recibido(t_id_cola id_cola, void* msj);

bool modo_suscriptor(char*proceso){
	return string_equals_ignore_case(proceso, SUSCRIPTOR_STRING);
}

void procesar_modo_suscriptor(char* cola_mensaje_string, char* tiempo_conexion_string) {

	log_event_intento_de_suscripcion_a_cola(cola_mensaje_string, tiempo_conexion_string);

	validar_mayor_igual_a_cero(tiempo_conexion_string);

	int tiempo_conexion = atoi(tiempo_conexion_string);
	t_id_cola id_cola = get_id_mensaje(cola_mensaje_string);

	t_conexion_cliente* conexion_cliente = conexion_cliente_crear(id_cola,
			config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION"), (void*) mensaje_recibido);

	conexion_broker = conexion_server_crear(config_get_string_value(config, "IP_BROKER"),
			config_get_string_value(config, "PUERTO_BROKER"));

	t_conexion* args = conexion_crear(conexion_broker, conexion_cliente);

	pthread_create(&hilo_suscriptor, NULL, (void*) suscribir_y_escuchar_cola, args);
	pthread_detach(hilo_suscriptor);

	sleep(tiempo_conexion);

	log_event_fin_de_suscripcion_a_cola(tiempo_conexion, cola_mensaje_string);

	conexion_destruir(args);
}

static void mensaje_recibido(t_id_cola id_cola, void* msj) {

	void* deserializado;

	switch (id_cola) {

	case APPEARED_POKEMON:
	case CATCH_POKEMON:
		deserializado = mensaje_appeared_catch_pokemon_deserializar(msj);
		mensaje_appeared_catch_pokemon_log(logger, deserializado, get_nombre_cola(id_cola));
		mensaje_appeared_catch_pokemon_destruir(deserializado);
		break;

	case GET_POKEMON:
		deserializado = mensaje_get_pokemon_deserializar(msj);
		mensaje_get_pokemon_log(logger, deserializado);
		mensaje_get_pokemon_destruir(deserializado);
		break;

	case NEW_POKEMON:
		deserializado = mensaje_new_pokemon_deserializar(msj);
		mensaje_new_pokemon_log(logger, deserializado);
		mensaje_new_pokemon_destruir(deserializado);
		break;

	case CAUGHT_POKEMON:
		deserializado = mensaje_caught_pokemon_deserializar(msj);
		mensaje_caught_pokemon_log(logger, deserializado);
		mensaje_caught_pokemon_destruir(deserializado);
		break;

	case LOCALIZED_POKEMON:
		deserializado = mensaje_localized_pokemon_deserializar(msj);
		mensaje_localized_pokemon_log(logger, deserializado);
		mensaje_localized_pokemon_destruir(deserializado);
		break;

	default:
		log_error(event_logger, "El %s recibió un mensaje que no esperaba: (%s)",
		GAMEBOY_STRING, get_nombre_cola(id_cola));
	}

	free(msj);
}


