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

int main(int cantidadDeArgumentos, char*argv[]) {

	inicializar();

	char* proceso = argv[1];
	char* mensaje = argv[2];

	validar_cantidad_minima_argumentos(cantidadDeArgumentos, 4);
	validar_proceso(proceso);
	validar_mensaje(mensaje);

	if (modo_suscriptor(proceso)) {

		validar_cantidad_argumentos(cantidadDeArgumentos, 4);
		char* tiempo_conexion = argv[3];
		procesar_modo_suscriptor(mensaje, tiempo_conexion);

	} else {

		procesar_envio_mensaje(proceso, mensaje, argv, cantidadDeArgumentos);
	}

	finalizar_gameboy(EXIT_SUCCESS);
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

void finalizar_gameboy(int ESTADO_FINALIZACION){
	log_destroy(logger);
	log_destroy(event_logger);

	exit(ESTADO_FINALIZACION);
}
