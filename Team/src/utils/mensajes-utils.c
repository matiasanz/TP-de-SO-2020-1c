/*
 * mensajes-utils.c
 *
 *  Created on: 1 jun. 2020
 *      Author: utnso
 */

#include "mensajes-utils.h"

void mensaje_recibido(t_paquete* paquete) {

	void* deserializado;

	switch (paquete_get_id_cola(paquete)) {

	case APPEARED_POKEMON:
		deserializado = mensaje_appeared_catch_pokemon_deserializar(paquete_get_stream(paquete));
		paquete_destruir(paquete);
		appeared_pokemon_recibido(deserializado);
		break;

	case CAUGHT_POKEMON:
		deserializado =  mensaje_caught_pokemon_deserializar(paquete_get_stream(paquete));
		paquete_destruir(paquete);
		caught_pokemon_recibido(deserializado);
		break;

	case LOCALIZED_POKEMON:
		deserializado =  mensaje_localized_pokemon_deserializar(paquete_get_stream(paquete));
		paquete_destruir(paquete);
		localized_pokemon_recibido(deserializado);
		break;

	default:
		log_error(event_logger,
				"El %s recibió un mensaje que no esperaba: (%s)",
				TEAM_STRING, get_nombre_cola(paquete_get_id_cola(paquete)));
	}
}
