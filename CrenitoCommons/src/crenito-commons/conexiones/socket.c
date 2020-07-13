/*
 * socket.c
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#include "socket.h"

// Funciones auxiliares
void static manejar_error_socket(int socket, char* operacion);
int static error_recibir(int recv_status);

int socket_bind(int socket, struct addrinfo* info) {

	if (error_conexion(bind(socket, info->ai_addr, info->ai_addrlen))) {
		manejar_error_socket(socket, "bind");
		return ERROR_SOCKET;
	}

	return socket;
}

int socket_connect(int socket, struct addrinfo* info) {

	if (error_conexion(connect(socket, info->ai_addr, info->ai_addrlen))) {
		manejar_error_socket(socket, "connect");
		socket = ERROR_SOCKET;
	}

	freeaddrinfo(info);

	return socket;
}

int socket_create(struct addrinfo* info) {

	int un_socket = socket(info->ai_family, info->ai_socktype,
			info->ai_protocol);

	if (error_conexion(un_socket)) {
		manejar_error_socket(un_socket, "create");
	}

	//Validar esto con ayudantes. Hay que confirgurarlo bloqueante o no?
	int yes = 1;
	if (error_conexion(setsockopt(un_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)))) {
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

	if (error_conexion(listen(socket, SOMAXCONN))) {
		manejar_error_socket(socket, "listen");
	}
}

int socket_crear_listener(char* ip, char* puerto) {

	struct addrinfo *servinfo;
	socket_configurar(ip, puerto, SERVIDOR, &servinfo);
	int socket = socket_create(servinfo);

	if(error_conexion(socket_bind(socket, servinfo))) {
		freeaddrinfo(servinfo);
		return ERROR_SOCKET;
	}
	socket_listen(socket);

	freeaddrinfo(servinfo);

	return socket;
}

void socket_send(int socket, void* mensaje, int bytes) {

	if (error_conexion(send(socket, mensaje, bytes, MSG_WAITALL))) {
		manejar_error_socket(socket, "send");
	}
}

void socket_cerrar(int socket) {

	if (error_conexion(close(socket))) {
		manejar_error_socket(socket, "close");
	}
}

int socket_crear_client(char* ip, char* puerto) {

	struct addrinfo *servinfo;
	socket_configurar(ip, puerto, CLIENTE, &servinfo);
	int socket = socket_create(servinfo);

	if (error_conexion(socket_connect(socket, servinfo))) {
		return ERROR_SOCKET;
	}

	return socket;
}

int socket_aceptar_conexion(int socket_servidor) {

	struct sockaddr_in dir_cliente;
	int tam_direccion = sizeof(struct sockaddr_in);
	struct sockaddr_in direccion;
	int socket = accept(socket_servidor, (struct sockaddr *) &dir_cliente,
			(socklen_t*) &tam_direccion);

	if (error_conexion(socket)) {
		manejar_error_socket(socket, "accept");
		socket = ERROR_SOCKET;
	}

	return socket;
}

int socket_recibir_int(int socket_cliente) {

	int entero;

	if (error_recibir(recv(socket_cliente, &entero, sizeof(int), MSG_WAITALL))) {
		manejar_error_socket(socket_cliente, "recv");
		entero = ERROR_SOCKET;
	}

	return entero;
}

void* socket_recibir_mensaje(int socket_cliente, int *size) {

	*size = socket_recibir_int(socket_cliente);

	if (error_conexion(*size)) {
		manejar_error_socket(socket_cliente, "recv");
		*size = ERROR_SOCKET;
		return NULL;
	}

	void* msj = malloc(*size);

	if (error_recibir(recv(socket_cliente, msj, *size, MSG_WAITALL))) {
		manejar_error_socket(socket_cliente, "recv");
		*size = ERROR_SOCKET;
		return NULL;
	}

	return msj;
}

t_paquete_header socket_recibir_header(int socket_cliente) {

	t_paquete_header header;

	if (error_recibir(recv(socket_cliente, &header, sizeof(t_paquete_header), MSG_WAITALL))) {
		manejar_error_socket(socket_cliente, "recv");
		header.codigo_operacion = ERROR_SOCKET;
	}

	return header;
}

int static error_recibir(int recv_status) {
	return recv_status <= 0;
}

void static manejar_error_socket(int socket, char* operacion) {

	log_warning_socket(socket, operacion);
	close(socket);
}
