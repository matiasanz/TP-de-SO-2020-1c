/*
 ============================================================================
 Name        : GameBoy.c
 Author      :
 Version     :
 Copyright   :
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "gameboy.h"

int main(int argc, char*argv[]) {

	inicializar();

	char* proceso = argv[1];
	char* mensaje = argv[2];

	validar_proceso(proceso);
	validar_mensaje(mensaje);

	if (string_equals_ignore_case(proceso, SUSCRIPTOR_STRING)) {

		char* tiempo_conexion = argv[3];
		procesar_modo_suscriptor(mensaje, tiempo_conexion);

	} else {

		procesar_envio_mensaje(proceso, mensaje, argv, argc);
	}

	config_destroy(config);
	return EXIT_SUCCESS;
}

void inicializar() {

	inicializar_logs();
	inicializar_config();
}

void inicializar_logs() {

	logger = log_create("./log/gameboy.log", GAMEBOY_STRING, 1, LOG_LEVEL_INFO);
	event_logger = log_create("./log/gameboy_event.log", "GAMEBOY_EVENT", 1, LOG_LEVEL_INFO);
}

void inicializar_config() {
	config = config_create("./config/gameboy.config");
}
