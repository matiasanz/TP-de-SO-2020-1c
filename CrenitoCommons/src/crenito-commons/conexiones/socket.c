/*
 * socket.c
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#include "socket.h"

// función auxilizar para loggear errores
void static manejar_error_socket(int socket, char* operacion);

void socket_bind(int unSocket, struct addrinfo* info) {

	if (bind(unSocket, info->ai_addr, info->ai_addrlen) == ERROR_SOCKET) {
		manejar_error_socket(unSocket, "bind");

	}
}

void socket_connect(int unSocket, struct addrinfo* info) {

	if (connect(unSocket, info->ai_addr, info->ai_addrlen) == ERROR_SOCKET) {
		manejar_error_socket(unSocket, "connect");
	}
}

int socket_create(struct addrinfo* info) {

	int unSocket;

	if ((unSocket = socket(info->ai_family, info->ai_socktype,
			info->ai_protocol)) == ERROR_SOCKET) {
		manejar_error_socket(unSocket, "create");
	}

	//Validar esto con ayudantes. Hay que confirgurarlo bloqueante o no?
	int yes = 1;
	if (setsockopt(unSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == ERROR_SOCKET) {
		manejar_error_socket(unSocket, "setsockopt");
	}

	return unSocket;
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

void socket_listen(int unSocket) {

	if (listen(unSocket, SOMAXCONN) == ERROR_SOCKET) {
		manejar_error_socket(unSocket, "listen");
	}
}

int socket_crear_listener(char* ip, char* puerto) {

	struct addrinfo *servinfo;
	socket_configurar(ip, puerto, SERVIDOR, &servinfo);
	int unSocket = socket_create(servinfo);
	socket_bind(unSocket, servinfo);
	socket_listen(unSocket);

	freeaddrinfo(servinfo);

	return unSocket;
}

void socket_send(int unSocket, void* mensaje, int bytes) {
	
	if (send(unSocket, mensaje, bytes, MSG_WAITALL) == ERROR_SOCKET) {
		manejar_error_socket(unSocket, "send");
	}
}

void socket_cerrar(int unSocket) {
	
	if (close(unSocket) == ERROR_SOCKET) {
		manejar_error_socket(unSocket, "close");
	}
}

int socket_crear_client(char* ip, char* puerto) {

	struct addrinfo *servinfo;
	socket_configurar(ip, puerto, CLIENTE, &servinfo);
	int unSocket = socket_create(servinfo);
	socket_connect(unSocket, servinfo);

	freeaddrinfo(servinfo);

	return unSocket;
}

t_conexion* socket_aceptar_conexion(int socket_servidor) {

	t_conexion* cliente = malloc(sizeof(t_conexion));
	cliente->addrlen = sizeof(struct sockaddr_in);

	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	struct sockaddr_in direccion;
	socklen_t addrlen;

	if ((cliente->socket = accept(socket_servidor,
			(struct sockaddr *) &cliente->addr, (socklen_t*) &cliente->addrlen))
			== ERROR_SOCKET) {
		manejar_error_socket(cliente -> socket, "accept");
	}

	log_info(get_crnito_logger(), "\n Nueva conexión aceptada, socket: %d , ip: %s , puerto : %d",
			cliente->socket, inet_ntoa(cliente->addr.sin_addr),
			ntohs(cliente->addr.sin_port));

	return cliente;
}

int socket_recibir_int(int socket_cliente) {

	int entero;
	
	if (recv(socket_cliente, &entero, sizeof(int), MSG_WAITALL) == ERROR_SOCKET) {
		manejar_error_socket(socket_cliente, "recv");
	}

	return entero;
}

void* socket_recibir_mensaje(int socket_cliente, int *size) {

	void * bytes;

	if (recv(socket_cliente, size, sizeof(int), MSG_WAITALL) == ERROR_SOCKET) {
		manejar_error_socket(socket_cliente, "recv");
	}

	bytes = malloc(*size);

	if (recv(socket_cliente, bytes, *size, MSG_WAITALL) == ERROR_SOCKET) {
		manejar_error_socket(socket_cliente, "recv");
	}

	return bytes;
}

void static manejar_error_socket(int socket, char* operacion) {

	log_error(get_crnito_logger(),
			"Error al realizar la operación %s, socket: %d", operacion, socket);
	close(socket);
	abort();

}
