#include "../hilos_team.h"
#include "../../team.h"
#include "../../utils/mensajes.h"

void team_procesador_cola_LOCALIZED(cr_list* mensajes){

	while(PROCESO_ACTIVO){

		t_mensaje_localized_pokemon* pokemonLocalizado = leer_mensaje_cuando_este_disponible(mensajes);

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
}


