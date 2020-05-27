/*
 * socket.c
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#include "socket.h"

// funciones auxiliares
void static manejar_error_socket(int socket, char* operacion);
int static socket_error(int indicador_conexion);

void socket_bind(int socket, struct addrinfo* info) {

	if (bind(socket, info->ai_addr, info->ai_addrlen) == ERROR_SOCKET) {
		manejar_error_socket(socket, "bind");

	}
}

void socket_connect(int socket, struct addrinfo* info) {

	if (connect(socket, info->ai_addr, info->ai_addrlen) == ERROR_SOCKET) {
		manejar_error_socket(socket, "connect");
	}
}

int socket_create(struct addrinfo* info) {

	int un_socket;

	if ((un_socket = socket(info->ai_family, info->ai_socktype, info->ai_protocol))
			== ERROR_SOCKET) {
		manejar_error_socket(un_socket, "create");
	}

	//Validar esto con ayudantes. Hay que confirgurarlo bloqueante o no?
	int yes = 1;
	if (setsockopt(un_socket, SOL_SOCKET, SO_REUSEADDR, &yes,
			sizeof(int)) == ERROR_SOCKET) {
		manejar_error_socket(un_socket, "setsockopt");
	}

	return un_socket;
}

void socket_configurar(char* ip, char* puerto, socket_type tipo,
		struct addrinfo **servinfo) {

	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (tipo == SERVIDOR)
		hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, servinfo);
}

void socket_listen(int socket) {

	if (listen(socket, SOMAXCONN) == ERROR_SOCKET) {
		manejar_error_socket(socket, "listen");
	}
}

int socket_crear_listener(char* ip, char* puerto) {

	struct addrinfo *servinfo;
	socket_configurar(ip, puerto, SERVIDOR, &servinfo);
	int socket = socket_create(servinfo);
	socket_bind(socket, servinfo);
	socket_listen(socket);

	freeaddrinfo(servinfo);

	return socket;
}

void socket_send(int socket, void* mensaje, int bytes) {

	if (send(socket, mensaje, bytes, MSG_WAITALL) == ERROR_SOCKET) {
		manejar_error_socket(socket, "send");
	}
}

void socket_cerrar(int socket) {

	if (close(socket) == ERROR_SOCKET) {
		manejar_error_socket(socket, "close");
	}
}

int socket_crear_client(char* ip, char* puerto) {

	struct addrinfo *servinfo;
	socket_configurar(ip, puerto, CLIENTE, &servinfo);
	int socket = socket_create(servinfo);
	socket_connect(socket, servinfo);

	freeaddrinfo(servinfo);

	return socket;
}

int socket_aceptar_conexion(int socket_servidor) {

	struct sockaddr_in dir_cliente;
	int tam_direccion = sizeof(struct sockaddr_in);
	struct sockaddr_in direccion;
	int socket;

	if ((socket = accept(socket_servidor, (struct sockaddr *) &dir_cliente,
			(socklen_t*) &tam_direccion)) == ERROR_SOCKET) {
		manejar_error_socket(socket, "accept");
		socket = ERROR_SOCKET;
	}

	return socket;
}

int socket_recibir_int(int socket_cliente) {

	int entero;

	if (recv(socket_cliente, &entero, sizeof(int), MSG_WAITALL) == ERROR_SOCKET) {
		manejar_error_socket(socket_cliente, "recv");
		entero = ERROR_SOCKET;
	}

	return entero;
}

void* socket_recibir_mensaje(int socket_cliente, int *size) {

	*size = socket_recibir_int(socket_cliente);

	if (socket_error(*size)){
		manejar_error_socket(socket_cliente, "recv");
		return NULL;
	}

	void* msj = malloc(*size);

	if (recv(socket_cliente, msj, *size, MSG_WAITALL) == ERROR_SOCKET) {
		manejar_error_socket(socket_cliente, "recv");
		return NULL;
	}

	return msj;
}

t_paquete_header socket_recibir_header(int socket_cliente) {

	t_paquete_header header;

	if (recv(socket_cliente, &header, sizeof(t_paquete_header),
			MSG_WAITALL) == ERROR_SOCKET) {
		manejar_error_socket(socket_cliente, "recv");
		header.codigo_operacion = ERROR_SOCKET;
	}

	return header;
}

int static socket_error(int indicador_conexion) {
	return indicador_conexion == ERROR_SOCKET;
}

void static manejar_error_socket(int socket, char* operacion) {

	log_error(get_crnito_logger(),
			"Error al realizar la operación %s, socket: %d", operacion, socket);
	close(socket);
}
