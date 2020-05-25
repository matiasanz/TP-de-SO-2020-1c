#include "../hilos_team.h"
#include "../../team.h"

char* posiciones_to_string(t_list* posiciones){

//	char*stringDePosiciones = "";
//
//	void posicion_concatenar_string(t_posicion*posicion){
//		char*stringDePosicion = posicion_to_string(*posicion);
////		string_append_with_format()concatenar(stringDePosiciones, stringDePosicion);
//	}

//	list_iterate(posiciones, posicion_concatenar_string);

	return "[? ?] [? ?] [? ?]"; //stringDePosiciones;
}

void team_suscriptor_cola_LOCALIZED(cr_list* mensajes){
	while(!FinDelProceso){
		mensaje* mensajeRecibido = cr_list_wait_and_remove(mensajes, 0);

		mensaje_localized_pokemon* pokemonLocalizado = desempaquetar_localized(mensajeRecibido->serializado);

		log_info(logger, "LOCALIZED %s %s", pokemonLocalizado->especie, posiciones_to_string(pokemonLocalizado->posiciones));

		if(!especie_recibida_con_anterioridad(pokemonLocalizado->especie, historialDePokemones)){

			void registrarEnCadaPosicion(t_posicion pos){
				pokemon* unPokemon = pokemon_ptr_create(pokemonLocalizado->especie, pos.pos_x, pos.pos_y);
				registrar_pokemon(unPokemon);
			}

			list_iterate(pokemonLocalizado->posiciones, (void(*)(void*)) registrarEnCadaPosicion);
		}

		else{
			printf("%s: figurita repetida se descarta\n", pokemonLocalizado->especie);
			// crear destructor //pokemon_destroy(unPokemon);
		}
	}

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Finalizo suscripcion a cola LOCALIZED");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}
