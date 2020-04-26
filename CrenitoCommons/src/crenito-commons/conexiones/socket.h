/*
 * socket.h
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_SOCKET_H_
#define SRC_CRENITO_COMMONS_SOCKET_H_

#include <arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<string.h>
#include "../crenito-logger.h"
#include "paquete.h"

typedef enum {
	CLIENTE, SERVIDOR
} socket_type;

typedef struct {
	struct sockaddr_in addr; //IP &PUERTO
	socklen_t addrlen;
	int socket;
} t_cliente;


// Funciones de construcción de sockets
void socket_listen(int unSocket);
void socket_bind(int unSocket, struct addrinfo* info);
void socket_connect(int unSocket, struct addrinfo* info);
int socket_create(struct addrinfo* info);
void socket_configurar(char* ip, char* puerto, socket_type tipo, struct addrinfo **servinfo);


/*Crea un socket de escucha,usando las funciones de construcción de sockets
 Esta debería ser la función llamada por fuera de las crenito-commons */
int socket_crear_listener(char* ip, char* puerto);

t_cliente* socket_aceptar_conexion(int socket_servidor);

//recv
int socket_recibir_cod_operacion(int socket_cliente);
void* socket_recibir_mensaje(int socket_cliente, int *size);

#endif /* SRC_CRENITO_COMMONS_SOCKET_H_ */
