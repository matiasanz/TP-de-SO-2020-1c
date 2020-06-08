#include "../hilos-del-team/hilos_team.h"
#include "../team.h"
#include "../estructuras-auxiliares/mensajes.h"

void team_procesador_cola_LOCALIZED(cr_list* mensajes){

	while(PROCESO_ACTIVO){
		t_mensaje_localized_pokemon* pokemonLocalizado = leer_mensaje_cuando_este_disponible(mensajes);

		pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
		log_info(logger, "LOCALIZED %s%s", pokemonLocalizado->especie, posiciones_to_string(pokemonLocalizado->posiciones));
		pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

		if(especie_recibida_con_anterioridad(pokemonLocalizado->especie, historialDePokemones)){
			printf("%s: figurita repetida se descarta\n", pokemonLocalizado->especie);
		}

		else{
			posiciones_ordenar_por_cercania_al_equipo(pokemonLocalizado->posiciones);
			registrar_en_cada_posicion(string_duplicate(pokemonLocalizado->especie), pokemonLocalizado->posiciones);
		}

		mensaje_localized_pokemon_destruir(pokemonLocalizado); //No tiene que quedar nada, ya que yo construyo nuevos pokemones con copia de la info
	}

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Finalizo suscripcion a cola LOCALIZED");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

/*************************************** Funciones Auxiliares ************************************************/

void registrar_en_cada_posicion(especie_pokemon unaEspecie, posiciones listaDePosiciones){

	void registrarEnCadaPosicion(t_posicion* pos){
		pokemon* unPokemon = pokemon_ptr_create(unaEspecie, *pos);
		registrar_pokemon(unPokemon);
	}

	list_iterate(listaDePosiciones, (void(*)(void*)) registrarEnCadaPosicion);
}

void posiciones_ordenar_por_cercania_al_equipo(posiciones listaPosiciones){
	bool cercaniaAEntrenadores(void*unaPos, void*otraPos){
		return posicion_distancia_a_equipo(unaPos) >= posicion_distancia_a_equipo(otraPos);
	}

	list_sort(listaPosiciones, &cercaniaAEntrenadores);
}


numero posicion_distancia_a_equipo(t_posicion*posicion){
	return posicion_distancia_a_entrenadores(posicion, equipo);
}

numero posicion_distancia_a_entrenadores(t_posicion* posicion, entrenadores unEquipo){

	numero distancia = NUMERO_MAXIMO;

	void menorDistancia(void*unEntrenador){
		t_id eid = ((entrenador*)unEntrenador)->id;
		pthread_mutex_lock(&mutexPosicionEntrenador[eid]);
		t_posicion posicionDelEntrenador = ((entrenador*)unEntrenador)->posicion;
		pthread_mutex_unlock(&mutexPosicionEntrenador[eid]);

		numero distanciaAlEntrenador = posicion_distancia(*posicion, posicionDelEntrenador);

		if(distancia > distanciaAlEntrenador) distancia = distanciaAlEntrenador;
	}

	pthread_mutex_lock(&mutexEntrenadores);
	list_iterate(equipo, &menorDistancia);
	pthread_mutex_unlock(&mutexEntrenadores);

	return distancia;
}
