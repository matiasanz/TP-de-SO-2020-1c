/*
 ============================================================================
 Name        : GameCard.c
 Author      : 
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "gamecard.h"

int main(void) {

	inicializar();

	return EXIT_SUCCESS;
}

void inicializar(void) {

	inicializar_config();
	inicializar_logs();
	inicializar_conexiones();
}

void inicializar_config() {

	config = config_create("./config/gamecard.config");
}

void inicializar_conexiones() {

	conexion_broker = conexion_server_crear(
			config_get_string_value(config, "IP_BROKER"),
			config_get_string_value(config, "PUERTO_BROKER"), GAMECARD,
			config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION"));

	pthread_t hilo_subscriptor;
	pthread_create(&hilo_subscriptor, NULL, (void*) subscribir_colas, NULL);
	pthread_detach(hilo_subscriptor);
}

void inicializar_logs() {

	logger = log_create("./log/gamecard.log", GAMECARD_STRING, 1,LOG_LEVEL_INFO);
	event_logger = log_create("./log/gamecard_event.log", "GAME_CARD_EVENT", 1, LOG_LEVEL_INFO);
}

void subscribir_colas(void* arg) {

	conexion_catch_pokemon = subscribir_cola(conexion_broker, CATCH_POKEMON);
	conexion_get_pokemon = subscribir_cola(conexion_broker, GET_POKEMON);
	conexion_new_pokemon = subscribir_cola(conexion_broker, NEW_POKEMON);

    pthread_exit(NULL);
}
