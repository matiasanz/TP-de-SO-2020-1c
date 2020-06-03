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
static int subscribir(t_conexion_server* server, t_conexion_cliente* cliente);
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
 * Si se produce un error, retorna ERROR_SOCKET
 */
static int recibir(int socket, void (*callback)(t_id_cola, void*));

int enviar(t_conexion_server* server, t_paquete* pqt) {

	int socket = socket_crear_client(server->ip, server->puerto);

	if (error_conexion(socket)) {
		return ERROR_SOCKET;
	}

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

void subscribir_y_escuchar_cola(t_conexion* args) {

	pthread_mutex_lock(&mutex_subscripcion);
	int estado_subscripcion = subscribir(args->server, args->cliente);
	pthread_mutex_unlock(&mutex_subscripcion);

	//TO-DO reconectar
	if (error_conexion(estado_subscripcion))
		return;

	t_conexion_cliente* cliente = args->cliente;
	t_subscriptor* subscriptor = cliente->subscriptor;

	while (1) {

		if (error_conexion(recibir(subscriptor->socket, cliente->callback))) {

			log_warning(logger, "Se desconectó el %s, cancelando escucha sobre la cola %s",
					BROKER_STRING, get_nombre_cola(cliente->id_cola));
			break;
		};

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
int subscribir(t_conexion_server* server, t_conexion_cliente* cliente) {

	cliente->subscriptor->socket = socket_crear_client(server->ip, server->puerto);

	//TO-DO reconectar
	if (error_conexion(cliente->subscriptor->socket)) {
		log_warning(logger,
				"el %s está desconectado, cancelando subscripción %s",
				BROKER_STRING, get_nombre_cola(cliente->id_cola));
		return ERROR_SOCKET;
	}

	t_paquete_header pqt = paquete_header_crear(SUBSCRIPCION,
			server->id_proceso, cliente->id_cola);

	cliente->subscriptor->id_subcriptor = handshake(
			cliente->subscriptor->socket, &pqt, sizeof(t_paquete_header));

	return cliente->subscriptor->id_subcriptor;
}

static int recibir(int socket, void (*callback)(t_id_cola, void*)) {

	t_paquete_header header = socket_recibir_header(socket);

	if (error_conexion(header.codigo_operacion)) {
		return ERROR_SOCKET;
	}

	int size = 0;
	void* msj = socket_recibir_mensaje(socket, &size);

	if (error_conexion(size)) {
		return ERROR_SOCKET;
	}

	uint32_t ACK = 1;
	socket_send(socket, &ACK, sizeof(ACK));

//	t_buffer* bfr = buffer_crear(size);
//	buffer_set_stream(bfr, msj);
//
//	t_paquete* pqt = paquete_crear(header, bfr);
	
	callback(header.id_cola, msj);

//	paquete_destruir(pqt);

	return EXIT_SUCCESS;
}
