#ifndef BROKER_H_
#define BROKER_H_

#include "utilidades-broker.h"

void atender_cliente(int* socket);
void inicializar();
void inicializar_config();
void inicializar_logs();
void crear_colas();

#endif /* BROKER_H_ */
