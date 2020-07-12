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
 * Algoritmo de reconexion frente a una eventual caida del Broker.
 * Devuelve el nuevo socket que pudo reconectar
 */
static int reconectar(t_conexion_server* server, t_conexion_cliente* cliente);
/*
 * Envía un paquete usando el socket indicado por parámetro
 * y se queda a la espera una respuesta de tipo entero
 * la cual retorna
 */
static int handshake(int socket, void* pqt, int size);
static int crear_paquete_suscripcion_y_realizar_handshake(int un_socket, t_id_cola id_cola);

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
	int socket_cliente = suscribir(args->server, args->cliente);
	pthread_mutex_unlock(&mutex_subscripcion);

	while (true) {
		if (error_conexion(recibir(socket_cliente, args->cliente->callback))) {
			socket_cliente = reconectar(args->server, args->cliente);
		};
	}
}

static int handshake(int socket, void* pqt, int size) {

	socket_send(socket, pqt, size);
	return socket_recibir_int(socket);
}

static int crear_paquete_suscripcion_y_realizar_handshake(int un_socket, t_id_cola id_cola) {

	if (proceso_debe_informar_suscripcion())
		log_suscripcion(id_cola);
	t_paquete_header pqt = paquete_header_crear(SUSCRIPCION, id_cola);
	return handshake(un_socket, &pqt, sizeof(t_paquete_header));
}

void conectar_y_escuchar_gameboy(t_conexion_host* gameboy) {

	int socket_server = socket_crear_listener(gameboy->ip, gameboy->puerto);

	while (true) {

		int socket_cliente = socket_aceptar_conexion(socket_server);
		recibir(socket_cliente, gameboy->callback);
	}

}

static int suscribir(t_conexion_server* server, t_conexion_cliente* cliente) {

	int socket_cliente = socket_crear_client(server->ip, server->puerto);

	if (error_conexion(socket_cliente)) {
		socket_cliente = reconectar(server, cliente);
	} else {
		crear_paquete_suscripcion_y_realizar_handshake(socket_cliente, cliente->id_cola);
	}

	return socket_cliente;
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

static int reconectar(t_conexion_server* server, t_conexion_cliente* cliente) {

	if (!debe_reconectar(cliente)) return ERROR_SOCKET;
	int un_socket = 0;

	do {
		log_inicio_proceso_reconexion(cliente->id_cola, cliente ->segundos_reconexion);
		sleep(cliente ->segundos_reconexion);
		un_socket = socket_crear_client(server->ip, server->puerto);
		log_resultado_proceso_reconexion(cliente->id_cola, error_conexion(un_socket) ? "Fallido" : "Exitoso");

	} while (error_conexion(un_socket));

	crear_paquete_suscripcion_y_realizar_handshake(un_socket, cliente->id_cola);
	return un_socket;
}
