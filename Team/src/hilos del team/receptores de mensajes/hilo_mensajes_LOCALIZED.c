#include "../hilos_team.h"
#include "../../team.h"

void team_suscriptor_cola_LOCALIZED(cr_list* mensajes){
	while(!FinDelProceso){
		mensaje* mensajeRecibido = cr_list_wait_and_remove(mensajes, 0);

		mensaje_localized_pokemon* pokemonLocalizado = desempaquetar_localized(mensajeRecibido->serializado);

		log_info(logger, "LOCALIZED %s%s", pokemonLocalizado->especie, posiciones_to_string(pokemonLocalizado->posiciones));

		if(especie_recibida_con_anterioridad(pokemonLocalizado->especie, historialDePokemones)){
			printf("%s: figurita repetida se descarta\n", pokemonLocalizado->especie);
		}

		else{
			registrar_en_cada_posicion(pokemonLocalizado->especie, pokemonLocalizado->posiciones);
		}
	}

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Finalizo suscripcion a cola LOCALIZED");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

/*************************************** Funciones Auxiliares ************************************************/

void registrar_en_cada_posicion(especie_pokemon unaEspecie, t_list*posiciones){

	void registrarEnCadaPosicion(t_posicion* pos){
		pokemon* unPokemon = pokemon_ptr_create(unaEspecie, pos->pos_x, pos->pos_y);
		registrar_pokemon(unPokemon);
	}

	list_iterate(posiciones, (void(*)(void*)) registrarEnCadaPosicion);
}
