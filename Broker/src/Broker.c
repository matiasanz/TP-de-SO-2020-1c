#include "broker.h"

static void validar_header(t_paquete_header header);
static void validar_socket(int socket_servidor);

int main(void) {

	inicializar();

	int socket_servidor = socket_crear_listener(
			config_get_string_value(config, "IP_BROKER"),
			config_get_string_value(config, "PUERTO_BROKER"));

	validar_socket(socket_servidor);

	while (1) {

		int socket_cliente = socket_aceptar_conexion(socket_servidor);
		pthread_create(&thread, NULL, (void*) atender_cliente, &socket_cliente);
		pthread_detach(thread);
	}

	return EXIT_SUCCESS;
}

void crear_colas() {

	cola_new_pokemon = cola_crear();
	cola_appeared_pokemon = cola_crear();
	cola_catch_pokemon = cola_crear();
	cola_caught_pokemon = cola_crear();
	cola_get_pokemon = cola_crear();
	cola_localized_pokemon = cola_crear();
}

void inicializar(void) {

	inicializar_config();
	inicializar_logs();
	crear_colas();
}

void inicializar_config() {

	config = config_create("./config/broker.config");
}

void inicializar_logs() {

	char* ruta = config_get_string_value(config, "LOGGER");
	logger = log_create(ruta, BROKER_STRING, 1, LOG_LEVEL_INFO);
	event_logger = log_create("./log/broker_event.log", "BROKER_EVENT", 1,
			LOG_LEVEL_INFO);
}

void atender_cliente(int* socket) {

	t_paquete_header header = socket_recibir_header(*socket);
	validar_header(header);

	switch (header.codigo_operacion) {
	case MENSAJE: {
		procesar_mensaje(*socket, header);
		break;
	}
	case SUBSCRIPCION: {
		procesar_subscripcion(*socket, header);
		break;
	}
	default:
		log_error(event_logger,
				"El codigo de operacion %d recibido desde el socket: %d por el proceso %s es incorrecto \n",
				header.codigo_operacion, *socket,
				get_nombre_proceso(header.id_proceso));
	}

	pthread_exit(NULL);
}

static void validar_header(t_paquete_header header) {

	if (header.codigo_operacion == ERROR_SOCKET) {
		log_error(event_logger,
				"Error al recibir el header del mensaje, finalizando hilo \n");
		pthread_exit(NULL);
	} else {
		log_info(logger, "Un proceso %s se conectó correctamente al %s \n",
				get_nombre_proceso(header.id_proceso), BROKER_STRING);
	}
}

static void validar_socket(int socket_servidor) {

	if (error_conexion(socket_servidor)) {
		log_info(event_logger,
				"Se produjo un error de conexion al iniciar el %s",
				BROKER_STRING);
		exit(EXIT_FAILURE);
	}

	log_info(event_logger, "%s iniciado exitosamente \n", BROKER_STRING);
}
