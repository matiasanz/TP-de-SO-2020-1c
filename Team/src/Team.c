/*
 ============================================================================
 Name        : Team.c
 Author      : 
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "team.h"


int main(void) {

	printf("hola soy el team");

	if(inicializar()==-1){
		exit_team();
		return EXIT_FAILURE;
	};

	return EXIT_SUCCESS;
}

int inicializar(){

	config=config_create(CONFIG_PATH);

	logger=log_create(config_get_string_value(config,"LOG_FILE"),TEAM_STRING,true,LOG_LEVEL_INFO);
	event_logger = log_create("./log/team_event.log", "TEAM_EVENT", 1, LOG_LEVEL_INFO);

	inicializar_conexiones();

	return 1;
}

void inicializar_conexiones() {

	conexion_broker = conexion_server_crear(
			config_get_string_value(config, "IP_BROKER"),
			config_get_string_value(config, "PUERTO_BROKER"), TEAM,
			config_get_int_value(config, "TIEMPO_RECONEXION"));

}

void subscribir_colas(void* arg) {

}



void exit_team(){

	log_destroy(logger);
	config_destroy(config);

}
