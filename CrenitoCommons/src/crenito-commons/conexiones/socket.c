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

	if (bind(unSocket, info->ai_addr, info->ai_addrlen) < 0) {
		manejar_error_socket(unSocket, "bind");

	}
}

void socket_connect(int unSocket, struct addrinfo* info) {

	if (connect(unSocket, info->ai_addr, info->ai_addrlen) < 0) {
		manejar_error_socket(unSocket, "connect");
	}
}

int socket_create(struct addrinfo* info) {

	int unSocket;

	if ((unSocket = socket(info->ai_family, info->ai_socktype,
			info->ai_protocol)) < 0) {
		manejar_error_socket(unSocket, "create");
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

	if (listen(unSocket, SOMAXCONN) < 0) {
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

t_cliente* socket_aceptar_conexion(int socket_servidor) {

	t_cliente* cliente = malloc(sizeof(t_cliente));
	cliente->addrlen = sizeof(struct sockaddr_in);

	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	struct sockaddr_in direccion;
	socklen_t addrlen;

	if ((cliente->socket = accept(socket_servidor,
			(struct sockaddr *) &cliente->addr, (socklen_t*) &cliente->addrlen))
			< 0) {
		log_error(get_crnito_logger(),
				"Error al realizar el accept, socket_cliente: %d, socket_servidor: %d",
				cliente->socket, socket_servidor);

	}

	printf("Nueva conexión , socket %d , ip is : %s , puerto : %d \n",
			cliente->socket, inet_ntoa(cliente->addr.sin_addr),
			ntohs(cliente->addr.sin_port));

	return cliente;
}

int socket_recibir_cod_operacion(int socket_cliente) {

	int cod_op;
	if (recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) == -1) {
		cod_op = -1;
	}

	return cod_op;
}

void* socket_recibir_mensaje(int socket_cliente, int *size) {

	void * bytes;

	if (recv(socket_cliente, size, sizeof(int), MSG_WAITALL) < 0) {
		log_error(get_crnito_logger(),
				"Error al realizar el recv del size, socket_cliente: %d",
				socket_cliente);
	}

	bytes = malloc(*size);

	if (recv(socket_cliente, bytes, *size, MSG_WAITALL) < 0) {
		log_error(get_crnito_logger(),
				"Error al realizar el recv del mensaje, socket_cliente: %d",
				socket_cliente);
	}

	return bytes;
}

void static manejar_error_socket(int socket, char* operacion) {

	log_error(get_crnito_logger(),
			"Error al realizar la operación %s, socket: %d", operacion, socket);
	close(socket);
	abort();

}
