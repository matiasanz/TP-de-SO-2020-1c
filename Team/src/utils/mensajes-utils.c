/*
 * mensajes-utils.c
 *
 *  Created on: 1 jun. 2020
 *      Author: utnso
 */

#include "mensajes-utils.h"

bool espero_mensajes(){
	return boolean_sem_consult(&BOOLSEM_EsperoMensajes);
}

void dejar_de_recibir(){
	boolean_sem_turn_off(&BOOLSEM_EsperoMensajes);
}

void mensaje_recibido(t_id_cola id_cola, void* msj){

	if(!espero_mensajes()){
		free(msj);
		sem_post(&finDeSuscripcion);
		pthread_exit(NULL);

		puts("Asi que me quisiste cortar? Mira como te rompo todo el proceso, gil");
	}

	void* deserializado;

	switch (id_cola) {

	case APPEARED_POKEMON:
		deserializado = mensaje_appeared_catch_pokemon_deserializar(msj);
		appeared_pokemon_recibido(deserializado);
		break;

	case CAUGHT_POKEMON:
		deserializado =  mensaje_caught_pokemon_deserializar(msj);
		caught_pokemon_recibido(deserializado);
		break;

	case LOCALIZED_POKEMON:
		deserializado =  mensaje_localized_pokemon_deserializar(msj);
		localized_pokemon_recibido(deserializado);
		break;

	default:
		log_error(event_logger,
				"El %s recibió un mensaje que no esperaba: (%s)",
				TEAM_STRING, get_nombre_cola(id_cola));
	}

	free(msj);
}
