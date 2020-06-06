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
void validar_mayor_a_cero(char* num) {
	validar_que_es_numero(num);
	int prueba = atoi(num);
	if (prueba <= 0) {
		log_error(event_logger,
				"la cantidad no puede ser menor o igual a cero, el numero que ingreso es %i",
				prueba);
		abort();
	}
}
void validar_mayor_igual_a_cero(char* num) {
	validar_que_es_numero(num);
	int pos = atoi(num);
	if (pos < 0) {
		log_error(event_logger,
				"la cantidad no puede ser menor  a cero, el numero que ingreso es %i",
				pos);
		abort();
	}

}
void validar_que_es_numero(char* num) {
	int longitud = strlen(num);
	int i;
	char* pruebas;
	char* sub = num;

	for (i = 1; i <= longitud; i++) {
		pruebas = string_substring_until(num, 1);
		num = string_substring_from(sub, i);
		if (isdigit(*pruebas) == 0) {
			log_error(event_logger,
					"el caracter ingresado no es un numero, ingreso el caracter: %c",
					*pruebas);
			abort();
		}

	}
}

