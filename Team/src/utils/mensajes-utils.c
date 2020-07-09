/*
 * mensajes-utils.c
 *
 *  Created on: 1 jun. 2020
 *      Author: utnso
 */

#include "mensajes-utils.h"

bool espero_mensajes(){

	bool esperoMensajes = boolean_sem_consult(&BOOLSEM_EsperoMensajes);

//	pthread_mutex_lock(&mutex_esperoMensajes);
//	bool esperoMensajes = ESPERO_MENSAJES;
//	pthread_mutex_unlock(&mutex_esperoMensajes);

	return esperoMensajes;
}

void dejar_de_recibir(){

	boolean_sem_turn_off(&BOOLSEM_EsperoMensajes);

//	pthread_mutex_lock(&mutex_esperoMensajes);
//	ESPERO_MENSAJES = false;
//	pthread_mutex_unlock(&mutex_esperoMensajes);
}

void mensaje_recibido(t_id_cola id_cola, void* msj){

	if(!espero_mensajes()){
		free(msj);
		return;
	}

	boolean_sem_open(&BOOLSEM_EsperoMensajes);

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

	boolean_sem_close(&BOOLSEM_EsperoMensajes);

	free(msj);
}
