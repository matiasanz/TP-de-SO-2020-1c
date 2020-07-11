#include "logs.h"
#include "mensajesGamecard.h"

void log_enunciado_posiciones_encontradas(char*especie, t_list*posiciones){
	char* posicionesString=posicion_list_to_string(posiciones);
	pthread_mutex_lock(&mutexLogger);
	log_info(logger,"Mensaje:%s, se localizaron %u posiciones para %s,->>>: %s",GET_POKEMON_STRING,list_size(posiciones),especie,posicionesString);
	pthread_mutex_unlock(&mutexLogger);
	free(posicionesString);
}

void log_enunciado_fallo_intento_localizar(t_mensaje_get_pokemon* mensajeGet){
	pthread_mutex_lock(&mutexLogger);
	log_warning(logger,"El archivo pokemon esta abierto, la operacion GET_POKEMON %s se reintentara luego",mensajeGet->especie);
	pthread_mutex_unlock(&mutexLogger);
}

void log_enunciado_pokemon_guardado(t_mensaje_new_pokemon* mensajeNew){
	pthread_mutex_lock(&mutexLogger);
		log_info(logger,"pokemon guardado:%s ::pos (%i,%i)::cant %i"
				,mensajeNew->pokemon.especie
				,mensajeNew->pokemon.posicion.pos_x
				,mensajeNew->pokemon.posicion.pos_y
				,mensajeNew->cantidad);

		pthread_mutex_unlock(&mutexLogger);

}

void log_enunciado_intento_fallido_de_new_pokemon(t_mensaje_new_pokemon* mensajeNew){
	pthread_mutex_lock(&mutexLogger);
	log_error(logger,"El archivo pokemon esta abierto, esta operacion se reintentara luego: New_Pokemon ::%s ::pos (%i,%i)::cant %i"
							,mensajeNew->pokemon.especie
							,mensajeNew->pokemon.posicion.pos_x
							,mensajeNew->pokemon.posicion.pos_y
							,mensajeNew->cantidad);
	pthread_mutex_unlock(&mutexLogger);
}
