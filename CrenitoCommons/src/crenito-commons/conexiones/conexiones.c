/*
 * conexiones.c
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#include "conexiones.h"

/*
 * Subscribe a los distintos procesos con el Broker.
 */
static void subscribir(t_conexion_server* server, t_conexion_cliente* cliente);
/*
 * Realiza escucha activa en un socket cliente.
 */
static void escuchar(t_conexion_cliente* cliente);
/*
 * Envía un paquete al socket indicado por parámetro
 * y espera una respuesta de tipo entero que devuelve
 */
static int handshake(int socket, void* pqt, int size);

int enviar(t_conexion_server* server, t_paquete* pqt) {

	int socket = socket_crear_client(server->ip, server->puerto);

	int id_mensaje = enviar_paquete(pqt, socket);

	socket_cerrar(socket);

	return id_mensaje;
}

int enviar_paquete(t_paquete* pqt, int socket) {

	int bytes = 0;
	void* a_enviar = paquete_serializar(pqt, &bytes);

	int id_mensaje = handshake(socket, a_enviar, bytes);
	free(a_enviar);

	return id_mensaje;
}

void* recibir(t_conexion_cliente* cliente) {

	int size = 0;
	void* msj = socket_recibir_mensaje(cliente->subscriptor->socket, &size);

	uint32_t ACK = 1;
	socket_send(cliente->subscriptor->socket, &ACK, sizeof(ACK));

	return msj;
}

void subscribir_y_escuchar_cola(t_conexion* args) {

	pthread_mutex_lock(&mutex_subscripcion);
	subscribir(args->server, args->cliente);
	pthread_mutex_unlock(&mutex_subscripcion);

	escuchar(args->cliente);
}

static int handshake(int socket, void* pqt, int size) {

	socket_send(socket, pqt, size);
	return socket_recibir_int(socket);
}

static void escuchar(t_conexion_cliente* cliente) {

	while (1) {
		t_paquete_header header = socket_recibir_header(
				cliente->subscriptor->socket);

		void* msj = recibir(cliente);

		void* deserializado = deserializar(msj,	header.id_cola);
		free(msj);

		cliente->callback(deserializado);

	}
}

static void subscribir(t_conexion_server* server, t_conexion_cliente* cliente) {

	t_paquete_header pqt = paquete_header_crear(SUBSCRIPCION,
			server->id_proceso, cliente->id_cola);

	cliente->subscriptor->socket = socket_crear_client(server->ip,
			server->puerto);

	cliente->subscriptor->id_subcriptor = handshake(
			cliente->subscriptor->socket, &pqt, sizeof(t_paquete_header));
}
