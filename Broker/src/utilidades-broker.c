#include"utilidades-broker.h"

//Funciones privadas
static int subscribir_proceso(int socket, t_id_cola id_cola);
static t_cola_container* get_cola(t_id_cola id_cola);
static int encolar_mensaje(t_cola_container* container, void* msj);
static void replicar_mensaje(t_cola_container* container, void* deserializado,
		t_id_cola id_cola);

t_cola_container* cola_crear() {

	t_cola_container* cola = malloc(sizeof(t_cola_container));
	cola->cola = queue_create();
	cola->subscriptores = list_create();
	pthread_mutex_init(&cola->mutex, NULL);

	return cola;
}

void procesar_mensaje(int socket, t_paquete_header header) {

	int size = 0;

	void* msj = socket_recibir_mensaje(socket, &size);
	t_cola_container* container = get_cola(header.id_cola);

	void* deserializado = deserializar(msj, header.id_cola);

	int id_mensaje = encolar_mensaje(container, deserializado);

	socket_send(socket, &id_mensaje, sizeof(id_mensaje));

	replicar_mensaje(container, deserializado, header.id_cola);

	switch (header.id_cola) {
	case NEW_POKEMON:
		break;
	case APPEARED_POKEMON:

		break;
	case CATCH_POKEMON:
	case CAUGHT_POKEMON:
	case GET_POKEMON:
	case LOCALIZED_POKEMON: {

		break;
	}
	default:

		log_error(event_logger,
				"La cola destino %d enviada desde el socket: %d por un proceso %s es incorrecto. Finzalizando hilo \n",
				header.id_cola, socket, get_nombre_proceso(header.id_proceso));
		pthread_exit(NULL);
	}
}

void procesar_subscripcion(int socket, t_paquete_header header) {

	int id_subscriptor = subscribir_proceso(socket, header.id_cola);

	log_info(logger,
			"Un proceso %s se subscribió a la cola %s. El id_subscriptor generado es: %d \n",
			get_nombre_proceso(header.id_proceso),
			get_nombre_cola(header.id_cola), id_subscriptor);

	socket_send(socket, &id_subscriptor, sizeof(id_subscriptor));
}

static int subscribir_proceso(int socket, t_id_cola id_cola) {

	t_cola_container* container = get_cola(id_cola);

	pthread_mutex_lock(&container->mutex);

	t_subscriptor* subscriptor = subscriptor_crear(socket,
			list_size(container->subscriptores) + 1);
	list_add(container->subscriptores, subscriptor);

	pthread_mutex_unlock(&container->mutex);

	return subscriptor->id_subcriptor;
}

static t_cola_container* get_cola(t_id_cola id_cola) {

	switch (id_cola) {
	case NEW_POKEMON:
		return cola_new_pokemon;
	case APPEARED_POKEMON:
		return cola_appeared_pokemon;
	case CATCH_POKEMON:
		return cola_catch_pokemon;
	case CAUGHT_POKEMON:
		return cola_caught_pokemon;
	case GET_POKEMON:
		return cola_get_pokemon;
	case LOCALIZED_POKEMON:
		return cola_localized_pokemon;
	default:
		log_error(event_logger, "No existe la cola: %d. Finalizando hilo",
				id_cola);
		pthread_exit(NULL);
		return NULL;
	}
}

static int encolar_mensaje(t_cola_container* container, void* msj) {

	int id_mensaje = 0;

	pthread_mutex_lock(&container->mutex);

	id_mensaje = queue_size(container->cola) + 1;
	mensaje_new_pokemon_set_id(msj, id_mensaje);
	queue_push(container->cola, msj);

	pthread_mutex_unlock(&container->mutex);

	return id_mensaje;
}

static void replicar_mensaje(t_cola_container* container, void* deserializado,
		t_id_cola id_cola) {

	for (int i = 0; i < list_size(container->subscriptores); ++i) {

		t_subscriptor* subscriptor = list_get(container->subscriptores, i);

		t_paquete* pqt = paquete_crear(paquete_header_crear(MENSAJE, BROKER, id_cola),
				serializar(deserializado, id_cola));

		//TO-DO: registrar ACK
		enviar_paquete(pqt, subscriptor->socket);

		free(deserializado);
		paquete_destruir(pqt);

	}
}
