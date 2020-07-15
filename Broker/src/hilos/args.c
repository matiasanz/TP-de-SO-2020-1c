/*
 * enviar_args.c
 *
 *  Created on: 10 jul. 2020
 *      Author: utnso
 */

#include "args.h"

t_enviar_mensaje_args* enviar_mensaje_args_crear(t_mensaje_cache* msj, t_suscriptor* suscriptor, t_mensaje_header* header, void* mensaje_deserializado, t_id_cola id_cola) {

	t_enviar_mensaje_args* args = malloc(sizeof(t_enviar_mensaje_args));

	args -> header = header;
	args->mensaje_deserializado = mensaje_deserializado;
	args -> id_cola = id_cola;
	args -> mensaje_cache = msj;
	args->suscriptor = suscriptor;

	return args;
}

void enviar_mensaje_args_destruir(t_enviar_mensaje_args* args) {

	//TODO: ver este free
	//mensaje_destruir(args ->mensaje_deserializado, args -> id_cola);
	free(args);
}

t_enviar_id_mensaje_args* enviar_id_mensaje_args_crear(uint32_t id_mensaje, int* id_socket, t_paquete_header header) {

	t_enviar_id_mensaje_args* args = malloc(sizeof(t_enviar_id_mensaje_args));

	args->id_mensaje = id_mensaje;
	args->id_socket = id_socket;
	args->header = header;

	return args;
}

void enviar_id_mensaje_args_destruir(t_enviar_id_mensaje_args* args) {

	free(args -> id_socket);
	free(args);
}
