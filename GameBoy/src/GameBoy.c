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
#include <stdarg.h>//para el va_list

int main(int argc, char*argv[]) {
	int client;

	inicializar();
	validar_proceso(argv[1]);
	validar_mensaje(argv[2]);

	if(argv[1]!="SUSCRIPTOR"){
		char* puerto;
		char* ip;

		obtener_ip_puerto(argv[1],&ip,&puerto);
		client=socket_crear_client(ip,puerto);
		log_info(logger,"Server levantado exitosamente!");
		t_paquete* pqt = crearMensaje(argv,argc);
		int bytes;
		void* a_enviar = paquete_serializar(pqt, &bytes);
		socket_send(client, a_enviar, bytes);
		int respuesta = socket_recibir_int(client);
		printf("Id obtenido del Broker: %d", respuesta);

	} else {
		//validar parametros suscripcion
		puts("entrando en modod suscriptor");
	}


	return EXIT_SUCCESS;
}

void inicializar(void){
	logger = log_create("./log/gameboy.log", GAMEBOY_STRING, 1, LOG_LEVEL_INFO);
	event_logger = log_create("./log/gameboy_event.log", "GAMEBOY_EVENT", 1, LOG_LEVEL_INFO);
	config = config_create("./config/gameboy.config");

}

