/*
 * hilo_localized_pokemon.c
 *
 *  Created on: 31 may. 2020
 *      Author: C-renito Casero
 */

#include "../hilos-de-comunicacion/hilo_localized_pokemon.h"
#include "../team.h"

void subscribir_y_escuchar_cola_localized_pokemon(void (*callback)(t_id_cola, void*)) {

	conexion_localized_pokemon = conexion_cliente_crear(LOCALIZED_POKEMON,
			TIEMPO_RECONEXION,
			callback);

	t_conexion* args_s = conexion_crear(conexion_broker,
			conexion_localized_pokemon);

	pthread_create(&hilo_localized_pokemon, NULL,
			(void*) subscribir_y_escuchar_cola, args_s);

	pthread_detach(hilo_localized_pokemon);
}

void mensaje_localized_pokemon_procesar(t_mensaje_localized_pokemon* mensaje);

void localized_pokemon_recibido(t_mensaje_localized_pokemon* localized_pokemon) {

	if(mensaje_localized_es_para_mi(localized_pokemon)){
		log_enunciado_llegada_de_mensaje_localized(localized_pokemon);

		mensaje_localized_pokemon_procesar(localized_pokemon);

//		cr_list_add_and_signal(mensajesLOCALIZED, localized_pokemon);
		//TODO probar si asi recibe bien los mensajes del gamecard y de ser asi traer lo del otro archivo
	}

	else{
		log_event_localized_descartado_por_id(localized_pokemon);
		mensaje_localized_pokemon_destruir(localized_pokemon);
	}
}

void mensaje_localized_pokemon_procesar(t_mensaje_localized_pokemon* pokemonLocalizado){

	if(!especie_recibida_con_anterioridad(pokemonLocalizado->especie, historialDePokemones)){
		especie_pokemon especie = pokemonLocalizado->especie;
		posiciones_ordenar_por_cercania_al_equipo(pokemonLocalizado->posiciones);
		registrar_en_cada_posicion(especie, pokemonLocalizado->posiciones);
	}

	else{
		log_event_localized_repetido(pokemonLocalizado->especie);
	}

	mensaje_localized_pokemon_destruir(pokemonLocalizado);
}
