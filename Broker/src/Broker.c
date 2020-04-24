#include "broker.h"

int main(void) {

	inicializar();
	char * ip = config_get_string_value(config, "IP_BROKER");
	char * puerto = config_get_string_value(config, "PUERTO_BROKER");
	int socket_servidor = cr_crear_socket(ip, puerto, SERVIDOR);

	log_info(logger, "server levantado exitosamente");

	while (1) {

		int socket_cliente = cr_aceptar_conexion(socket_servidor);
		pthread_create(&thread, NULL, (void*) serve_client, &socket_cliente);
		pthread_detach(thread);

	}

	return EXIT_SUCCESS;
}

void inicializar(void) {
	logger = log_create("./log/broker.log", "BROKER", 1, LOG_LEVEL_INFO);
	config = config_create("./config/broker.config");
}
