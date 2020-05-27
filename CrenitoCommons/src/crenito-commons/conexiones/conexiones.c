/*
 * conexiones.c
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#include "conexiones.h"

//Funciones privadas
static int handshake(void* pqt, int size, int socket);

t_conexion_cliente subscribir_cola(t_conexion_server server, t_id_cola id_cola){

	t_conexion_cliente subscriptor;
	t_paquete_header pqt = paquete_header_crear(SUBSCRIPCION, server.id_proceso, id_cola);

	subscriptor.socket = socket_crear_client(server.ip, server.puerto);
	subscriptor.id_subcriptor = handshake(&pqt, sizeof(t_paquete_header), subscriptor.socket);

	return subscriptor;
}

t_conexion_server conexion_server_crear(char* ip, char* puerto, t_id_proceso id_proceso, int segundos_reconexion){

	t_conexion_server server;

	server.ip = ip;
	server.puerto = puerto;
	server.id_proceso = id_proceso;
	server.segundos_reconexion = segundos_reconexion;

	return server;
}

t_conexion_cliente* conexion_cliente_crear(int socket, int id_subscriptor){

	t_conexion_cliente* cliente = malloc(sizeof(t_conexion_cliente));

	cliente -> socket = socket;
	cliente -> id_subcriptor = id_subscriptor;

	return cliente;

}

static int handshake(void* pqt, int size, int socket){

	socket_send(socket, pqt, size);
	return socket_recibir_int(socket);
}
