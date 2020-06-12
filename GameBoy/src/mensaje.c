#include "gameboy.h"

t_paquete* crearMensaje(char* argumentos[], int longitud) {

	t_buffer* mensaje_serializado;
	t_id_cola Tipomensaje;
	int atrapado;
	char* process = argumentos[1];
	char* mensaje = argumentos[2];
	char* especie = argumentos[3];
	char* x = argumentos[4];
	char* y = argumentos[5];
	char* cantidad;
	char* id_correlativo;
	char* id;

	switch (get_id_mensaje(mensaje)) {

	case NEW_POKEMON:
		Tipomensaje = NEW_POKEMON;
		validar_quien_conoce_newpokemon(process);

		validar_mayor_igual_a_cero(x);
		validar_mayor_igual_a_cero(y);

		if (string_equals_ignore_case(argumentos[1], BROKER_STRING)) {

			validar_cantidad_argumentos(longitud, 7);
			cantidad = argumentos[6];
			validar_mayor_a_cero(cantidad);
			t_mensaje_new_pokemon* msj = mensaje_new_pokemon_crear(especie,
					atoi(x), atoi(y), atoi(cantidad));
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
			t_mensaje_new_pokemon* msj = mensaje_new_pokemon_crear(especie,
					atoi(x), atoi(y), atoi(cantidad));
			mensaje_new_pokemon_set_id(msj, atoi(id));
			mensaje_serializado = mensaje_new_pokemon_serializar(msj);
		}

		break;

	case APPEARED_POKEMON:
		Tipomensaje = APPEARED_POKEMON;
		validar_quien_conoce_appearedpokemon(process);

		validar_mayor_igual_a_cero(x);
		validar_mayor_igual_a_cero(y);

		if (string_equals_ignore_case(argumentos[1], BROKER_STRING)) {
			validar_cantidad_argumentos(longitud, 7);
			id_correlativo = argumentos[6];
			validar_que_es_numero(id_correlativo);
			t_mensaje_appeared_catch_pokemon* msj =
					mensaje_appeared_catch_pokemon_crear(especie, atoi(x),
							atoi(y));
			mensaje_appeared_catch_pokemon_set_id_correlativo(msj,
					atoi(id_correlativo));
			mensaje_serializado = mensaje_appeared_catch_pokemon_serializar(
					msj);

		} else {
			validar_cantidad_argumentos(longitud, 6);
			t_mensaje_appeared_catch_pokemon* msj =
					mensaje_appeared_catch_pokemon_crear(especie, atoi(x),
							atoi(y));
			mensaje_serializado = mensaje_appeared_catch_pokemon_serializar(
					msj);

		}

		break;
	case CATCH_POKEMON:
		Tipomensaje = CATCH_POKEMON;
		validar_quien_conoce_catchpokemon(process);
		validar_mayor_igual_a_cero(x);
		validar_mayor_igual_a_cero(y);
		if (string_equals_ignore_case(argumentos[1], BROKER_STRING)) {
			validar_cantidad_argumentos(longitud, 6);
			t_mensaje_appeared_catch_pokemon* msj =
					mensaje_appeared_catch_pokemon_crear(especie, atoi(x),
							atoi(y));
			mensaje_serializado = mensaje_appeared_catch_pokemon_serializar(
					msj);
		} else {
			validar_cantidad_argumentos(longitud, 7);
			id = argumentos[6];
			validar_que_es_numero(id);
			t_mensaje_appeared_catch_pokemon* msj =
					mensaje_appeared_catch_pokemon_crear(especie, atoi(x),
							atoi(y));
			;
			mensaje_appeared_catch_pokemon_set_id(msj, atoi(id));
			mensaje_serializado = mensaje_appeared_catch_pokemon_serializar(
					msj);
		}

		break;
	case CAUGHT_POKEMON:

		Tipomensaje = CAUGHT_POKEMON;
		validar_quien_conoce_caughtpokemon(process);
		validar_cantidad_argumentos(longitud, 5);
		id_correlativo = argumentos[3];
		validar_que_es_numero(id_correlativo);
		validar_ok_fail(argumentos[4]);
		if (string_equals_ignore_case(argumentos[3], "OK")) {
			atrapado = 1;
		} else {
			atrapado = 0;
		}
		t_mensaje_caught_pokemon* msj = mensaje_caught_pokemon_crear(atrapado);
		mensaje_caught_pokemon_set_id_correlativo(msj, atoi(id_correlativo));
		mensaje_serializado = mensaje_caught_pokemon_serializar(msj);

		break;
	case GET_POKEMON:
		Tipomensaje = GET_POKEMON;
		validar_quien_conoce_getpokemon(process);
		especie = argumentos[3];
		if (string_equals_ignore_case(argumentos[1], BROKER_STRING)) {
			validar_cantidad_argumentos(longitud, 4);
			t_mensaje_get_pokemon* msj = mensaje_get_pokemon_crear(especie);
			mensaje_serializado = mensaje_get_pokemon_serializar(msj);

		} else {
			validar_cantidad_argumentos(longitud, 5);
			id = argumentos[4];
			validar_que_es_numero(id);
			t_mensaje_get_pokemon* msj = mensaje_get_pokemon_crear(especie);
			mensaje_get_pokemon_set_id(msj, atoi(id));
			mensaje_serializado = mensaje_get_pokemon_serializar(msj);
		}
		break;

	default:
		log_error(event_logger, "este mensaje no es conocido %s", mensaje);
		exit(EXIT_FAILURE);
		break;

	}

	return paquete_crear(paquete_header_crear(MENSAJE, GAMEBOY, Tipomensaje),
			mensaje_serializado);

}
