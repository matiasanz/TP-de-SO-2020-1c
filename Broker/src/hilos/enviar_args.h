/*
 * enviar_args.h
 *
 *  Created on: 10 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_HILOS_ENVIAR_ARGS_H_
#define SRC_HILOS_ENVIAR_ARGS_H_

#include "../broker_utils.h"
#include "../colas.h"
#include "../memoria/mmu.h"

typedef struct {
	t_mensaje_cache* msj_cache;
	t_suscriptor* suscriptor;
} t_enviar_args;

t_enviar_args* enviar_args_crear(t_mensaje_cache* msj, t_suscriptor* suscriptor);
void enviar_args_destruir(t_enviar_args* args);

#endif /* SRC_HILOS_ENVIAR_ARGS_H_ */
