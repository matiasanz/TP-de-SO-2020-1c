/*
 * hilo_consolidar_cola.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "hilo_consolidacion_colas.h"

static bool debe_consolidar_cola(t_cola_container* container, int* cantidad_suscriptores);
static void consolidar_colas(void* param);
static void consolidar_cola(t_cola_container* container, int cantidad_suscriptores, t_id_cola id_cola);
static void reenviar(t_mensaje_cache* msj);
static void eliminar(t_list* mensajes_confirmados, t_cola_container* container, t_id_cola id_cola);

void inicializar_hilo_consolidacion_colas() {

	frecuencia_consolidacion_colas = config_get_int_value(config, "FRECUENCIA_CONSOLIDACION_COLAS");

	pthread_create(&hilo_consolidacion_colas, NULL, (void*) consolidar_colas, NULL);
	pthread_detach(hilo_consolidacion_colas);
}

static bool debe_consolidar_cola(t_cola_container* container, int* cantidad_suscriptores) {

	// Si no hay mensajes en la cola no se debe consolidar
	if (list_is_empty(container->cola))
		return false;

	// Si no hay suscriptores no se debe consolidar
	*cantidad_suscriptores = cola_get_cantidad_suscriptores(container);
	return *cantidad_suscriptores > 0;
}

static void consolidar_colas(void* param) {

	while (true) {

		sleep(frecuencia_consolidacion_colas);

		pthread_mutex_lock(&mutex_acceso_memoria);
		//TODO: Revisar esto para no hardcodear el id de las colas
		for (int i = 1; i <= 6; ++i) {

			t_cola_container* container = get_cola(i);
			int cantidad_suscriptores = 0;

			log_event_inicio_consolidacion_colas(get_nombre_cola(i), list_size(container->cola));

			if (debe_consolidar_cola(container, &cantidad_suscriptores)) {
				consolidar_cola(container, cantidad_suscriptores, i);
			}
		}
		pthread_mutex_unlock(&mutex_acceso_memoria);
	}
}

static void consolidar_cola(t_cola_container* container, int cantidad_suscriptores, t_id_cola id_cola) {

	t_list* mensajes_confirmados = list_create();

	void revisar_cola(t_mensaje_cache * msj) {
		int* id_mensaje = malloc(sizeof(int));

		if (todos_los_suscriptores_confirmaron(msj, cantidad_suscriptores)) {

			*id_mensaje = mensaje_cache_get_id(msj);
			list_add(mensajes_confirmados, id_mensaje);
		} else {
			reenviar(msj);
		}
	}

	pthread_mutex_lock(&container->mutex_mensajes);
	t_list* copia_cola = list_duplicate(container->cola);
	pthread_mutex_unlock(&container->mutex_mensajes);

	list_iterate(copia_cola, (void*) revisar_cola);
	eliminar(mensajes_confirmados, container, id_cola);
}

static void reenviar(t_mensaje_cache* msj) {

	pthread_mutex_lock(&msj->mutex_edicion_mensaje);
	int cantidad_suscriptores_fallidos = list_size(mensaje_cache_get_suscriptores(msj, FALLIDO));
	pthread_mutex_unlock(&msj->mutex_edicion_mensaje);

	//Si no hay destinatarios pendientes no se debe reenviar
	if (cantidad_suscriptores_fallidos == 0)
		return;

	int index = 0;
	while (index < cantidad_suscriptores_fallidos) {

		pthread_mutex_lock(&msj->mutex_edicion_mensaje);
		t_suscriptor* suscriptor = list_remove(mensaje_cache_get_suscriptores(msj, FALLIDO), index);
		pthread_mutex_unlock(&msj->mutex_edicion_mensaje);

		crear_hilo_y_enviar_mensaje_a_suscriptor(msj, suscriptor);
		index++;
	}
}

static void eliminar(t_list* mensajes_confirmados, t_cola_container* container, t_id_cola id_cola) {

	void buscar_y_eliminar(uint32_t* id_mensaje) {
		log_event_consolidacion_cola_eliminacion(*id_mensaje, id_cola);
		cola_buscar_y_eliminar_mensaje(*id_mensaje, id_cola);
	}

	list_iterate(mensajes_confirmados, (void*) buscar_y_eliminar);
	//TODO: revisar si hay que destruir elementos
	list_destroy(mensajes_confirmados);
}
