#include "broker.h"

int main(void) {

	inicializar();

	int socket_servidor = socket_crear_listener(
			config_get_string_value(config, "IP_BROKER"),
			config_get_string_value(config, "PUERTO_BROKER"));

	log_info(logger, "server levantado exitosamente");

	while (1) {
		t_cliente* cliente = socket_aceptar_conexion(socket_servidor);
		pthread_create(&thread, NULL, (void*) serve_client, &cliente ->socket);
		pthread_detach(thread);

	}

	return EXIT_SUCCESS;
}

void inicializar(void) {
	logger = log_create("./log/broker.log", "BROKER", 1, LOG_LEVEL_INFO);
	config = config_create("./config/broker.config");
}
