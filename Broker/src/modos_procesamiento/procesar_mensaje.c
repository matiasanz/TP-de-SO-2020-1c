/*
 * procesar_mensaje.c
 *
 *  Created on: 14 jun. 2020
 *      Author: utnso
 */
#include "procesar_mensaje.h"

//Funciones privadas
static void replicar_mensaje(t_cola_container* container, t_mensaje_cache* msj, t_id_cola id_cola);

void procesar_mensaje(int socket, t_paquete_header header) {

	int size = 0;
	void* msj_recibido = socket_recibir_mensaje(socket, &size);

	t_mensaje_cache* msj_cache = guardar_en_memoria(msj_recibido, header.id_cola);

	t_cola_container* container = get_cola(header.id_cola);

	encolar_mensaje(container, msj_cache);

	//TODO sacar esto a una funcion
	int id_mensaje = mensaje_cache_get_id(msj_cache);
	socket_send(socket, &id_mensaje, sizeof(id_mensaje));

	replicar_mensaje(container, msj_cache, header.id_cola);
}

static void replicar_mensaje(t_cola_container* container, t_mensaje_cache* mensaje_cache, t_id_cola id_cola) {

	for (int i = 0; i < list_size(container->suscriptores); ++i) {

		t_suscriptor* suscriptor = list_get(container->suscriptores, i);
		//TODO sincronizar correctamente
		pthread_mutex_lock(&container ->mutex);
		enviar_mensaje_a_suscriptor(mensaje_cache, suscriptor);
		pthread_mutex_lock(&container -> mutex);
	}
}

