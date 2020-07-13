/*
 * enviar_args.c
 *
 *  Created on: 10 jul. 2020
 *      Author: utnso
 */

#include "enviar_args.h"

t_enviar_args* enviar_args_crear(t_mensaje_cache* msj, t_suscriptor* suscriptor) {

	t_enviar_args* args = malloc(sizeof(t_enviar_args));

	args->msj_cache = msj;
	args->suscriptor = suscriptor;

	return args;
}

void enviar_args_destruir(t_enviar_args* args) {

	suscriptor_destruir(args->suscriptor);
	free(args);
}
