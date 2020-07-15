/*
 * hilo_envio_mensajes.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "hilo_envio_mensajes.h"

static void enviar_mensaje_a_suscriptor_desde_hilo(t_enviar_mensaje_args* args);

void restaurar_y_enviar_mensaje_a_suscriptor(t_mensaje_cache* msj, t_suscriptor* suscriptor, t_id_cola id_cola) {

	t_mensaje_header* msj_header = malloc(sizeof(t_mensaje_header));
	void* mensaje_deserializado = restaurar_mensaje_desde_cache(msj, msj_header);
	enviar_mensaje_a_suscriptor_desde_hilo(enviar_mensaje_args_crear(msj, suscriptor, msj_header, mensaje_deserializado, id_cola));
}

void crear_hilo_restaurar_y_enviar_mensaje_a_suscriptor(t_mensaje_cache* msj, t_suscriptor* suscriptor, t_id_cola id_cola) {

	t_mensaje_header* msj_header = malloc(sizeof(t_mensaje_header));
	void* mensaje_deserializado = restaurar_mensaje_desde_cache(msj, msj_header);

	pthread_create(&hilo_enviar, NULL, (void*) enviar_mensaje_a_suscriptor, enviar_mensaje_args_crear(msj, suscriptor, msj_header, mensaje_deserializado, id_cola));
	pthread_detach(hilo_enviar);
}

void crear_hilo_y_enviar_mensaje_a_suscriptor(t_mensaje_cache* msj, t_suscriptor* suscriptor, t_mensaje_header* msj_header, void* msj_deserializado, t_id_cola id_cola) {

	pthread_create(&hilo_enviar, NULL, (void*) enviar_mensaje_a_suscriptor_desde_hilo, enviar_mensaje_args_crear(msj, suscriptor, msj_header, msj_deserializado, id_cola));
	pthread_detach(hilo_enviar);
}

void enviar_mensaje_a_suscriptor(t_mensaje_cache* mensaje_cache, t_suscriptor* suscriptor, t_mensaje_header* msj_header, void* mensaje_deserializado, t_id_cola id_cola) {

	t_paquete* pqt = paquete_crear(MENSAJE, id_cola, serializar(mensaje_deserializado, id_cola));
	uint32_t ack = enviar_paquete(pqt, suscriptor_get_socket(suscriptor));
	mensaje_cache_set_ack(mensaje_cache, suscriptor, ack, msj_header -> id);

	paquete_destruir(pqt);
}

void enviar_mensaje_a_suscriptor_desde_hilo(t_enviar_mensaje_args* args) {

	enviar_mensaje_a_suscriptor(args ->mensaje_cache, args ->suscriptor, args ->header, args ->mensaje_deserializado, args -> id_cola);
	enviar_mensaje_args_destruir(args);
}

void crear_hilo_y_enviar_id_univoco(uint32_t id_mensaje, int* socket_id, t_paquete_header header) {

	pthread_create(&hilo_enviar, NULL, (void*) enviar_id_mensaje, enviar_id_mensaje_args_crear(id_mensaje, socket_id, header));
	pthread_detach(hilo_enviar);
}

void enviar_id_mensaje(t_enviar_id_mensaje_args* args) {

	int* un_socket = args->id_socket;
	if (error_conexion(socket_send(*un_socket, &args->id_mensaje, sizeof(uint32_t)))) {
		log_warning_enviar_id_mensaje(args->id_mensaje, paquete_header_get_proceso(args->header));
	}

	enviar_id_mensaje_args_destruir(args);
}
