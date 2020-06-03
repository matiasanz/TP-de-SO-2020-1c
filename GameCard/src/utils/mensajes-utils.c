/*
 * mensajes-utils.c
 *
 *  Created on: 1 jun. 2020
 *      Author: utnso
 */

#include "mensajes-utils.h"

void mensaje_recibido(t_id_cola id_cola, void* msj) {

	void* deserializado;

	switch (id_cola) {

	case CATCH_POKEMON:
		deserializado = mensaje_appeared_catch_pokemon_deserializar(msj);
		catch_pokemon_recibido(deserializado);
		break;

	case GET_POKEMON:
		deserializado = mensaje_get_pokemon_deserializar(msj);
		get_pokemon_recibido(deserializado);
		break;

	case NEW_POKEMON:
		deserializado = mensaje_new_pokemon_deserializar(msj);
		new_pokemon_recibido(deserializado);
		break;

	default:
		log_error(event_logger,
				"El %s recibió un mensaje que no esperaba: (%s)",
				GAMECARD_STRING, get_nombre_cola(id_cola));
	}

	free(msj);
}
