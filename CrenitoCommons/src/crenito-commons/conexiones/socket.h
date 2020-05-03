/*
 * socket.h
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_SOCKET_H_
#define SRC_CRENITO_COMMONS_SOCKET_H_

#include <arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include "../utils.h"
#include "paquete.h"

#define ERROR_SOCKET -1

typedef enum {
	CLIENTE, SERVIDOR
} socket_type;

typedef struct {
	struct sockaddr_in addr; //IP &PUERTO
	socklen_t addrlen;
	int socket;
} t_conexion;


// Funciones de construcción de sockets
void socket_listen(int unSocket);
void socket_bind(int unSocket, struct addrinfo* info);
void socket_connect(int unSocket, struct addrinfo* info);
int socket_create(struct addrinfo* info);
void socket_configurar(char* ip, char* puerto, socket_type tipo, struct addrinfo **servinfo);

//Funciones para ser llamadas por fuera de las commons:
void socket_send(int unSocket, void* mensaje, int bytes);
void socket_cerrar(int unSocket);
//Crea un socket de escucha,usando las funciones de construcción de sockets
int socket_crear_listener(char* ip, char* puerto);
//Crea un socket cliente y se conecta usando las funciones de construcción de sockets
int socket_crear_client(char* ip, char* puerto);

t_conexion* socket_aceptar_conexion(int socket_servidor);

//recibe un valor entero
int socket_recibir_int(int socket_cliente);

/* Primero recibe el tamañano de la variable y luego recibe su contenido
 * Devuelve en el puntero size la cantidad de bytes recibidos */
void* socket_recibir_mensaje(int socket_cliente, int *size);

#endif /* SRC_CRENITO_COMMONS_SOCKET_H_ */
