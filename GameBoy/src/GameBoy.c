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

	char* proceso = argv[1];
	char* mensaje = argv[2];
	t_conexion_server* conexion;
	inicializar_logs();
	config = config_create("./config/gameboy.config");

	validar_proceso(proceso);
	validar_mensaje(mensaje);

	if (string_equals_ignore_case(proceso, SUSCRIPTOR_STRING)) {

		char* tiempo_conexion = argv[3];
		procesar_modo_suscriptor(mensaje, tiempo_conexion);

	} else {

		switch (get_id_proceso(proceso)) {
		case BROKER:
			conexion = conexion_server_crear(config_get_string_value(config, "IP_BROKER"),
					config_get_string_value(config, "PUERTO_BROKER"), GAMEBOY);
			break;
		case TEAM:
			conexion = conexion_server_crear(config_get_string_value(config, "IP_TEAM"),
					config_get_string_value(config, "PUERTO_TEAM"), GAMEBOY);
			break;
		case GAMECARD:
			conexion = conexion_server_crear(config_get_string_value(config, "IP_GAMECARD"),
					config_get_string_value(config, "PUERTO_GAMECARD"), GAMEBOY);
			break;
		default:
			log_error(event_logger, "este proceso no es conocido %s", proceso);
			break;
		}

		t_paquete* pqt = crear_mensaje(argv, argc);

		int respuesta = enviar(conexion, pqt);
		log_info(logger, "El proceso gameboy se conecto con el proceso %s", proceso, "y obtuvo la respuesta: %d",
				respuesta);

		config_destroy(config);
		paquete_destruir(pqt);

	}

	return 1;
}

void inicializar_logs() {

	logger = log_create("./log/gameboy.log", GAMEBOY_STRING, 1, LOG_LEVEL_INFO);
	event_logger = log_create("./log/gameboy_event.log", "GAMEBOY_EVENT", 1, LOG_LEVEL_INFO);
}

