/*
 * socket.h
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_SOCKET_H_
#define SRC_CRENITO_COMMONS_SOCKET_H_

#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include "paquete.h"

typedef enum {
	CLIENTE,
	SERVIDOR
} socket_type;

// Funciones de construcción de sockets
void socket_listen(int socket);
int socket_bind(int socket, struct addrinfo* info);
int socket_connect(int socket, struct addrinfo* info);
int socket_create(struct addrinfo* info);
void socket_configurar(char* ip, char* puerto, socket_type tipo, struct addrinfo **servinfo);

void socket_send(int socket, void* mensaje, int bytes);
void socket_cerrar(int socket);
//Crea un socket de escucha,usando las funciones de construcción de sockets
int socket_crear_listener(char* ip, char* puerto);
//Crea un socket cliente y se conecta usando las funciones de construcción de sockets
int socket_crear_client(char* ip, char* puerto);

int socket_aceptar_conexion(int socket_servidor);

//recibe un valor entero
int socket_recibir_int(int socket_cliente);

/* Primero recibe el tamañano de la variable y luego recibe su contenido
 * Devuelve en el puntero size la cantidad de bytes recibidos */
void* socket_recibir_mensaje(int socket_cliente, int *size);

t_paquete_header socket_recibir_header(int socket_cliente);

#endif /* SRC_CRENITO_COMMONS_SOCKET_H_ */
