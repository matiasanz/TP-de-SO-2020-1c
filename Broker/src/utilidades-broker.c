#include"utilidades-broker.h"

//Funciones privadas
static int subscribir_proceso(int socket, t_id_cola id_cola);
static t_cola_container* get_cola(t_id_cola id_cola);

t_cola_container* cola_crear() {

	t_cola_container* cola = malloc(sizeof(t_cola_container));
	cola->cola = queue_create();
	cola->subscriptores = list_create();
	pthread_mutex_init(&cola->mutex, NULL);

	return cola;
}

void procesar_mensaje(int socket, t_paquete_header header) {
	// Simula que recibió el mensaje y devuelve siempre un id = 1
	int id_mensaje = 1;

	log_info(event_logger, "mensaje recibido, enviando id mensaje: %d",
			id_mensaje);

	send(socket, &id_mensaje, sizeof(int), 0);
}

void procesar_subscripcion(int socket, t_paquete_header header) {

	int id_subscriptor = subscribir_proceso(socket, header.id_cola);

	log_info(logger,
			"El proceso %s se subscribió a la cola %s. El id_subscriptor generado es: %d \n",
			get_nombre_proceso(header.id_proceso), get_nombre_cola(header.id_cola), id_subscriptor);

	socket_send(socket, &id_subscriptor, sizeof(id_subscriptor));
}

static int subscribir_proceso(int socket, t_id_cola id_cola) {

	t_cola_container* container = get_cola(id_cola);

	pthread_mutex_lock(&container->mutex);

	t_conexion_cliente* subscriptor = conexion_cliente_crear(socket, queue_size(container->cola) + 1);
	queue_push(container->cola, subscriptor);

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
		log_error(event_logger, "No existe la cola: %d", id_cola);
		return NULL;
	}
}