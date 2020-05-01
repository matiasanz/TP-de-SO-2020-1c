#ifndef BROKER_H_
#define BROKER_H_

#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include "utilidades-broker.h"

t_config* config;

void inicializar(void);
void inicializarColas();
#endif /* BROKER_H_ */
