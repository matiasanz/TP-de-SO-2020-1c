/*
 * conexiones.c
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#include "conexiones.h"

char* get_nombre_proceso(t_id_proceso id_proceso) {

	switch (id_proceso) {
	case BROKER:
		return BROKER_STRING;
	case GAMEBOY:
		return GAMEBOY_STRING;
	case GAMECARD:
		return GAMECARD_STRING;
	case TEAM:
		return TEAM_STRING;
	default:
		log_error(get_crnito_logger(), "Id de proceso incorrecto: %d", id_proceso);
		return NULL;
	}
}

char* get_nombre_cola_mensaje(t_tipo_cola_mensaje tipo_cola_mensaje) {

	switch (tipo_cola_mensaje) {
	case NEW_POKEMON:
		return NEW_POKEMON_STRING;
	case APPEARED_POKEMON:
		return APPEARED_POKEMON_STRING;
	case CATCH_POKEMON:
		return CATCH_POKEMON_STRING;
	case CAUGHT_POKEMON:
		return CAUGHT_POKEMON_STRING;
	case GET_POKEMON:
		return GET_POKEMON_STRING;
	case LOCALIZED_POKEMON:
		return LOCALIZED_POKEMON_STRING;
	default:
		log_error(get_crnito_logger(), "No existe el tipo de cola: %d",
				tipo_cola_mensaje);
		return NULL;
	}
}

int subscripcion_cola(t_id_proceso id_proceso, t_tipo_cola_mensaje nombre_cola,
		char* ip, char* puerto) {

	t_mensaje_subscripcion* msj = mensaje_subscripcion_crear(id_proceso, nombre_cola);
	int socket_ciente = socket_crear_client(ip, puerto);

	//Inicio handshake
	socket_send(socket_ciente, msj, sizeof(t_mensaje_subscripcion));
	int status = socket_recibir_int(socket_ciente);
	//Fin handshake

	socket_cerrar(socket_ciente);
	mensaje_subscripcion_destruir(msj);

	return status;
}

int subscripcion_exitosa(int estado) {
	return estado == SUBSCRIPCION_EXITO;
}
