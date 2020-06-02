#include "gameboy.h"

static void gamecard(t_conexion_server* conexion_broker);
static void team(t_conexion_server* conexion_broker);

int main(void) {

//	pthread_mutex_init(&mutex_enviar, NULL);

	inicializar_logs();

	//Me traigo la config para conocer la ip puerto del broker
	t_config* config = config_create("./config/gameboy.config");

	conexion_broker = conexion_server_crear(
			config_get_string_value(config, "IP_BROKER"),
			config_get_string_value(config, "PUERTO_BROKER"), GAMEBOY);

	conexion_team = conexion_server_crear(
			config_get_string_value(config, "IP_TEAM"),
			config_get_string_value(config, "PUERTO_TEAM"), GAMEBOY);

	conexion_gamecard = conexion_server_crear(
				config_get_string_value(config, "IP_GAMECARD"),
				config_get_string_value(config, "PUERTO_GAMECARD"), GAMEBOY);

	team(conexion_broker);
	gamecard(conexion_broker);

	config_destroy(config);

	return 1;

}

static void gamecard(t_conexion_server* conexion_broker) {

	//APPEARED
	t_mensaje_appeared_catch_pokemon* catch =
			mensaje_appeared_catch_pokemon_crear("newtwo", 9, 11);
	//GET
	t_mensaje_get_pokemon * get = mensaje_get_pokemon_crear("eeve");

	//NEW

	t_mensaje_new_pokemon* new = mensaje_new_pokemon_crear("machop", 2, 9, 76);

	//PAQUETE
	t_paquete* pqt_catch = paquete_crear(
			paquete_header_crear(MENSAJE, GAMEBOY, CATCH_POKEMON),
			mensaje_appeared_catch_pokemon_serializar( catch));

	t_paquete* pqt_get = paquete_crear(
			paquete_header_crear(MENSAJE, GAMEBOY, GET_POKEMON),
			mensaje_get_pokemon_serializar(get));

	t_paquete* pqt_new = paquete_crear(
			paquete_header_crear(MENSAJE, GAMEBOY, NEW_POKEMON),
			mensaje_new_pokemon_serializar(new));



	int respuesta = 0;
	//envio de mensajes a Broker
	respuesta = enviar(conexion_broker, pqt_catch);
	log_info(event_logger, "GAME_CARD: Respuesta BROKER catch: %d", respuesta);
	respuesta = 0;

	respuesta = enviar(conexion_broker, pqt_get);
	log_info(event_logger, "GAME_CARD: Respuesta BROKER get: %d", respuesta);
	respuesta = 0;

	respuesta = enviar(conexion_broker, pqt_new);
	log_info(event_logger, "GAME_CARD: Respuesta BROKER new: %d", respuesta);
	respuesta = 0;

	//envio de mensajes a GAME_CARD
	respuesta = enviar(conexion_gamecard, pqt_catch);
	log_info(event_logger, "Respuesta GAME_CARD catch: %d", respuesta);
	respuesta = 0;

	respuesta = enviar(conexion_gamecard, pqt_get);
	log_info(event_logger, "Respuesta GAME_CARD get: %d", respuesta);
	respuesta = 0;

	respuesta = enviar(conexion_gamecard, pqt_new);
	log_info(event_logger, "Respuesta GAME_CARD new: %d", respuesta);
	respuesta = 0;

	mensaje_appeared_catch_pokemon_destruir( catch);
	mensaje_get_pokemon_destruir(get);
	mensaje_new_pokemon_destruir(new);
	paquete_destruir(pqt_get);
	paquete_destruir(pqt_new);
	paquete_destruir(pqt_catch);
	//		list_destroy(posiciones);
}

static void team(t_conexion_server* conexion_broker) {

	//APPEARED
	t_mensaje_appeared_catch_pokemon* appeared =
			mensaje_appeared_catch_pokemon_crear("charmander", 2, 4);
	//CAUGHT
	t_mensaje_caught_pokemon * caught = mensaje_caught_pokemon_crear(1);

	//LOCALIZED
	t_list * posiciones = list_create();

	t_posicion* p0 = posicion_crear_ptr(0, 0);
	list_add(posiciones, p0);

	t_posicion* p2 = posicion_crear_ptr(5, 3);
	list_add(posiciones, p2);

	t_posicion* p1 = posicion_crear_ptr(2, 4);
	list_add(posiciones, p1);

	t_mensaje_localized_pokemon* localized = mensaje_localized_pokemon_crear(
			"bulbasaur", posiciones);

	//PAQUETE
	t_paquete* pqt_appeared = paquete_crear(
			paquete_header_crear(MENSAJE, GAMEBOY, APPEARED_POKEMON),
			mensaje_appeared_catch_pokemon_serializar(appeared));

	t_paquete* pqt_caught = paquete_crear(
			paquete_header_crear(MENSAJE, GAMEBOY, CAUGHT_POKEMON),
			mensaje_caught_pokemon_serializar(caught));

	t_paquete* pqt_localized = paquete_crear(
			paquete_header_crear(MENSAJE, GAMEBOY, LOCALIZED_POKEMON),
			mensaje_localized_pokemon_serializar(localized));

	int respuesta = 0;
	//Mensajes enviados al Broker
	respuesta = enviar(conexion_broker, pqt_localized);
	log_info(event_logger, "TEAM: Respuesta BROKER localized: %d", respuesta);
	respuesta = 0;

	respuesta = enviar(conexion_broker, pqt_appeared);
	log_info(event_logger, "TEAM: Respuesta BROKER appeared: %d", respuesta);
	respuesta = 0;

	respuesta = enviar(conexion_broker, pqt_caught);
	log_info(event_logger, "TEAM: Respuesta BROKER cuaght: %d", respuesta);
	respuesta = 0;

//Mensajes enviados al Team
	respuesta = enviar(conexion_team, pqt_localized);
	log_info(event_logger, "Respuesta TEAM localized: %d", respuesta);
	respuesta = 0;

	respuesta = enviar(conexion_team, pqt_appeared);
	log_info(event_logger, "Respuesta TEAM appeared: %d", respuesta);
	respuesta = 0;

	respuesta = enviar(conexion_team, pqt_caught);
	log_info(event_logger, "Respuesta TEAM caught: %d", respuesta);
	respuesta = 0;

	mensaje_appeared_catch_pokemon_destruir(appeared);
	mensaje_caught_pokemon_destruir(caught);
	mensaje_localized_pokemon_destruir(localized);
	paquete_destruir(pqt_localized);
	paquete_destruir(pqt_caught);
	paquete_destruir(pqt_appeared);
	list_destroy(posiciones);

}



void inicializar_logs() {

	logger = log_create("./log/gameboy.log", GAMEBOY_STRING, 1, LOG_LEVEL_INFO);
	event_logger = log_create("./log/gameboy_event.log", "GAMEBOY_EVENT", 1,
			LOG_LEVEL_INFO);
}
