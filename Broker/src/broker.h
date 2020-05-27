#ifndef BROKER_H_
#define BROKER_H_

#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include "utilidades-broker.h"

t_config* config;

void atender_cliente(int* socket);
void inicializar();
void inicializar_config();
void inicializar_logs();
void crear_colas();

#endif /* BROKER_H_ */
