#include "validaciones_gameboy.h"

void validar_proceso(char* proceso) {

	if (strcmp(proceso, BROKER_STRING) && strcmp(proceso, TEAM_STRING)
			&& strcmp(proceso, GAMECARD_STRING)
			&& strcmp(proceso, "SUSCRIPTOR")) {

		log_error(event_logger,
				"el id proceso es incorrecto, el id recibido es %s: ", proceso);
		abort();
	}

}

void validar_mensaje(char* mensaje) {

	if (strcmp(mensaje, NEW_POKEMON_STRING)
			&& strcmp(mensaje, APPEARED_POKEMON_STRING)
			&& strcmp(mensaje, CATCH_POKEMON_STRING)
			&& strcmp(mensaje, CAUGHT_POKEMON_STRING)
			&& strcmp(mensaje, GET_POKEMON_STRING)) {
		log_error(event_logger,
				"el mensaje no existe, el mensaje recibido es %s: ", mensaje);
		abort();
	}

}

//void obtener_conexion(char* proceso,t_conexion_server* conexion){
//
//	t_config* config = config_create("./config/gameboy.config");
//
//	if(strcmp(proceso,BROKER_STRING)==0){
//		 conexion_broker = conexion_server_crear(
//							config_get_string_value(config, "IP_BROKER"),
//							config_get_string_value(config, "PUERTO_BROKER"), GAMEBOY,
//							0);
//		 conexion = conexion_broker;
//
//	}
//	if(strcmp(proceso,TEAM_STRING)==0){
//		 conexion_team = conexion_server_crear(
//							config_get_string_value(config, "IP_TEAM"),
//							config_get_string_value(config, "PUERTO_TEAM"), GAMEBOY,
//							0);
//		 rconexion= conexion_team;
//
//	}
//
//	if(strcmp(proceso,GAMECARD_STRING)==0){
//		 conexion_gamecard = conexion_server_crear(
//									config_get_string_value(config, "IP_GAMECARD"),
//									config_get_string_value(config, "PUERTO_GAMECARD"), GAMEBOY,
//									0);
//		 conexion= conexion_gamecard;
//	}
//
//}

void validar_cantidad_argumentos(int cantidad, int numero) {

	if (cantidad != numero) {
		log_error(event_logger,
				"la cantidad de argumentos recibidos es incorrecto, la cantidad que ingreso es %i: ",
				cantidad);
		abort();
	}
}

void validar_quien_conoce_newpokemon(char* proceso) {

	if (strcmp(proceso, BROKER_STRING) && strcmp(proceso, GAMECARD_STRING)) {
		log_error(event_logger, "este mensaje no lo entiende el  proceso %s",
				proceso);
		abort();
	}
}

void validar_quien_conoce_appearedpokemon(char* proceso) {

	if (strcmp(proceso, BROKER_STRING) && strcmp(proceso, TEAM_STRING)) {
		log_error(event_logger, "este mensaje no lo entiende el  proceso %s",
				proceso);
		abort();
	}
}

void validar_quien_conoce_catchpokemon(char* proceso) {

	if (strcmp(proceso, BROKER_STRING) && strcmp(proceso, GAMECARD_STRING)) {
		log_error(event_logger, "este mensaje no lo entiende el  proceso %s",
				proceso);
		abort();
	}
}

void validar_quien_conoce_caughtpokemon(char* proceso) {

	if (strcmp(proceso, BROKER_STRING)) {
		log_error(event_logger, "este mensaje no lo entiende el  proceso %s",
				proceso);
		abort();
	}
}

void validar_quien_conoce_getpokemon(char* proceso) {

	if (strcmp(proceso, BROKER_STRING) && strcmp(proceso, GAMECARD_STRING)) {
		log_error(event_logger, "este mensaje no lo entiende el  proceso %s",
				proceso);
		abort();
	}
}

void validar_ok_fail(char* argumento) {

	if (strcmp(argumento, "OK") && strcmp(argumento, "FAIL")) {
		log_error(event_logger, "el ultimo argumento no existe, ingreso %s",
				argumento);
		abort();
	}
}

void validar_mayor_a_cero(int* cantidad, char* num) {

	*cantidad = atoi(num);

	if (cantidad <= 0) {
		log_error(event_logger,
				"la cantidad no puede ser menor o igual a cero, el numero que ingreso es %i",
				cantidad);
		abort();
	}
}

void validar_mayor_igual_a_cero(int* pos, char* num) {

	*pos = atoi(num);
	if (pos < 0) {
		log_error(event_logger,
				"la cantidad no puede ser menor  a cero, el numero que ingreso es %i",
				pos);
		abort();
	}

}

