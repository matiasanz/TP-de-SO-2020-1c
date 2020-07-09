/*
 * hilo_consolidar_cola.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "hilo_consolidacion_colas.h"

static void consolidar_colas(void* param);
static void consolidar_cola(t_cola_container* container);
static void reenviar(t_mensaje_cache* msj);

void inicializar_hilo_consolidacion_colas() {

	frecuencia_consolidacion_colas = config_get_int_value(config, "FRECUENCIA_CONSOLIDACION_COLAS");

	pthread_create(&hilo_consolidacion_colas, NULL, (void*) consolidar_colas, NULL);
	pthread_detach(hilo_consolidacion_colas);
}

static void consolidar_colas(void* param) {

	while (true) {

		sleep(frecuencia_consolidacion_colas);

		//TODO: Revisar esto para no hardcodear el id de las colas
		for (int i = 1; i <= 6; ++i) {
			t_cola_container* container = get_cola(i);

			pthread_mutex_lock(&container->mutex_mensajes);

			log_info(event_logger, "Inicio consolidacion cola %s , cantidad de mensajes: %d", get_nombre_cola(i),
					list_size(container->cola));
			// Si no hay mensajes no se debe consolidar
			if (!list_is_empty(container->cola)) {
				consolidar_cola(container);
			}

			log_info(event_logger, "Fin consolidacion cola %s , cantidad de mensajes: %d", get_nombre_cola(i),
					list_size(container->cola));
			pthread_mutex_unlock(&container->mutex_mensajes);
		}
	}
}

static void consolidar_cola(t_cola_container* container) {

	int cantidad_suscriptores = cola_get_cantidad_suscriptores(container);

	// Si no hay suscriptores no se debe consolidar
	if (cantidad_suscriptores == 0) {
		return;
	}

	for (int i = 0; i < list_size(container->cola); ++i) {

		t_mensaje_cache* msj = list_get(container->cola, i);

		if (todos_los_suscriptores_confirmaron(msj, cantidad_suscriptores)) {
			list_remove_and_destroy_element(container->cola, i, (void*) mensaje_cache_eliminar_de_cola);

		} else {
			reenviar(msj);
		}
	}
}

static void reenviar(t_mensaje_cache* msj) {

	pthread_mutex_lock(&msj->mutex_edicion_mensaje);
	int cantidad_destinatarios = list_size(mensaje_cache_get_suscriptores(msj, FALLIDO));
	pthread_mutex_unlock(&msj->mutex_edicion_mensaje);

	if (cantidad_destinatarios == 0)
		return;

	int index = 0;
	while (index < cantidad_destinatarios) {

		pthread_mutex_lock(&msj->mutex_edicion_mensaje);
		t_suscriptor* suscriptor = list_remove(mensaje_cache_get_suscriptores(msj, FALLIDO), index);
		pthread_mutex_unlock(&msj->mutex_edicion_mensaje);

		crear_hilo_y_enviar_mensaje_a_suscriptor(msj, suscriptor);
		suscriptor_destruir(suscriptor);
		index++;
	}
}
