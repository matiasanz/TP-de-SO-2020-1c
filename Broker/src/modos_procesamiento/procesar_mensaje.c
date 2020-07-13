/*
 * procesar_mensaje.c
 *
 *  Created on: 14 jun. 2020
 *      Author: utnso
 */
#include "procesar_mensaje.h"

//Funciones privadas
static void replicar_mensaje(t_cola_container* container, t_mensaje_cache* msj);

void procesar_mensaje(int socket, t_paquete_header header) {

	int size = 0;
	void* msj_recibido = socket_recibir_mensaje(socket, &size);
	log_mensaje_recibido(header.id_cola, msj_recibido);


	t_mensaje_cache* msj_cache = guardar_en_memoria(msj_recibido, header.id_cola);

	t_cola_container* container = get_cola(header.id_cola);

	encolar_mensaje(container, msj_cache);

	//TODO sacar esto a una funcion
	//Observacion: al leer una particion, se esta jugando con el contenido de memoria
	pthread_mutex_lock(&mutex_acceso_memoria);
	int id_mensaje = mensaje_cache_get_id(msj_cache);
	pthread_mutex_unlock(&mutex_acceso_memoria);
	//TODO Dudoso, quizas convendria retornarlo como argumento en guardar_en_memoria()

	socket_send(socket, &id_mensaje, sizeof(id_mensaje));

	replicar_mensaje(container, msj_cache);
}

static void replicar_mensaje(t_cola_container* container, t_mensaje_cache* mensaje_cache) {

	void replicar(t_suscriptor* suscriptor) {
		crear_hilo_y_enviar_mensaje_a_suscriptor(mensaje_cache, suscriptor_duplicar(suscriptor));
	}

	pthread_mutex_lock(&container->mutex_suscriptores);
	list_iterate(container->suscriptores, (void*) replicar);
	pthread_mutex_unlock(&container->mutex_suscriptores);

}
