#include "gameboy.h"

t_paquete* crear_paquete(char* proceso, char* mensaje, char* argumentos[], int longitud) {

	t_buffer* mensaje_serializado;
	t_id_cola id_cola;

	char* especie = argumentos[3];
	char* x = argumentos[4];
	char* y = argumentos[5];
	char* cantidad;
	char* id_correlativo;
	char* id;

	switch (get_id_mensaje(mensaje)) {

	case NEW_POKEMON:
		id_cola = NEW_POKEMON;
		validar_quien_conoce_newpokemon(proceso);

		validar_mayor_igual_a_cero(x);
		validar_mayor_igual_a_cero(y);

		if (string_equals_ignore_case(argumentos[1], BROKER_STRING)) {

			validar_cantidad_argumentos(longitud, 7);
			cantidad = argumentos[6];
			validar_mayor_a_cero(cantidad);
			t_mensaje_new_pokemon* msj = mensaje_new_pokemon_crear(especie, atoi(x), atoi(y), atoi(cantidad));
			mensaje_serializado = mensaje_new_pokemon_serializar(msj);

		} else {
			if (longitud == 8) {
				id = argumentos[7];
				// numero alto como se sugiere en https://github.com/sisoputnfrba/foro/issues/1673
			} else {
				id = "999";
			}

			//validar_que_es_numero(id);
			cantidad = argumentos[6];
			validar_mayor_a_cero(cantidad);
			t_mensaje_new_pokemon* msj = mensaje_new_pokemon_crear(especie, atoi(x), atoi(y), atoi(cantidad));
			mensaje_new_pokemon_set_id(msj, atoi(id));
			mensaje_serializado = mensaje_new_pokemon_serializar(msj);
		}

		break;

	case APPEARED_POKEMON:
		id_cola = APPEARED_POKEMON;
		validar_quien_conoce_appearedpokemon(proceso);

		validar_mayor_igual_a_cero(x);
		validar_mayor_igual_a_cero(y);

		if (string_equals_ignore_case(argumentos[1], BROKER_STRING)) {
			validar_cantidad_argumentos(longitud, 7);
			id_correlativo = argumentos[6];

			t_mensaje_appeared_catch_pokemon* msj = mensaje_appeared_catch_pokemon_crear(especie, atoi(x),
					atoi(y));
			mensaje_appeared_catch_pokemon_set_id_correlativo(msj, atoi(id_correlativo));
			mensaje_serializado = mensaje_appeared_catch_pokemon_serializar(msj);

		} else {
			validar_cantidad_argumentos(longitud, 6);

			t_mensaje_appeared_catch_pokemon* msj = mensaje_appeared_catch_pokemon_crear(especie, atoi(x),
					atoi(y));
			mensaje_serializado = mensaje_appeared_catch_pokemon_serializar(msj);

		}

		break;

	case CATCH_POKEMON:
		id_cola = CATCH_POKEMON;
		validar_quien_conoce_catchpokemon(proceso);
		validar_mayor_igual_a_cero(x);
		validar_mayor_igual_a_cero(y);
		if (string_equals_ignore_case(argumentos[1], BROKER_STRING)) {
			validar_cantidad_argumentos(longitud, 6);
			t_mensaje_appeared_catch_pokemon* msj = mensaje_appeared_catch_pokemon_crear(especie, atoi(x),
					atoi(y));
			mensaje_serializado = mensaje_appeared_catch_pokemon_serializar(msj);
		} else {
			validar_cantidad_argumentos(longitud, 7);
			id = argumentos[6];

			t_mensaje_appeared_catch_pokemon* msj = mensaje_appeared_catch_pokemon_crear(especie, atoi(x),
					atoi(y));
			;
			mensaje_appeared_catch_pokemon_set_id(msj, atoi(id));
			mensaje_serializado = mensaje_appeared_catch_pokemon_serializar(msj);
		}

		break;

	case CAUGHT_POKEMON:

		id_cola = CAUGHT_POKEMON;
		validar_quien_conoce_caughtpokemon(proceso);
		validar_cantidad_argumentos(longitud, 5);
		id_correlativo = argumentos[3];

		char* string_atrapado = string_duplicate(argumentos[4]);
		string_trim_right(&string_atrapado);

		validar_ok_fail(string_atrapado);
		int atrapado = string_equals_ignore_case(string_atrapado, "OK");

		free(string_atrapado);
		t_mensaje_caught_pokemon* msj = mensaje_caught_pokemon_crear(atrapado);
		mensaje_caught_pokemon_set_id_correlativo(msj, atoi(id_correlativo));
		mensaje_serializado = mensaje_caught_pokemon_serializar(msj);

		break;
	case GET_POKEMON:
		id_cola = GET_POKEMON;
		validar_quien_conoce_getpokemon(proceso);

		char* especie = string_duplicate(argumentos[3]);
		string_trim_right(&especie);

		if (string_equals_ignore_case(argumentos[1], BROKER_STRING)) {
			validar_cantidad_argumentos(longitud, 4);
			t_mensaje_get_pokemon* msj = mensaje_get_pokemon_crear(especie);
			mensaje_serializado = mensaje_get_pokemon_serializar(msj);

		} else {
			validar_cantidad_argumentos(longitud, 5);
			id = argumentos[4];

			t_mensaje_get_pokemon* msj = mensaje_get_pokemon_crear(especie);
			mensaje_get_pokemon_set_id(msj, atoi(id));
			mensaje_serializado = mensaje_get_pokemon_serializar(msj);
		}
		free(especie);
		break;

	default:
		log_error(event_logger, "este mensaje no es conocido %s", mensaje);
		exit(EXIT_FAILURE);
		break;

	}

	return paquete_crear(paquete_header_crear(MENSAJE, GAMEBOY, id_cola), mensaje_serializado);

}

void procesar_envio_mensaje(char* proceso, char* mensaje, char* argumentos[], int longitud) {

	t_conexion_server* conexion = obtener_conexion(proceso);
	t_paquete* pqt = crear_paquete(proceso, mensaje, argumentos, longitud);

	int respuesta = enviar(conexion, pqt);
	log_info(logger, "El proceso gameboy se conecto con el proceso %s", proceso, "y obtuvo la respuesta: %d",
			respuesta);

	paquete_destruir(pqt);
	conexion_server_destruir(conexion);

}

t_conexion_server* obtener_conexion(char* proceso_string) {

	switch (get_id_proceso(proceso_string)) {
	case BROKER:
		return conexion_server_crear(config_get_string_value(config, "IP_BROKER"),
				config_get_string_value(config, "PUERTO_BROKER"), GAMEBOY);
	case TEAM:
		return conexion_server_crear(config_get_string_value(config, "IP_TEAM"),
				config_get_string_value(config, "PUERTO_TEAM"), GAMEBOY);
	case GAMECARD:
		return conexion_server_crear(config_get_string_value(config, "IP_GAMECARD"),
				config_get_string_value(config, "PUERTO_GAMECARD"), GAMEBOY);

	default:
		log_error(event_logger, "Error al inicializar la conexión. El proceso %s es incorrecto", proceso_string);
		exit(EXIT_FAILURE);
		return NULL;
	}
}
