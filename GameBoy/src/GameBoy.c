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
	char* proceso=argv[1];
	char* mensaje=argv[2];
	t_conexion_server conexion;
	inicializar_logs();
	validar_proceso(proceso);
	validar_mensaje(mensaje);
	if(strcmp(proceso,"SUSCRIPTOR")){
		inicializar_config();

		switch(get_id_proceso(proceso)){
			case BROKER:
				conexion_broker = conexion_server_crear(
												config_get_string_value(config, "IP_BROKER"),
												config_get_string_value(config, "PUERTO_BROKER"), GAMEBOY,
												0);
							 conexion = conexion_broker;
				break;
			case TEAM:
				conexion_team = conexion_server_crear(
												config_get_string_value(config, "IP_TEAM"),
												config_get_string_value(config, "PUERTO_TEAM"), GAMEBOY,
												0);
							 conexion= conexion_team;
				break;
			case GAMECARD:
				conexion_gamecard = conexion_server_crear(
														config_get_string_value(config, "IP_GAMECARD"),
														config_get_string_value(config, "PUERTO_GAMECARD"), GAMEBOY,
														0);
							 conexion= conexion_gamecard;
				break;
		}

		t_paquete* pqt = crearMensaje(argv,argc);
		int respuesta= enviar(conexion,pqt);
		printf("Id obtenido: %d", respuesta);
		log_info(logger,"El proceso :%s",proceso,"se conecto con el gameboy y envio una respuesta: %d",respuesta);
		//obtener_conexion(argv[1],&conexion);


	}else{

	}

	return 1;

}

void inicializar_logs() {

	logger = log_create("./log/gameboy.log", GAMEBOY_STRING, 1, LOG_LEVEL_INFO);
	event_logger = log_create("./log/gameboy_event.log", "GAMEBOY_EVENT", 1, LOG_LEVEL_INFO);
}

void inicializar_config(){
	t_config* config = config_create("./config/gameboy.config");
}
