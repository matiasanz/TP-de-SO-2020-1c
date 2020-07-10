/*
 * hilo_envio_mensajes.h
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_HILOS_HILO_ENVIO_MENSAJES_H_
#define SRC_HILOS_HILO_ENVIO_MENSAJES_H_

#include "enviar_args.h"

pthread_t hilo_enviar;

void crear_hilo_y_enviar_mensaje_a_suscriptor(t_mensaje_cache* msj, t_suscriptor* suscriptor);
void enviar_mensaje_a_suscriptor(t_enviar_args* args);

#endif /* SRC_HILOS_HILO_ENVIO_MENSAJES_H_ */
