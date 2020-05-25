#include "../hilos_team.h"
#include "../../team.h"

//void team_cola_LOCALIZED(t_list* mensajes){
//	mensaje* mensajeRecibido = list_remove(mensajes, 0);
//
//	t_mensaje_localized* pokemonLocalizado = desempaquetar_localized(mensajeRecibido->serializado);
//
//	char*posiciones = posicionesToString(pokemonLocalizado->posiciones);
//
//	log_info(logger, "LOCALIZED %s %s", unPokemon->especie, posiciones); //Agregar lista infinita de posiciones
//
//	if(!especie_recibida_con_anterioridad(pokemonLocalizado->especie, historialDePokemones)){
//
//		void registrarEnCadaPosicion(t_posicion pos){
//			pokemon unPokemon = pokemon_create(pokemonLocalizado->especie, pos.pos_x, pos.pos_y);
//			registrar_pokemon(unPokemon);
//		}
//
//		list_iterate(pokemonLocalizado->posiciones, (void(*)(void*)) registrarEnCadaPosicion);
//	}
//
//	else{
//		printf("%s: figurita repetida se descarta\n", pokemonLocalizado->especie);
//		// crear destructor //pokemon_destroy(unPokemon);
//	}
//}
