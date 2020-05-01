#include "broker.h"

int main(void) {

	inicializar();

	int socket_servidor = socket_crear_listener(
			config_get_string_value(config, "IP_BROKER"),
			config_get_string_value(config, "PUERTO_BROKER"));

	log_info(event_logger, "server levantado exitosamente");

	while (1) {

		t_conexion* cliente = socket_aceptar_conexion(socket_servidor);
		pthread_create(&thread, NULL, (void*) serve_client, cliente);
		pthread_detach(thread);
	}

	return EXIT_SUCCESS;
}

void inicializarColas() {

	cola_new_pokemon = inicializar_cola_container();
	cola_appeared_pokemon = inicializar_cola_container();
	cola_catch_pokemon = inicializar_cola_container();
	cola_caught_pokemon = inicializar_cola_container();
	cola_get_pokemon = inicializar_cola_container();
	cola_localized_pokemon = inicializar_cola_container();
}

void inicializar(void) {

	logger = log_create("./log/broker.log", BROKER_STRING, 1, LOG_LEVEL_INFO);
	event_logger = log_create("./log/broker_event.log", "BROKER_EVENT", 1, LOG_LEVEL_INFO);
	config = config_create("./config/broker.config");

	inicializarColas();

}
