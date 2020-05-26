#include "../team.h"
#include "hilos_team.h"
#include<semaphore.h>

// Pasa a estado ready a los entrenadores que se encuentran en NEW o en LOCKED hasta APPEARD
	entrenadores entrenadores_despertar_APPEARD(entrenadores, pokemon*);

//*******************************************************************

/*TODO
 * No frena nunca
 */
void team_planificar(){
		objetivosGlobales = entrenadores_objetivos_globales(equipo);
		Get_pokemones(objetivosGlobales);
		sem_post(&BORRAR_ESTO_MENSAJE_GET_ENVIADO); //hardcodeado para que me llegue localized

	puts("**********************************************\nSe va a planificar");

	while(!FinDelProceso){
		sem_wait(&HayTareasPendientes);

		entrenadores colaDeReady = entrenadores_en_estado(equipo, READY);

		pthread_mutex_lock(pokemonesRequeridos->mutex);
		if(!list_is_empty(pokemonesRequeridos->lista)){ //TODO abstraer
			pokemon*unPokemon = list_get(pokemonesRequeridos->lista, 0);
			pthread_mutex_unlock(pokemonesRequeridos->mutex);

			if(!unPokemon){
				error_show("Se intento leer un pokemon inexistente\n");
				exit(1);
			}

			list_destroy(colaDeReady);
			colaDeReady = entrenadores_despertar_APPEARD(equipo, unPokemon);
		}

		else pthread_mutex_unlock(pokemonesRequeridos->mutex);

		t_id* idProximoEntrenador = entrenadores_id_proximo_a_planificar(colaDeReady); // equipoReady) TODO

		if(idProximoEntrenador){
			printf(">>>Planificar: signal(Entrenador N°%u)\n", *idProximoEntrenador);
			sem_post(&EjecutarEntrenador[*idProximoEntrenador]); //signal(id);

			sem_wait(&EntradaSalida_o_FinDeEjecucion);
		}

		else puts("No hay entrenadores en ready");
	}

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Se finalizo hilo de planificacion");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

}


entrenadores entrenadores_despertar_APPEARD(entrenadores equipo, pokemon* unPokemon){

	void entrenador_despertar_appeard(entrenador*unEntrenador){
		//if(unEntrenador->siguienteTarea == CATCHEAR) && no esta en ready. Aca tendria sentido tener dos listas.
		if(entrenador_en_estado(unEntrenador, NEW) || entrenador_en_estado(unEntrenador, LOCKED_HASTA_APPEARD)){
			entrenador_pasar_a(unEntrenador, READY, "Acaba de llegar un pokemon que puede cazar");
		}
	}

	list_iterate(equipo, (void(*)(void*)) &entrenador_despertar_appeard);

	entrenadores enReady = entrenadores_en_estado(equipo, READY);

	bool porCercania(entrenador*unEntrenador, entrenador* otroEntrenador){
		numero distanciaDelPrimero = posicion_distancia(unEntrenador->posicion, unPokemon->posicion);
		numero distanciaDelSegundo = posicion_distancia(otroEntrenador->posicion, unPokemon->posicion);
		return  distanciaDelPrimero - distanciaDelSegundo;
	}

	list_sort(enReady, (bool(*)(void*, void*)) &porCercania);

	return enReady;
}
