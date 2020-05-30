#include "../hilos_team.h"
#include "../../team.h"

void team_suscriptor_cola_LOCALIZED(cr_list* mensajes){

	while(PROCESO_ACTIVO){
		mensaje* mensajeRecibido = cr_list_wait_and_remove(mensajes, 0);

		mensaje_localized_pokemon* pokemonLocalizado = desempaquetar_localized(mensajeRecibido->serializado);

		log_info(logger, "LOCALIZED %s%s", pokemonLocalizado->especie, posiciones_to_string(pokemonLocalizado->posiciones));

		if(especie_recibida_con_anterioridad(pokemonLocalizado->especie, historialDePokemones)){
			printf("%s: figurita repetida se descarta\n", pokemonLocalizado->especie);
		}

		else{
			numero instanciasRequeridas = objetivos_cantidad_requerida_de(pokemonLocalizado->especie); //TODO
			posiciones posicionesMasCercanasAEquipo = posiciones_N_mas_cercanas_al_equipo(pokemonLocalizado->posiciones, instanciasRequeridas);
			registrar_en_cada_posicion(pokemonLocalizado->especie, posicionesMasCercanasAEquipo);
		}

		//mensaje_localized_destroy(mensajeRecibido); //No tiene que quedar nada, ya que yo construyo nuevos pokemones con copia de la info
	}

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Finalizo suscripcion a cola LOCALIZED");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

/*************************************** Funciones Auxiliares ************************************************/

void registrar_en_cada_posicion(especie_pokemon unaEspecie, posiciones listaDePosiciones){

	void registrarEnCadaPosicion(t_posicion* pos){
		printf("----------------------------------------------------Localized %s\n", unaEspecie);
		pokemon* unPokemon = pokemon_ptr_create(unaEspecie, pos->pos_x, pos->pos_y);
		registrar_pokemon(unPokemon);
	}

	list_iterate(listaDePosiciones, (void(*)(void*)) registrarEnCadaPosicion);
}

posiciones posiciones_N_mas_cercanas_al_equipo(posiciones listaPosiciones, numero cantidadRequerida){

	bool cercaniaAEntrenadores(void*unaPos, void*otraPos){
		return posicion_distancia_a_equipo(unaPos) >= posicion_distancia_a_equipo(otraPos);
	}

	list_sort(listaPosiciones, &cercaniaAEntrenadores);

	return list_take(listaPosiciones, cantidadRequerida);
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
