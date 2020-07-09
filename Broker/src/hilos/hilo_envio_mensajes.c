/*
 * hilo_envio_mensajes.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "hilo_envio_mensajes.h"

static void enviar_mensaje_a_suscriptor_y_finalizar_hilo(t_enviar_args* args);

void crear_hilo_y_enviar_mensaje_a_suscriptor(t_mensaje_cache* msj, t_suscriptor* suscriptor) {

	t_enviar_args* args = malloc(sizeof(t_enviar_args));
	args->msj_cache = msj;
	args->suscriptor = suscriptor;

	particion_actualizar_fecha_ultimo_acceso(args->msj_cache->particion);

	pthread_create(&hilo_enviar, NULL, (void*) enviar_mensaje_a_suscriptor_y_finalizar_hilo, args);
	pthread_detach(hilo_enviar);
}

void enviar_mensaje_a_suscriptor(t_enviar_args* args) {

	void* deserializado = restaurar_mensaje_desde_cache(args->msj_cache);
	t_id_cola id_cola = mensaje_cache_get_id_cola(args->msj_cache);
	t_paquete* pqt = paquete_crear(paquete_header_crear(MENSAJE, BROKER, id_cola, id_proceso),
			serializar(deserializado, id_cola));

	uint32_t ack = enviar_paquete(pqt, args->suscriptor->socket);

	mensaje_cache_agregar_suscriptor(args->msj_cache, args->suscriptor,
			conexion_exitosa(ack) ? CONFIRMADO : FALLIDO);

	paquete_destruir(pqt);

}

static void enviar_mensaje_a_suscriptor_y_finalizar_hilo(t_enviar_args* args) {

	enviar_mensaje_a_suscriptor(args);
	pthread_exit(NULL);
}
