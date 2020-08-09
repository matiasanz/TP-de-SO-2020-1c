#include "mensaje.h"

t_paquete* crear_paquete(char* proceso_recibido, char* mensaje, char* argumentos[], int longitud) {

	t_buffer* mensaje_serializado;
	t_id_cola id_cola;

	char* especie = argumentos[3];
	char* pos_x = argumentos[4];
	char* pos_y = argumentos[5];
	char* cantidad;
	char* id_correlativo;
	char* id;

	switch (get_id_mensaje(mensaje)) {

	case NEW_POKEMON:
		id_cola = NEW_POKEMON;
		validar_quien_conoce_newpokemon(proceso_recibido);
		validar_cantidad_minima_argumentos(longitud, 5, "el mensaje NEW_POKEMON");

		validar_mayor_igual_a_cero(pos_x);
		validar_mayor_igual_a_cero(pos_y);

		if (string_equals_ignore_case(argumentos[1], BROKER_STRING)) {

			validar_cantidad_argumentos(longitud, 7);

			cantidad = argumentos[6];
			validar_mayor_a_cero(cantidad);

			t_mensaje_new_pokemon* msj = mensaje_new_pokemon_crear(especie, atoi(pos_x), atoi(pos_y),
					atoi(cantidad));
			mensaje_serializado = mensaje_new_pokemon_serializar(msj);

		} else {

			id = (longitud == 8) ? argumentos[7] : "9999";
			// numero alto como se sugiere en https://github.com/sisoputnfrba/foro/issues/1673

			cantidad = argumentos[6];
			validar_mayor_a_cero(cantidad);

			t_mensaje_new_pokemon* msj = mensaje_new_pokemon_crear(especie, atoi(pos_x), atoi(pos_y),
					atoi(cantidad));
			mensaje_new_pokemon_set_id(msj, atoi(id));
			mensaje_serializado = mensaje_new_pokemon_serializar(msj);
		}

		break;

	case APPEARED_POKEMON:
		id_cola = APPEARED_POKEMON;

		validar_quien_conoce_appearedpokemon(proceso_recibido);
		validar_cantidad_minima_argumentos(longitud, 6, "el mensaje APPEARED_POKEMON");

		validar_mayor_igual_a_cero(pos_x);
		validar_mayor_igual_a_cero(pos_y);

		if (string_equals_ignore_case(argumentos[1], BROKER_STRING)) {
			validar_cantidad_argumentos(longitud, 7);
			id_correlativo = argumentos[6];

			t_mensaje_appeared_catch_pokemon* msj = mensaje_appeared_catch_pokemon_crear(especie, atoi(pos_x),
					atoi(pos_y));
			mensaje_appeared_catch_pokemon_set_id_correlativo(msj, atoi(id_correlativo));
			mensaje_serializado = mensaje_appeared_catch_pokemon_serializar(msj);

		} else {
			validar_cantidad_argumentos(longitud, 6);

			t_mensaje_appeared_catch_pokemon* msj = mensaje_appeared_catch_pokemon_crear(especie, atoi(pos_x),
					atoi(pos_y));
			mensaje_serializado = mensaje_appeared_catch_pokemon_serializar(msj);
		}

		break;

	case CATCH_POKEMON:

		id_cola = CATCH_POKEMON;

		validar_quien_conoce_catchpokemon(proceso_recibido);
		validar_cantidad_minima_argumentos(longitud, 6, "el mensaje CATCH_POKEMON");

		validar_mayor_igual_a_cero(pos_x);
		validar_mayor_igual_a_cero(pos_y);

		if (string_equals_ignore_case(argumentos[1], BROKER_STRING)) {
			validar_cantidad_argumentos(longitud, 6);
			t_mensaje_appeared_catch_pokemon* msj = mensaje_appeared_catch_pokemon_crear(especie, atoi(pos_x),
					atoi(pos_y));
			mensaje_serializado = mensaje_appeared_catch_pokemon_serializar(msj);

		} else {

			validar_cantidad_argumentos(longitud, 7);
			id = argumentos[6];

			t_mensaje_appeared_catch_pokemon* msj = mensaje_appeared_catch_pokemon_crear(especie, atoi(pos_x),
					atoi(pos_y));

			mensaje_appeared_catch_pokemon_set_id(msj, atoi(id));
			mensaje_serializado = mensaje_appeared_catch_pokemon_serializar(msj);
		}

		break;

	case CAUGHT_POKEMON:

		id_cola = CAUGHT_POKEMON;
		validar_quien_conoce_caughtpokemon(proceso_recibido);
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
		validar_quien_conoce_getpokemon(proceso_recibido);
		validar_cantidad_minima_argumentos(longitud, 4, "el mensaje GET_POKEMON");

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
		finalizar_gameboy(EXIT_FAILURE);
		break;

	}

	return paquete_crear(MENSAJE, id_cola, mensaje_serializado);
}

void procesar_envio_mensaje(char* proceso, char* mensaje, char* argumentos[], int longitud) {

	log_event_intento_de_envio(proceso, mensaje);

	t_conexion_server* conexion = obtener_conexion(proceso);
	t_paquete* pqt = crear_paquete(proceso, mensaje, argumentos, longitud);

	int respuesta = enviar(conexion, pqt);

	paquete_destruir(pqt);
	conexion_server_destruir(conexion);

	if (error_conexion(respuesta)) {
		log_warning(event_logger, "No se pudo conectar al proceso %s\n", proceso);
		finalizar_gameboy(EXIT_FAILURE);
	}

	log_enunciado_conexion_a_proceso(proceso, respuesta);

}

t_conexion_server* obtener_conexion(char* proceso_string) {

	switch (proceso_obtener_tipo_segun_nombre(proceso_string)) {
	case BROKER:
		return conexion_server_crear(config_get_string_value(config, "IP_BROKER"),
				config_get_string_value(config, "PUERTO_BROKER"));
	case TEAM:
		return conexion_server_crear(config_get_string_value(config, "IP_TEAM"),
				config_get_string_value(config, "PUERTO_TEAM"));
	case GAMECARD:
		return conexion_server_crear(config_get_string_value(config, "IP_GAMECARD"),
				config_get_string_value(config, "PUERTO_GAMECARD"));

	default:
		log_error(event_logger, "Error al inicializar la conexión. El proceso %s es incorrecto", proceso_string);
		finalizar_gameboy(EXIT_FAILURE);
		return NULL;
	}
}
