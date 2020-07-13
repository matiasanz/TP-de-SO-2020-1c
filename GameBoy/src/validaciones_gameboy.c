#include "gameboy.h"
#include "validaciones_gameboy.h"

void validar_proceso(char* proceso) {

	if (strcmp(proceso, BROKER_STRING)   && strcmp(proceso, TEAM_STRING)
									     && strcmp(proceso, GAMECARD_STRING)
										 && strcmp(proceso, SUSCRIPTOR_STRING)) {

		log_error(event_logger, "Se intento leer un proceso <<%s>> desconocido", proceso);
		finalizar_gameboy(EXIT_FAILURE);
	}
}

void validar_mensaje(char* mensaje) {

	if (strcmp(mensaje, NEW_POKEMON_STRING) && strcmp(mensaje, APPEARED_POKEMON_STRING )
											&& strcmp(mensaje, CATCH_POKEMON_STRING    )
											&& strcmp(mensaje, CAUGHT_POKEMON_STRING   )
											&& strcmp(mensaje, GET_POKEMON_STRING      )
											&& strcmp(mensaje, LOCALIZED_POKEMON_STRING)) {

		log_error(event_logger, "Se leyo un mensaje invalido, el mensaje recibido es <<%s>>: ", mensaje);
		finalizar_gameboy(EXIT_FAILURE);
	}

}

void validar_cantidad_minima_argumentos(int cantidadRecibida, int cantidadMinima, char* paraQue){
	if(cantidadRecibida < cantidadMinima){
		log_error(event_logger, "Se ingresaron menos argumentos que el minimo admitido por %s", paraQue);
		if(MOSTRAR_LOGS) puts("(i) Para mas informacion hacer <cat comandos>\n");
		finalizar_gameboy(EXIT_FAILURE);
	}
}

void validar_cantidad_argumentos(int cantidadRecibida, int cantidadEsperada) {

	if (cantidadRecibida != cantidadEsperada) {
		log_error(event_logger, "Se esperaban %u argumentos pero se recibieron %u"
							  , cantidadEsperada
							  , cantidadRecibida);

		finalizar_gameboy(EXIT_FAILURE);
	}
}

void throw_proceso_desconocido_exception(char* proceso){
	log_error(event_logger, "este mensaje no lo recibe el proceso %s", proceso);
	finalizar_gameboy(EXIT_FAILURE);
}

void validar_quien_conoce_newpokemon(char* proceso) {

	if (strcmp(proceso, BROKER_STRING) && strcmp(proceso, GAMECARD_STRING)) {
		throw_proceso_desconocido_exception(proceso);
	}
}

void validar_quien_conoce_appearedpokemon(char* proceso) {
	if (strcmp(proceso, BROKER_STRING) && strcmp(proceso, TEAM_STRING)) {
		throw_proceso_desconocido_exception(proceso);
	}
}

void validar_quien_conoce_catchpokemon(char* proceso) {

	if (strcmp(proceso, BROKER_STRING) && strcmp(proceso, GAMECARD_STRING)) {
		throw_proceso_desconocido_exception(proceso);
	}
}

void validar_quien_conoce_caughtpokemon(char* proceso) {

	if (strcmp(proceso, BROKER_STRING)) {
		throw_proceso_desconocido_exception(proceso);
	}
}

void validar_quien_conoce_getpokemon(char* proceso) {

	if (strcmp(proceso, BROKER_STRING) && strcmp(proceso, GAMECARD_STRING)) {
		throw_proceso_desconocido_exception(proceso);
	}
}

void validar_ok_fail(char* argumento) {

	if (strcmp(argumento, "OK") && strcmp(argumento, "FAIL")) {
		log_error(event_logger, "El resultado ingresado es inconsistente: %s", argumento);
		finalizar_gameboy(EXIT_FAILURE);
	}
}
void validar_mayor_a_cero(char* num) {

	int prueba = atoi(num);
	if (prueba <= 0) {
		log_error(event_logger, "la cantidad no puede ser menor o igual a cero, el numero que ingreso es %i",
				prueba);
		finalizar_gameboy(EXIT_FAILURE);
	}
}

void validar_mayor_igual_a_cero(char* num) {

	int pos = atoi(num);
	if (pos < 0) {
		log_error(event_logger, "la cantidad no puede ser menor  a cero, el numero que ingreso es %i", pos);
		finalizar_gameboy(EXIT_FAILURE);
	}

}

