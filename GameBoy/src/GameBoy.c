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

	/*conexion_broker = conexion_server_crear(
				config_get_string_value(config, "IP_BROKER"),
				config_get_string_value(config, "PUERTO_BROKER"), GAMEBOY,
				0);*/

	t_conexion_server* unaConexion=conexion_server_crear(
			config_get_string_value(config, "IP_GAMECARD"),
			config_get_string_value(config, "PUERTO_GAMECARD"), GAMEBOY);




		t_mensaje_new_pokemon* mensajeAenviar=mensaje_new_pokemon_crear("pikachu",2,3,10);

		t_paquete_header header=paquete_header_crear(MENSAJE,GAMEBOY,NEW_POKEMON);

		t_buffer* bufferDepaquete=mensaje_new_pokemon_serializar(mensajeAenviar);

		t_paquete* paqueteAEnviar=paquete_crear(header,bufferDepaquete);


		enviar(unaConexion,paqueteAEnviar);



	return 1;

}

void inicializar_logs() {

	logger = log_create("./log/gameboy.log", GAMEBOY_STRING, 1, LOG_LEVEL_INFO);
	event_logger = log_create("./log/gameboy_event.log", "GAMEBOY_EVENT", 1, LOG_LEVEL_INFO);
}
