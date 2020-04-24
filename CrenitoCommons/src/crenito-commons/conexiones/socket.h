/*
 * socket.h
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_SOCKET_H_
#define SRC_CRENITO_COMMONS_SOCKET_H_

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

int cr_crear_socket(char* ip, char* puerto, socket_type tipo);
int cr_aceptar_conexion(int socket_servidor);

//recv
int cr_recibir_cod_operacion(int socket_cliente);
void* cr_recibir_mensaje(int socket_cliente, int *size);

#endif /* SRC_CRENITO_COMMONS_SOCKET_H_ */
