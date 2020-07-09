/*
 * conexiones-utils.c
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#include "conexiones-utils.h"

t_conexion_host* conexion_host_crear(char* ip, char* puerto, void (*callback)(t_id_cola, void*)) {

	t_conexion_host* host = malloc(sizeof(t_conexion_host));

	host-> ip = ip;
	host-> puerto = puerto;
	host-> callback = callback;

	return host;
}

t_conexion_server* conexion_server_crear(char* ip, char* puerto, t_id_proceso id_proceso) {

	t_conexion_server* server = malloc(sizeof(t_conexion_server));

	server -> ip = ip;
	server-> puerto = puerto;
	server-> id_proceso = id_proceso;

	return server;
}

void conexion_server_destruir(t_conexion_server* server) {

	free(server);
}

t_conexion_cliente* conexion_cliente_crear(t_id_cola id_cola , int segundos_reconexion, void (*callback)(t_id_cola, void*)) {

	t_conexion_cliente* cliente = malloc(sizeof(t_conexion_cliente));

	cliente->subscriptor = suscriptor_crear(0, 0);
	cliente->id_cola = id_cola;
	cliente -> segundos_reconexion = segundos_reconexion;
	cliente -> callback = callback;

	return cliente;
}

void conexion_cliente_destruir(t_conexion_cliente* cliente) {

	suscriptor_destruir(cliente ->subscriptor);
	free(cliente);
}

t_conexion* conexion_crear(t_conexion_server* server, t_conexion_cliente* cliente) {

	t_conexion* conexion = malloc(sizeof(t_conexion));

	conexion -> cliente = cliente;
	conexion -> server = server;

	return conexion;
}

void conexion_destruir(t_conexion* conexion) {

	conexion_server_destruir(conexion -> server);
	conexion_cliente_destruir(conexion -> cliente);
	free(conexion);
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
