/*
 * procesar_subscripcion.c
 *
 *  Created on: 14 jun. 2020
 *      Author: utnso
 */

#include "procesar_suscripcion.h"

//Funciones privadas
static t_suscriptor* suscribir_proceso(int socket, t_cola_container* container);
static void enviar_mensajes_cacheados(t_cola_container* container, t_suscriptor* suscriptor);
void enviar_mensaje_a_suscriptor(t_mensaje_cache* msj_cache, t_suscriptor* suscriptor);

void procesar_suscripcion(int socket, t_paquete_header header) {

	t_cola_container* container = get_cola(header.id_cola);
	t_suscriptor* suscriptor = suscribir_proceso(socket, container);

	uint32_t id_suscriptor = suscriptor->id_subcriptor;
	log_info(logger, "Un proceso %s se suscribió a la cola %s. El id_subscriptor generado es: %d \n",
			get_nombre_proceso(header.id_proceso), get_nombre_cola(header.id_cola), id_suscriptor);

	socket_send(socket, &id_suscriptor, sizeof(id_suscriptor));

	enviar_mensajes_cacheados(container, suscriptor);
}

static t_suscriptor* suscribir_proceso(int socket, t_cola_container* container) {

	pthread_mutex_lock(&container->mutex);
	/*TODO en lugar de generar el id de suscriptor deberia recibirse
	 * y estar definido en el config file de cada proceso */
	t_suscriptor* suscriptor = suscriptor_crear(socket, list_size(container->suscriptores) + 1);
	list_add(container->suscriptores, suscriptor);
	pthread_mutex_unlock(&container->mutex);

	return suscriptor;
}

static void enviar_mensajes_cacheados(t_cola_container* container, t_suscriptor* suscriptor) {

	//TODO sincronizar correctamente
	pthread_mutex_lock(&container->mutex);
	for (int i = 0; i < list_size(container->cola); ++i) {

		t_mensaje_cache* msj = list_get(container->cola, i);

		//TODO: hay que usar multihreading
		if (particion_en_uso(msj->particion)) {
			particion_actualizar_acceso(msj->particion);
			enviar_mensaje_a_suscriptor(msj, suscriptor);
		}
	}
	pthread_mutex_unlock(&container->mutex);
}

void enviar_mensaje_a_suscriptor(t_mensaje_cache* msj_cache, t_suscriptor* suscriptor) {

	//TODO: crear un hilo por cada llamado a la funcion enviar
	void* deserializado = restaurar_mensaje_desde_cache(msj_cache);
	t_id_cola  id_cola = mensaje_cache_get_id_cola(msj_cache);
	t_paquete* pqt = paquete_crear(paquete_header_crear(MENSAJE, BROKER, id_cola), serializar(deserializado, id_cola));

	//TODO: registrar correctamente envio + ACK
	uint32_t ack = enviar_paquete(pqt, suscriptor->socket);

	mensaje_cache_set_suscriptor_enviado(msj_cache, suscriptor);

	if (conexion_exitosa(ack)) {
		mensaje_cache_set_suscriptor_confirmado(msj_cache, suscriptor);
	}
	//TODO: si no confirman, hay que avisar en otro hilo para que reintente.

	free(msj_cache);
	paquete_destruir(pqt);

}
