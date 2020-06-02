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

// Fix para que compile esto hay que arreglarlo!!
static int get_id_proceso(char* proceso);

static int get_id_proceso(char* proceso) {
	return 1;
}

int main(int argc, char*argv[]) {

	char* proceso = argv[1];
	char* mensaje = argv[2];

	inicializar_logs();
	validar_proceso(proceso);
	validar_mensaje(mensaje);

	if (strcmp(proceso, "SUSCRIPTOR")) {
		inicializar_config();

		switch (get_id_proceso(proceso)) {
		case BROKER:
			conexion = conexion_server_crear(
					config_get_string_value(config, "IP_BROKER"),
					config_get_string_value(config, "PUERTO_BROKER"), GAMEBOY);
			break;
		case TEAM:
			conexion = conexion_server_crear(
					config_get_string_value(config, "IP_TEAM"),
					config_get_string_value(config, "PUERTO_TEAM"), GAMEBOY);

			break;
		case GAMECARD:
			conexion = conexion_server_crear(
					config_get_string_value(config, "IP_GAMECARD"),
					config_get_string_value(config, "PUERTO_GAMECARD"),
					GAMEBOY);
			break;
		}

		t_paquete* pqt = crearMensaje(argv, argc);
		int respuesta = enviar(conexion, pqt);

		log_info(logger, "El proceso :%s", proceso,
				"se conecto con el gameboy y envio una respuesta: %d",
				respuesta);

	} else {

	}

	return 1;

}

void inicializar_logs() {

	logger = log_create("./log/gameboy.log", GAMEBOY_STRING, 1, LOG_LEVEL_INFO);
	event_logger = log_create("./log/gameboy_event.log", "GAMEBOY_EVENT", 1,
			LOG_LEVEL_INFO);
}

void inicializar_config() {
	config = config_create("./config/gameboy.config");
}
