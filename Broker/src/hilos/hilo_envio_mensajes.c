/*
 * hilo_envio_mensajes.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "hilo_envio_mensajes.h"

static void enviar_id_mensaje(t_enviar_id_mensaje_args* args);

void crear_hilo_y_enviar_mensaje_a_suscriptor(t_mensaje_cache* msj, t_suscriptor* suscriptor) {

	//TODO Verificar si al reenviar deberia actualizar este valor
	particion_actualizar_fecha_ultimo_acceso(msj->particion);

	pthread_create(&hilo_enviar, NULL, (void*) enviar_mensaje_a_suscriptor, enviar_mensaje_args_crear(msj, suscriptor));
	pthread_detach(hilo_enviar);
}

void enviar_mensaje_a_suscriptor(t_enviar_mensaje_args* args) {

	void* deserializado = restaurar_mensaje_desde_cache(args->msj_cache);
	t_id_cola id_cola = mensaje_cache_get_id_cola(args->msj_cache);
	t_paquete* pqt = paquete_crear(paquete_header_crear(MENSAJE, BROKER, id_cola, id_proceso), serializar(deserializado, id_cola));
	uint32_t id_mensaje = mensaje_cache_get_id(args->msj_cache);
	uint32_t id_suscriptor = suscriptor_get_id_proceso(args->suscriptor);

	log_envio_mensaje_a_suscriptor(id_mensaje, id_suscriptor);

	uint32_t ack = enviar_paquete(pqt, suscriptor_get_socket(args->suscriptor));

	if (conexion_exitosa(ack)) {
		log_confirmacion_mensaje_suscriptor(id_mensaje, id_suscriptor);
		mensaje_cache_agregar_suscriptor(args->msj_cache, args->suscriptor, CONFIRMADO);
	} else {
		log_warning_envio_mensaje(id_mensaje, id_suscriptor);
		mensaje_cache_agregar_suscriptor(args->msj_cache, args->suscriptor, FALLIDO);
	}

	enviar_mensaje_args_destruir(args);
	paquete_destruir(pqt);
}

void crear_hilo_y_enviar_id_univoco(uint32_t id_mensaje, int socket_id, t_paquete_header header) {

	pthread_create(&hilo_enviar, NULL, (void*) enviar_id_mensaje, enviar_id_mensaje_args_crear(id_mensaje, socket_id, header));
	pthread_detach(hilo_enviar);
}

static void enviar_id_mensaje(t_enviar_id_mensaje_args* args) {

	if(error_conexion(socket_send(args ->id_socket, &args ->id_mensaje, sizeof(uint32_t)))){
		log_error_enviar_id_mensaje(args ->id_mensaje, args -> header);
	}

	enviar_id_mensaje_args_destruir(args);
}
