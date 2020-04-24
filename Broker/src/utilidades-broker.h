#ifndef UTILIDADESBROKER_H_
#define UTILIDADESBROKER_H_

#include<commons/log.h>
#include<pthread.h>
#include <crenito-commons/conexiones/socket.h>

pthread_t thread;
t_log* logger;

void process_request(int cod_op, int cliente_fd);
void serve_client(int *socket);
void devolver_mensaje(int cod_op, t_buffer* buffer, int socket_cliente);
void handover(int cod_op, int socket_cliente);

#endif /* UTILIDADESBROKER_H_ */
