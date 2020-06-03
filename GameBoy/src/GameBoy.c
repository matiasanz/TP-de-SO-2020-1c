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

int main(void) {

	inicializar_logs();

	//Me traigo la config para conocer la ip puerto del broker
	t_config* config = config_create("./config/gameboy.config");

	conexion_broker = conexion_server_crear(
				config_get_string_value(config, "IP_BROKER"),
				config_get_string_value(config, "PUERTO_BROKER"), GAMEBOY,
				0);

	return 1;

}

void inicializar_logs() {

	logger = log_create("./log/gameboy.log", GAMEBOY_STRING, 1, LOG_LEVEL_INFO);
	event_logger = log_create("./log/gameboy_event.log", "GAMEBOY_EVENT", 1, LOG_LEVEL_INFO);
}
