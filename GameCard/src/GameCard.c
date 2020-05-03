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
	logger = log_create("./log/gamecard.log", GAMECARD_STRING, 1, LOG_LEVEL_INFO);
	event_logger = log_create("./log/gamecard_event.log", "GAME_CARD_EVENT", 1, LOG_LEVEL_INFO);
	config = config_create("./config/gamecard.config");

	subscribpcion_colas();

}

void subscribpcion_colas() {
	char* ip_broker = config_get_string_value(config, "IP_BROKER");
	char* puerto_broker = config_get_string_value(config, "PUERTO_BROKER");
	int estado_subscipcion;

	estado_subscipcion = subscripcion_cola(GAMECARD, NEW_POKEMON, ip_broker, puerto_broker);
	log_info(event_logger , "Respuesta subscripcion a la cola %s: %s",
			get_nombre_cola_mensaje(NEW_POKEMON),
			subscripcion_exitosa(estado_subscipcion) ? "ok" : "error");

	estado_subscipcion = subscripcion_cola(GAMECARD, CATCH_POKEMON, ip_broker, puerto_broker);
	log_info(event_logger, "Respuesta subscripcion a la cola %s: %s",
			get_nombre_cola_mensaje(CATCH_POKEMON),
			subscripcion_exitosa(estado_subscipcion) ? "ok" : "error");

	estado_subscipcion = subscripcion_cola(GAMECARD, GET_POKEMON, ip_broker, puerto_broker);
	log_info(event_logger, "Respuesta subscripcion a la cola %s: %s",
			get_nombre_cola_mensaje(GET_POKEMON),
			subscripcion_exitosa(estado_subscipcion) ? "ok" : "error");
}
