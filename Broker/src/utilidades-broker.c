#include"utilidades-broker.h"

void serve_client(t_conexion* conexion) {

	int codigo_operacion = socket_recibir_int(conexion->socket);
	char* nombre_proceso = get_nombre_proceso(socket_recibir_int(conexion->socket));

	log_info(logger, "El proceso %s se conectó correctamente al %s",
			nombre_proceso, BROKER_STRING);

	switch (codigo_operacion) {
	case MENSAJE: {
		procesar_mensaje(conexion, nombre_proceso);
		break;
	}
	case SUBSCRIPCION: {
		procesar_subscripcion(conexion, nombre_proceso);
		break;
	}
	default:
		log_error(event_logger,
				"El codigo de operacion %d recibido desde el socket: %d por el proceso %s es incorrecto",
				codigo_operacion, conexion->socket, nombre_proceso);
		pthread_exit(NULL);
		abort();
	}
}

void procesar_mensaje(t_conexion* conexion, char* nombre_proceso) {

	// Simula que recibió el mensaje y devuelve siempre un id = 1
	int id_mensaje = 1;

	log_info(event_logger, "mensaje recibido, enviando id mensaje: %d", id_mensaje);

	send(conexion ->socket, &id_mensaje, sizeof(int), 0);
}

void procesar_subscripcion(t_conexion* conexion, char* nombre_proceso) {

	t_tipo_cola_mensaje id_cola_container = socket_recibir_int(
			conexion->socket);

	int estado_subscripcion = subscribir_proceso_a_cola(conexion,
			id_cola_container);

	log_estado_subscripcion(estado_subscripcion, nombre_proceso,
			id_cola_container);

	socket_send(conexion->socket, &estado_subscripcion,
			sizeof(estado_subscripcion));
}

void log_estado_subscripcion(int estado_subscripcion, char* nombre_proceso,
		int id_cola_container) {

	char* nombre_cola = get_nombre_cola_mensaje(id_cola_container);

	if (subscripcion_exitosa(estado_subscripcion)) {
		log_info(logger,
				"Subscripcion exitosa desde el proceso %s hacia la cola %s",
				nombre_proceso, nombre_cola);

		log_info(event_logger, "La cantidad de subcriptores en la cola %s es %d", nombre_cola,
				queue_size(get_cola_container(id_cola_container)->cola));

	} else {
		log_error(event_logger, "No fue posible subscribir el proceso %s a la cola %s",
				nombre_proceso, nombre_cola);
	}
}

int subscribir_proceso_a_cola(t_conexion* conexion,
		t_tipo_cola_mensaje cola_a_subscribir) {

	t_cola_container* container = get_cola_container(cola_a_subscribir);

	pthread_mutex_lock(&container->locker);
	queue_push(container->cola, conexion);
	pthread_mutex_unlock(&container->locker);

	return SUBSCRIPCION_EXITO;
}

t_cola_container* get_cola_container(t_tipo_cola_mensaje cola_a_subscribir) {

	switch (cola_a_subscribir) {
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
		log_error(event_logger, "No existe la cola: %d", cola_a_subscribir);
		return NULL;
	}
}

t_cola_container* inicializar_cola_container() {

	t_cola_container* cola = malloc(sizeof(t_cola_container));
	cola->cola = queue_create();
	cola->subscriptores = list_create();
	pthread_mutex_init(&cola->locker, NULL);

	return cola;
}

void handover(int cod_op, int socket_cliente) {
	int size;
	char * msg;

	msg = socket_recibir_mensaje(socket_cliente, &size);
	log_info(event_logger, "contenido del mensaje: %s ", msg);

	t_buffer* buffer = buffer_crear(size);
	memcpy(buffer->stream, msg, size);

	devolver_mensaje(cod_op, buffer, socket_cliente);
	log_info(event_logger, "mensaje devuelto con exito!");
	free(msg);
}

void devolver_mensaje(int cod_op, t_buffer* buffer, int socket_cliente) {
	t_paquete* paquete = paquete_crear(cod_op, BROKER, buffer);

	int bytes = 0;
	void* a_enviar = paquete_serializar(paquete, &bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}
