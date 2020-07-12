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
static void eliminar_de_la_cola_y_destruir(t_list* mensajes_confirmados, t_cola_container* container, t_id_cola id_cola);

void inicializar_hilo_consolidacion_colas() {

	frecuencia_consolidacion_colas = config_get_int_value(config, "FRECUENCIA_CONSOLIDACION_COLAS");

	pthread_create(&hilo_consolidacion_colas, NULL, (void*) consolidar_colas, NULL);
	pthread_detach(hilo_consolidacion_colas);
}

static bool debe_consolidar_cola(t_cola_container* container, int* cantidad_suscriptores) {
	return !cola_esta_vacia(container) && cola_tiene_suscriptores(container);
}

static void consolidar_colas(void* param) {

	while (true) {

		sleep(frecuencia_consolidacion_colas);

//		pthread_mutex_lock(&mutex_acceso_memoria); //Sacar TODO
		//TODO: Revisar esto para no hardcodear el id de las colas;
		// Propuesta: for(t_id i=PRIMER_COLA; !esLaUltima(COLA); COLA = get_next(COLA)) (?
		for (int i = 1; i <= 6; ++i) {

			t_cola_container* container = get_cola(i);
			int cantidad_suscriptores = 0;

			pthread_mutex_lock(&container->mutex_mensajes); //Agrego
			log_inicio_consolidacion_colas(get_nombre_cola(i), list_size(container->cola));

			if (debe_consolidar_cola(container, &cantidad_suscriptores)) {
//				pthread_mutex_lock(&mutex_acceso_memoria); //Iria aca llegado el caso TODO
				consolidar_cola(container, cantidad_suscriptores, i);
//				pthread_mutex_unlock(&mutex_acceso_memoria); //Pero sigue siendo grande
			}

			pthread_mutex_unlock(&container->mutex_mensajes);

		}
//		pthread_mutex_unlock(&mutex_acceso_memoria); //Medio grande
	}
}

static void consolidar_cola(t_cola_container* container, int cantidad_suscriptores, t_id_cola id_cola) {

	t_list* mensajes_confirmados = list_create();

	void revisar_cola(t_mensaje_cache * msj) {

		if (todos_los_suscriptores_confirmaron(msj, cantidad_suscriptores)) {

			int* id_mensaje = malloc(sizeof(int));
			   * id_mensaje = mensaje_cache_get_id(msj);

			list_add(mensajes_confirmados, id_mensaje);
		}

		else reenviar(msj);
	}

//	pthread_mutex_lock(&container->mutex_mensajes);
//	t_list* copia_cola = list_duplicate(container->cola); //Ya no hace falta TODO
//	pthread_mutex_unlock(&container->mutex_mensajes);

	list_iterate( container->cola/*copia_cola*/, (void*) revisar_cola); //Trabajo con la misma cola
	eliminar_de_la_cola_y_destruir(mensajes_confirmados, container, id_cola);

	//TODO: revisar si hay que destruir elementos <--no deberia
	list_destroy(mensajes_confirmados);
}

int mensaje_cache_contar_destinatarios_pendientes(t_mensaje_cache* mensaje){
	pthread_mutex_lock(&mensaje->mutex_edicion_mensaje);
	int cantidad_suscriptores_fallidos = list_size(mensaje_cache_get_suscriptores(mensaje, FALLIDO));
	pthread_mutex_unlock(&mensaje->mutex_edicion_mensaje);

	return cantidad_suscriptores_fallidos;
}

static void reenviar(t_mensaje_cache* msj) {

	int cantidad_suscriptores_fallidos = mensaje_cache_contar_destinatarios_pendientes(msj);

	if (!cantidad_suscriptores_fallidos) {
		//Si no hay destinatarios pendientes no se debe reenviar
		return;
	}

	int index = 0;
	while (index < cantidad_suscriptores_fallidos) {

		pthread_mutex_lock(&msj->mutex_edicion_mensaje);
		t_suscriptor* suscriptor = list_remove(mensaje_cache_get_suscriptores(msj, FALLIDO), 0 /*index*/);
		pthread_mutex_unlock(&msj->mutex_edicion_mensaje);

		crear_hilo_y_enviar_mensaje_a_suscriptor(msj, suscriptor);

		index++;
	}
}

static void eliminar_de_la_cola_y_destruir(t_list* mensajes_confirmados, t_cola_container* container, t_id_cola id_cola) {

	void buscar_y_eliminar(uint32_t* id_mensaje) {
		log_consolidacion_cola_eliminacion(*id_mensaje, id_cola);
		//Con esta funcion no uso el mutex cola->mensajes y de paso aprovecho que ya tengo al container TODO
		cola_remove_and_destroy_by_id(*id_mensaje, container);
//		cola_buscar_y_eliminar_mensaje(*id_mensaje, id_cola);
	}

	//TODO validar: Segun veo, este es el unico lugar donde se afecta la memoria
	pthread_mutex_lock(&mutex_acceso_memoria);
	list_iterate(mensajes_confirmados, (void*) buscar_y_eliminar);
	pthread_mutex_unlock(&mutex_acceso_memoria);
}
