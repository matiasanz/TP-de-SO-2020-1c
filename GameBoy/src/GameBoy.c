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
#include "validaciones_gameboy.h"
#include <commons/string.h>
#include <string.h>

int main(int argc, char*argv[]) {
	t_conexion_server conexion;
	inicializar_logs();
	validar_proceso(argv[1]);
	validar_mensaje(argv[2]);
	if(strcmp(argv[1],"SUSCRIPTOR")){

		t_config* config = config_create("./config/gameboy.config");
		if(strcmp(argv[1],BROKER_STRING)==0){
			 conexion_broker = conexion_server_crear(
								config_get_string_value(config, "IP_BROKER"),
								config_get_string_value(config, "PUERTO_BROKER"), GAMEBOY,
								0);
			 conexion = conexion_broker;
			 log_info(logger,"Server levantado exitosamente!");

		}
		if(strcmp(argv[1],TEAM_STRING)==0){
			 conexion_team = conexion_server_crear(
								config_get_string_value(config, "IP_TEAM"),
								config_get_string_value(config, "PUERTO_TEAM"), GAMEBOY,
								0);
			 conexion= conexion_team;
			 log_info(logger,"Server levantado exitosamente!");

		}

		if(strcmp(argv[1],GAMECARD_STRING)==0){
			 conexion_gamecard = conexion_server_crear(
										config_get_string_value(config, "IP_GAMECARD"),
										config_get_string_value(config, "PUERTO_GAMECARD"), GAMEBOY,
										0);
			 conexion= conexion_gamecard;
			 log_info(logger,"Server levantado exitosamente!");
		}

		t_paquete* pqt = crearMensaje(argv,argc);
		int respuesta= enviar(conexion,pqt);
		printf("Id obtenido: %d", respuesta);
		log_info(logger,"Mensaje enviado a la cola de mensajes!");
		//obtener_conexion(argv[1],&conexion);


	}else{

	}

	return 1;

}

void inicializar_logs() {

	logger = log_create("./log/gameboy.log", GAMEBOY_STRING, 1, LOG_LEVEL_INFO);
	event_logger = log_create("./log/gameboy_event.log", "GAMEBOY_EVENT", 1, LOG_LEVEL_INFO);
}
