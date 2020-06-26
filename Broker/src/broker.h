#ifndef BROKER_H_
#define BROKER_H_

#include "colas.h"
#include "memoria/mmu.h"
#include "modos_procesamiento/procesar_mensaje.h"
#include "modos_procesamiento/procesar_suscripcion.h"

pthread_t hilo_receptor_mensajes;
int socket_servidor;

void atender_cliente(int* socket);
void inicializar();
void inicializar_config();
void inicializar_logs();
void inicializar_colas();
void inicializar_servidor();

#endif /* BROKER_H_ */
