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

	if (inicializar() == -1) {
		exit_team();
		return EXIT_FAILURE;
	};

	sem_wait(&objetivos_team);
	return EXIT_SUCCESS;
}

int inicializar() {

	sem_init(&objetivos_team, 0, 0);

	config = config_create(CONFIG_PATH);

	logger = log_create(config_get_string_value(config, "LOG_FILE"), TEAM_STRING, true, LOG_LEVEL_INFO);

	event_logger = log_create("./log/team_event.log", "TEAM_EVENT", 1, LOG_LEVEL_INFO);

	inicializar_conexiones();

	return 1;
}

void inicializar_conexiones() {

	conexion_broker = conexion_server_crear(
			config_get_string_value(config, "IP_BROKER"),
			config_get_string_value(config, "PUERTO_BROKER"), TEAM);

	pthread_mutex_init(&mutex_subscripcion, NULL);
	pthread_mutex_init(&mutex_mensaje_recibido_log, NULL);

	subscribir_y_escuchar_cola_appeared_pokemon((void*) mensaje_recibido);
	subscribir_y_escuchar_cola_caught_pokemon((void*) mensaje_recibido);
	subscribir_y_escuchar_cola_localized_pokemon((void*) mensaje_recibido);

	conectar_gameboy((void*) mensaje_recibido);
}

void exit_team() {

	log_destroy(logger);
	config_destroy(config);

}
