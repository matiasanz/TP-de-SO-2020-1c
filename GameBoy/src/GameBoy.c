/*
 ============================================================================
 Name        : GameBoy.c
 Author      :
 Version     :
 Copyright   :
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdbool.h>

#include "gameboy.h"

int main(int cantidadDeArgumentos, char*argumentos[]) {

	inicializar();

	validar_cantidad_minima_argumentos(cantidadDeArgumentos, 4, "el proceso Gameboy");

	char* proceso = argumentos[1];
	char* mensaje = argumentos[2];

	validar_proceso(proceso);
	validar_mensaje(mensaje);

	if (modo_suscriptor(proceso)) {

		validar_cantidad_argumentos(cantidadDeArgumentos, 4);
		TIEMPO_CONEXION_STRING = argumentos[3];
		procesar_modo_suscriptor(mensaje, TIEMPO_CONEXION_STRING);

	} else {
		procesar_envio_mensaje(proceso, mensaje, argumentos, cantidadDeArgumentos);
	}

	finalizar_gameboy(EXIT_SUCCESS);
}

void inicializar() {

	inicializar_config();
	proceso_inicializar(GAMEBOY);
	inicializar_logs();
}

void inicializar_logs() {

	logger = log_create("./log/gameboy.log", GAMEBOY_STRING, MOSTRAR_LOGS, LOG_LEVEL_INFO);
	event_logger = log_create("./log/gameboy_event.log", "GAMEBOY_EVENT", MOSTRAR_LOGS, LOG_LEVEL_INFO);
}

void inicializar_config() {
	config = config_create("./config/gameboy.config");
	MOSTRAR_LOGS = config_get_int_value(config, "MOSTRAR_LOGS");
}

void finalizar_gameboy(int ESTADO_FINALIZACION){
	log_destroy(logger);
	log_destroy(event_logger);

	exit(ESTADO_FINALIZACION);
}
