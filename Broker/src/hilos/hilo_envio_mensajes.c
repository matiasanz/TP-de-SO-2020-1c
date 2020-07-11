/*
 * hilo_envio_mensajes.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "hilo_envio_mensajes.h"

static void enviar_mensaje_a_suscriptor_y_finalizar_hilo(t_enviar_args* args);

void crear_hilo_y_enviar_mensaje_a_suscriptor(t_mensaje_cache* msj, t_suscriptor* suscriptor) {

	//TODO Verificar si al reenviar deberia actualizar este valor
	particion_actualizar_fecha_ultimo_acceso(msj->particion);

	pthread_create(&hilo_enviar, NULL, (void*) enviar_mensaje_a_suscriptor_y_finalizar_hilo, enviar_args_crear(msj, suscriptor));
	pthread_detach(hilo_enviar);
}

void enviar_mensaje_a_suscriptor(t_enviar_args* args) {

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

	enviar_args_destruir(args);
	paquete_destruir(pqt);
}

static void enviar_mensaje_a_suscriptor_y_finalizar_hilo(t_enviar_args* args) {

	enviar_mensaje_a_suscriptor(args);
	pthread_exit(NULL);
}
