/*
 * hilo_consolidar_cola.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "hilo_consolidacion_colas.h"

static bool debe_consolidar_cola(t_cola_container* container, int* cantidad_suscriptores);
static void consolidar_colas(void* param);
static void consolidar_cola(t_cola_container* container);
static void restaurar_contenido_y_reenviar(t_mensaje_cache* msj, t_id_cola id_cola);

void inicializar_hilo_consolidacion_colas() {

	frecuencia_consolidacion_colas = config_get_int_value(config, "FRECUENCIA_CONSOLIDACION_COLAS");

	pthread_create(&hilo_consolidacion_colas, NULL, (void*) consolidar_colas, NULL);
	pthread_detach(hilo_consolidacion_colas);
}

static bool debe_consolidar_cola(t_cola_container* container, int* cantidad_suscriptores) {

	// Si no hay mensajes en la cola no se debe consolidar
	if (cola_esta_vacia(container))
		return false;

	// Si no hay suscriptores no se debe consolidar
	*cantidad_suscriptores = cola_get_cantidad_suscriptores(container);
	return *cantidad_suscriptores > 0;
}

static void consolidar_colas(void* param) {

	while (true) {

		sleep(frecuencia_consolidacion_colas);

		void consolidar(t_id_cola* id_cola) {

			t_cola_container* container = get_cola(*id_cola);
			
			pthread_mutex_lock(&container->mutex_mensajes);
			log_event_inicio_consolidacion_colas(*id_cola, list_size(container->cola));
			consolidar_cola(container);
			pthread_mutex_unlock(&container->mutex_mensajes);
		}

		pthread_mutex_lock(&mutex_escritura_eliminacion_memoria);
		list_iterate(id_colas, (void*) consolidar);
		pthread_mutex_unlock(&mutex_escritura_eliminacion_memoria);

	}
}

static void consolidar_cola(t_cola_container* container) {

	int cantidad_suscriptores = 0;
	if (!debe_consolidar_cola(container, &cantidad_suscriptores)) return;

	t_list* mensajes_confirmados = list_create();
	t_list* mensajes_a_reenviar = list_create();

	void revisar_cola(t_mensaje_cache * msj) {

		int* id_mensaje = malloc(sizeof(int));

		if (todos_los_suscriptores_confirmaron(msj, cantidad_suscriptores)) {
			*id_mensaje = mensaje_cache_get_id(msj);
			list_add(mensajes_confirmados, id_mensaje);
		} else {
			list_add(mensajes_a_reenviar, msj);
		}
	}

	list_iterate(container->cola, (void*) revisar_cola);
	cola_buscar_y_eliminar_mensajes(mensajes_confirmados, container);

	void restaurar_y_enviar(t_mensaje_cache* msj) {
		restaurar_contenido_y_reenviar(msj, cola_get_id(container));
	}

	list_iterate(mensajes_a_reenviar, (void*) restaurar_contenido_y_reenviar);
}

static void restaurar_contenido_y_reenviar(t_mensaje_cache* msj, t_id_cola id_cola) {

	int cantidad_suscriptores_fallidos = mensaje_cache_get_cantidad_suscriptores_fallidos(msj);
	//Si no hay destinatarios pendientes no se debe reenviar
	if (cantidad_suscriptores_fallidos == 0)
		return;

	int index = 0;
	while (index < cantidad_suscriptores_fallidos) {

		t_suscriptor* suscriptor = mensaje_cache_get_primer_sucriptor_fallido(msj);

		//Debug
		//restaurar_y_enviar_mensaje_a_suscriptor(msj, suscriptor);
		crear_hilo_restaurar_y_enviar_mensaje_a_suscriptor(msj, suscriptor, id_cola);
		index++;
	}
}

