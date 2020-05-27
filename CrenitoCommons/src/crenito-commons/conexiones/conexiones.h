/*
 * conexiones.h
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_
#define SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_

#include "paquete.h"
#include "socket.h"

typedef struct {
	char* ip;
	char* puerto;
	t_id_proceso id_proceso;
	int segundos_reconexion;
} t_conexion_server;

typedef struct {
	uint32_t id_subcriptor;
	uint32_t socket;
} t_conexion_cliente;

t_conexion_cliente subscribir_cola(t_conexion_server server, t_id_cola id_cola);
t_conexion_server conexion_server_crear(char* ip, char* puerto, t_id_proceso id_proceso, int segundos_reconexion);
t_conexion_cliente* conexion_cliente_crear(int socket, int id_subscriptor);

#endif /* SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_ */
