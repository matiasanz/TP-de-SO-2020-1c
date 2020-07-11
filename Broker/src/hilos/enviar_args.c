/*
 * enviar_args.c
 *
 *  Created on: 10 jul. 2020
 *      Author: utnso
 */

#include "enviar_args.h"

t_enviar_mensaje_args* enviar_mensaje_args_crear(t_mensaje_cache* msj, t_suscriptor* suscriptor) {

	t_enviar_mensaje_args* args = malloc(sizeof(t_enviar_mensaje_args));

	args->msj_cache = msj;
	args->suscriptor = suscriptor;

	return args;
}

void enviar_mensaje_args_destruir(t_enviar_mensaje_args* args) {

	suscriptor_destruir(args->suscriptor);
	free(args);
}

t_enviar_id_mensaje_args* enviar_id_mensaje_args_crear(uint32_t id_mensaje, int id_socket, t_paquete_header header) {

	t_enviar_id_mensaje_args* args = malloc(sizeof(t_enviar_id_mensaje_args));

	args->id_mensaje = id_mensaje;
	args->id_socket = id_socket;
	args -> header = header;

	return args;
}

void enviar_id_mensaje_args_destruir(t_enviar_id_mensaje_args* args) {
	free(args);
}
