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
 * Envía un paquete usando el socket indicado por parámetro
 * y se queda a la espera una respuesta de tipo entero
 * la cual retorna
 */
static int handshake(int socket, void* pqt, int size);
/*
 * Recibe un paquete usando el socket indicado por parámetro.
 * Al recibirlo, ejeceuta la funcion de escucha
 * que también recibe por parámetro.
 */
static void recibir(int socket, void (*callback)(void*));

int enviar(t_conexion_server* server, t_paquete* pqt) {

	int socket = socket_crear_client(server->ip, server->puerto);

	int id_mensaje = enviar_paquete(pqt, socket);

	socket_cerrar(socket);

	//TO-DO Si falla la conexion devolver -1
	return id_mensaje;
}

int enviar_paquete(t_paquete* pqt, int socket) {

	int bytes = 0;
	void* a_enviar = paquete_serializar(pqt, &bytes);

	int id_mensaje = handshake(socket, a_enviar, bytes);
	free(a_enviar);

	return id_mensaje;
}

void subscribir_y_escuchar_cola(t_conexion* args) {

	pthread_mutex_lock(&mutex_subscripcion);
	subscribir(args->server, args->cliente);
	pthread_mutex_unlock(&mutex_subscripcion);

	t_conexion_cliente* cliente = args->cliente;
	t_subscriptor* subscriptor = cliente->subscriptor;

	while (1) {
		//TO-DO reconectar
		recibir(subscriptor->socket, cliente->callback);
	}
}

static int handshake(int socket, void* pqt, int size) {

	socket_send(socket, pqt, size);
	return socket_recibir_int(socket);
}

void conectar_y_escuchar_gameboy(t_conexion_host* gameboy) {

	int socket_server = socket_crear_listener(gameboy->ip, gameboy->puerto);

	while (1) {

		int socket_cliente = socket_aceptar_conexion(socket_server);
		recibir(socket_cliente, gameboy->callback);
	}

}
static void subscribir(t_conexion_server* server, t_conexion_cliente* cliente) {

	t_paquete_header pqt = paquete_header_crear(SUBSCRIPCION,
			server->id_proceso, cliente->id_cola);

	//TO-DO reconectar
	cliente->subscriptor->socket = socket_crear_client(server->ip, server->puerto);

	cliente->subscriptor->id_subcriptor = handshake(cliente->subscriptor->socket, &pqt,
			sizeof(t_paquete_header));
}

static void recibir(int socket, void (*callback)(void*)) {

	t_paquete_header header = socket_recibir_header(socket);

	int size = 0;
	void* msj = socket_recibir_mensaje(socket, &size);

	uint32_t ACK = 1;
	socket_send(socket, &ACK, sizeof(ACK));

	t_buffer* bfr = buffer_crear(size);
	buffer_set_stream(bfr, msj);

	callback(paquete_crear(header, bfr));
}
