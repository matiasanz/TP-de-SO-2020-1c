/*
 * enviar_args.h
 *
 *  Created on: 10 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_HILOS_ENVIAR_ARGS_H_
#define SRC_HILOS_ENVIAR_ARGS_H_

#include "../utils/broker_utils.h"
#include "../colas.h"
#include "../memoria/mmu.h"

typedef struct {
	t_mensaje_header* header;
	void* mensaje_deserializado;
	t_id_cola id_cola;
	t_suscriptor* suscriptor;
	t_mensaje_cache* mensaje_cache;
} t_enviar_mensaje_args;

typedef struct {
	uint32_t id_mensaje;
	int* id_socket;
	t_paquete_header header;
} t_enviar_id_mensaje_args;

t_enviar_mensaje_args* enviar_mensaje_args_crear(t_mensaje_cache* msj, t_suscriptor* suscriptor, t_mensaje_header* header, void* mensaje_deserializado, t_id_cola id_cola);
void enviar_mensaje_args_destruir(t_enviar_mensaje_args* args);

t_enviar_id_mensaje_args* enviar_id_mensaje_args_crear(uint32_t id_mensaje, int* id_socket, t_paquete_header header);
void enviar_id_mensaje_args_destruir(t_enviar_id_mensaje_args* args);

#endif /* SRC_HILOS_ENVIAR_ARGS_H_ */
