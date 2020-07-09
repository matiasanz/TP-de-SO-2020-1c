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
static int suscribir(t_conexion_server* server, t_conexion_cliente* cliente);
/*
 * Envía un paquete usando el socket indicado por parámetro
 * y se queda a la espera una respuesta de tipo entero
 * la cual retorna
 */
static int handshake(int socket, void* pqt, int size);

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

void suscribir_y_escuchar_cola(t_conexion* args) {

	pthread_mutex_lock(&mutex_subscripcion);
	int estado_subscripcion = suscribir(args->server, args->cliente);
	pthread_mutex_unlock(&mutex_subscripcion);

	//TO-DO reconectar
	if (error_conexion(estado_subscripcion))
		return;

	t_conexion_cliente* cliente = args->cliente;
	t_suscriptor* suscriptor = cliente->suscriptor;

	while (1) {

		if (error_conexion(recibir(suscriptor->socket, cliente->callback))) {

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
int suscribir(t_conexion_server* server, t_conexion_cliente* cliente) {

	cliente->suscriptor->socket = socket_crear_client(server->ip, server->puerto);

	//TO-DO reconectar
	if (error_conexion(cliente->suscriptor->socket)) {
		log_warning(logger,
				"el %s está desconectado, cancelando subscripción %s",
				BROKER_STRING, get_nombre_cola(cliente->id_cola));
		return ERROR_SOCKET;
	}

	t_paquete_header pqt = paquete_header_crear(SUSCRIPCION,
			server->tipo_proceso, cliente->id_cola, cliente ->suscriptor ->id_subcriptor);

	return handshake(cliente->suscriptor->socket, &pqt, sizeof(t_paquete_header));
}

int recibir(int socket, void (*callback)(t_id_cola, void*)) {

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

	callback(header.id_cola, msj);

	return EXIT_SUCCESS;
}
