/*
 ============================================================================
 Name        : Broker.c
 Author      : 
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 =============================================================================
 */

#include "broker.h"

int main(void) {

	inicializar();

	iniciar_servidor(config_get_string_value(config, "IP"),
			config_get_string_value(config, "PUERTO"));

	return EXIT_SUCCESS;
}

void inicializar(void) {
	logger = iniciar_logger();
	config = leer_config();
}

t_log* iniciar_logger(void) {

	return log_create("./broker.log", "BROKER", 1, LOG_LEVEL_INFO);
}

t_config* leer_config(void) {
	return config_create("./broker.config");
}
