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

int main(int cantidadDeArgumentos, char*argumentos[]) {

	inicializar();

	char* proceso = argumentos[1];
	char* mensaje = argumentos[2];

	validar_cantidad_minima_argumentos(cantidadDeArgumentos, 4);
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

	inicializar_logs();
	inicializar_config();
	id_proceso = config_get_int_value(config, "ID_PROCESO");
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
