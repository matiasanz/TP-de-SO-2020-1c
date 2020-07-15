/*
 * hilo_envio_mensajes.h
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_HILOS_HILO_ENVIO_MENSAJES_H_
#define SRC_HILOS_HILO_ENVIO_MENSAJES_H_

#include "args.h"

pthread_t hilo_enviar;

void crear_hilo_restaurar_y_enviar_mensaje_a_suscriptor(t_mensaje_cache* msj, t_suscriptor* suscriptor, t_id_cola id_cola);
void crear_hilo_y_enviar_mensaje_a_suscriptor(t_mensaje_cache* msj, t_suscriptor* suscriptor, t_mensaje_header* msj_header, void* msj_deserializado, t_id_cola id_cola);
void crear_hilo_y_enviar_id_univoco(uint32_t id_mensaje, int* socket_id, t_paquete_header header);

void restaurar_y_enviar_mensaje_a_suscriptor(t_mensaje_cache* msj, t_suscriptor* suscriptor, t_id_cola id_cola);
void enviar_mensaje_a_suscriptor(t_mensaje_cache* mensaje_cache, t_suscriptor* suscriptor, t_mensaje_header* msj_header, void* mensaje_deserializado, t_id_cola id_cola);
void enviar_id_mensaje(t_enviar_id_mensaje_args* args);

#endif /* SRC_HILOS_HILO_ENVIO_MENSAJES_H_ */
