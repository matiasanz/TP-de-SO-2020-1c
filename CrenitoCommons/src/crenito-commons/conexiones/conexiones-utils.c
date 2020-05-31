/*
 * conexiones-utils.c
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#include "conexiones-utils.h"

t_conexion_server* conexion_server_crear(char* ip, char* puerto, t_id_proceso id_proceso) {

	t_conexion_server* server = malloc(sizeof(t_conexion_server));

	server -> ip = ip;
	server-> puerto = puerto;
	server-> id_proceso = id_proceso;

	return server;
}

t_conexion_cliente* conexion_cliente_crear(t_id_cola id_cola , int segundos_reconexion, void(*callback)(void*)) {

	t_conexion_cliente* cliente = malloc(sizeof(t_conexion_cliente));

	cliente->subscriptor = subscriptor_crear(0, 0);
	cliente->id_cola = id_cola;
	cliente -> segundos_reconexion = segundos_reconexion;
	cliente -> callback = callback;

	return cliente;
}

t_conexion* conexion_crear(t_conexion_server* server, t_conexion_cliente* cliente) {

	t_conexion* conexion = malloc(sizeof(t_conexion));

	conexion -> cliente = cliente;
	conexion -> server = server;

	return conexion;
}

void conexion_destruir(t_conexion* conexion) {

	free(conexion);
}


t_subscriptor* subscriptor_crear(uint32_t socket, uint32_t id_subscriptor) {

	t_subscriptor* subscriptor = malloc(id_subscriptor);

	subscriptor->id_subcriptor = id_subscriptor;
	subscriptor->socket = socket;

	return subscriptor;
}

void* deserializar(void* msj, t_id_cola id_cola) {

	switch (id_cola) {
	case NEW_POKEMON:
		return mensaje_new_pokemon_deserializar(msj);
	case APPEARED_POKEMON:
	case CATCH_POKEMON:
		return  mensaje_appeared_catch_pokemon_deserializar(msj);
	case CAUGHT_POKEMON:
		return mensaje_caught_pokemon_deserializar(msj);
	case GET_POKEMON:
		return mensaje_get_pokemon_deserializar(msj);
	case LOCALIZED_POKEMON:
		return mensaje_localized_pokemon_deserializar(msj);
	default:
		log_error(event_logger, "No existe la cola: %d. Finalizando hilo",
				id_cola);
		pthread_exit(NULL);
		return NULL;
	}
}

t_buffer* serializar(void* msj, t_id_cola id_cola) {

	switch (id_cola) {
	case NEW_POKEMON:
		return mensaje_new_pokemon_serializar(msj);
	case APPEARED_POKEMON:
	case CATCH_POKEMON:
		return  mensaje_appeared_catch_pokemon_serializar(msj);
	case CAUGHT_POKEMON:
		return mensaje_caught_pokemon_serializar(msj);
	case GET_POKEMON:
		return mensaje_get_pokemon_serializar(msj);
	case LOCALIZED_POKEMON:
		return mensaje_localized_pokemon_serializar(msj);
	default:
		log_error(event_logger, "No existe la cola: %d. Finalizando hilo",
				id_cola);
		pthread_exit(NULL);
		return NULL;
	}
}
