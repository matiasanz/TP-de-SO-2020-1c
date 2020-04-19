/*
 * conexiones.h
 *
 *  Created on: 3 mar. 2019
 *      Author: utnso
 */

#ifndef UTILIDADESBROKER_H_
#define UTILIDADESBROKER_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<string.h>
#include<pthread.h>

typedef enum {
	BROKER = 1, GAMEBOY = 2, GAMECARD = 3, TEAM = 4

} op_code;

typedef struct {
	int size;
	void* stream;
} t_buffer;

typedef struct {
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

pthread_t thread;
t_log* logger;

void* recibir_buffer(int*, int);
void iniciar_servidor(char* ip, char* puerto);
void esperar_cliente(int);
void* recibir_mensaje(int socket_cliente, int* size);
int recibir_operacion(int);
void process_request(int cod_op, int cliente_fd);
void serve_client(int *socket);
void* serializar_paquete(t_paquete* paquete, int bytes);
void devolver_mensaje(int cod_op, void* payload, int size, int socket_cliente);
void procesarGameBoy(int socket_cliente);
void procesarGameCard(int socket_cliente);
void procesarTeam(int socket_cliente);
void handover(int cod_op, int socket_cliente);

#endif /* UTILIDADESBROKER_H_ */
