#include"utilidades-broker.h"

//Funciones privadas
static int subscribir_proceso(int socket, t_id_cola id_cola);
static t_cola_container* get_cola(t_id_cola id_cola);
static void encolar_mensaje(t_cola_container* container, void* msj);
static void replicar_mensaje(t_cola_container* container, void* deserializado,
		t_id_cola id_cola);
static int generar_id_univoco();
int mensaje_set_id_univoco(void* msj, t_id_cola id_cola);

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

	int id_mensaje = mensaje_set_id_univoco(deserializado, header.id_cola);

	encolar_mensaje(container, deserializado);

	socket_send(socket, &id_mensaje, sizeof(id_mensaje));

	replicar_mensaje(container, deserializado, header.id_cola);

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

static void encolar_mensaje(t_cola_container* container, void* msj) {

	pthread_mutex_lock(&container->mutex);
	queue_push(container->cola, msj);
	pthread_mutex_unlock(&container->mutex);
}

static void replicar_mensaje(t_cola_container* container, void* deserializado,
		t_id_cola id_cola) {

	for (int i = 0; i < list_size(container->subscriptores); ++i) {

		t_subscriptor* subscriptor = list_get(container->subscriptores, i);

		t_paquete* pqt = paquete_crear(
				paquete_header_crear(MENSAJE, BROKER, id_cola),
				serializar(deserializado, id_cola));

		//TO-DO: registrar ACK
		enviar_paquete(pqt, subscriptor->socket);

		free(deserializado);
		paquete_destruir(pqt);

	}
}

static int generar_id_univoco() {

	int id_mensaje = 0;

	pthread_mutex_lock(&mutex_id_univoco);
	id_univoco += 1;
	id_mensaje = id_univoco;
	pthread_mutex_unlock(&mutex_id_univoco);

	return id_mensaje;
}

int mensaje_set_id_univoco(void* msj, t_id_cola id_cola) {

	switch (id_cola) {
	case NEW_POKEMON:
		mensaje_new_pokemon_set_id(msj, generar_id_univoco());
		return mensaje_new_pokemon_get_id(msj);
	case APPEARED_POKEMON:
	case CATCH_POKEMON:
		mensaje_appeared_catch_pokemon_set_id(msj, generar_id_univoco());
		return mensaje_appeared_catch_pokemon_get_id(msj);
	case CAUGHT_POKEMON:
		mensaje_caught_pokemon_set_id(msj, generar_id_univoco());
		return mensaje_caught_pokemon_get_id(msj);
	case GET_POKEMON:
		mensaje_get_pokemon_set_id(msj, generar_id_univoco());
		return mensaje_get_pokemon_get_id(msj);
	case LOCALIZED_POKEMON: {
		mensaje_localized_pokemon_set_id(msj, generar_id_univoco());
		return mensaje_localized_pokemon_get_id(msj);
	}
	default:
		log_error(event_logger,
				"La cola destino %d es incorrecta. Finzalizando hilo \n",
				id_cola);
		pthread_exit(NULL);
		return 0;
	}
}
