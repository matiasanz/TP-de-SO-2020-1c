/*
 * socket.c
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#include "socket.h"

void static _error_bind_connect(int socket, char* operacion);

int cr_crear_socket(char* ip, char* puerto, socket_type tipo) {
	int unSocket;
	struct addrinfo hints, *servinfo, *info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (tipo == SERVIDOR)
		hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &servinfo);

	for (info = servinfo; info != NULL; info = info->ai_next) {

		if ((unSocket = socket(info->ai_family, info->ai_socktype,
				info->ai_protocol)) == -1) {
			log_error(get_crnito_logger(), "Error al crear el socket");
			exit(-1);
			continue;
		}
		if (tipo == SERVIDOR) {
			if (bind(unSocket, info->ai_addr, info->ai_addrlen) < 0) {
				_error_bind_connect(unSocket, "bind");
				continue;
			}
		} else {
			if (connect(unSocket, info->ai_addr, info->ai_addrlen) < 0) {
				_error_bind_connect(unSocket, "connect");
				continue;
			}
		}
		break;
	}

	if (tipo == SERVIDOR)
		listen(unSocket, SOMAXCONN);

	freeaddrinfo(servinfo);

	return unSocket;

}

int cr_aceptar_conexion(int socket_servidor) {
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);
	int socket_cliente;

	if ((socket_cliente = accept(socket_servidor, (void*) &dir_cliente,
			&tam_direccion)) < 0) {
		log_error(get_crnito_logger(),
				"Error al realizar el accept, socket_cliente: %d, socket_servidor: %d",
				socket_cliente, socket_servidor);

	}

	return socket_cliente;
}

int cr_recibir_cod_operacion(int socket_cliente) {

	int cod_op;
	if (recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) == -1) {
		cod_op = -1;
	}

	return cod_op;
}

void* cr_recibir_mensaje(int socket_cliente, int *size) {
	void * buffer;

	if (recv(socket_cliente, size, sizeof(int), MSG_WAITALL) < 0) {
		log_error(get_crnito_logger(),
				"Error al realizar el recv del size, socket_cliente: %d",
				socket_cliente);
	}

	buffer = malloc(*size);

	if (recv(socket_cliente, buffer, *size, MSG_WAITALL) < 0) {
		log_error(get_crnito_logger(),
				"Error al realizar el recv del mensaje, socket_cliente: %d",
				socket_cliente);
	}

	return buffer;
}

void static _error_bind_connect(int socket, char* operacion) {

	log_error(get_crnito_logger(),
			"Error al realizar la operación %s, socket: %d", operacion, socket);
	close(socket);

}