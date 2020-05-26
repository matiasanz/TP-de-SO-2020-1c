#include "../team.h"
#include "hilos_team.h"
#include<semaphore.h>

/*TODO
 * No frena nunca
 */
void team_planificar(){
	puts("**********************************************\nSe va a planificar");

	while(!FinDelProceso){
		sem_wait(&HayTareasPendientes);
//		sem_wait(&HayEntrenadores); //TODO Resolver espera activa; problema: en cuanto inicializar y no se contempla caso deadlock

		entrenadores colaDeReady = entrenadores_en_estado(equipo, READY); //TODO VERRRR

		entrenadores_despertar_en_caso_de_APPEARD(equipo, colaDeReady);
//wait(colaDeReady->hayMas) //colaDeReady tendria que ser otra lista global :(
		t_id* idProximoEntrenador = entrenadores_id_proximo_a_planificar(colaDeReady);

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

/***************************************** Funciones Auxiliares ***************************************************/

void entrenadores_despertar_en_caso_de_APPEARD(entrenadores unEquipo, entrenadores colaDeReady){
	pthread_mutex_lock(pokemonesRequeridos->mutex);
	if(!list_is_empty(pokemonesRequeridos->lista)){
		pokemon*unPokemon = list_get(pokemonesRequeridos->lista, 0);
		pthread_mutex_unlock(pokemonesRequeridos->mutex);

		if(!unPokemon){
			error_show("Se intento leer un pokemon inexistente\n");
			exit(1);
		}

		list_destroy(colaDeReady);
		colaDeReady = entrenadores_despertar_para_APPEARD(unEquipo, unPokemon);
	}

	else pthread_mutex_unlock(pokemonesRequeridos->mutex);

}


entrenadores entrenadores_despertar_para_APPEARD(entrenadores unEquipo, pokemon* unPokemon){

	void entrenador_despertar_appeard(entrenador*unEntrenador){
		//if(unEntrenador->siguienteTarea == CATCHEAR) && no esta en ready. Aca tendria sentido tener dos listas.
		if(entrenador_en_estado(unEntrenador, NEW) || entrenador_en_estado(unEntrenador, LOCKED_HASTA_APPEARD)){
			entrenador_pasar_a(unEntrenador, READY, "Acaba de llegar un pokemon que puede cazar");
		}
	}

	list_iterate(unEquipo, (void(*)(void*)) &entrenador_despertar_appeard);

	entrenadores enReady = entrenadores_en_estado(unEquipo, READY);

	bool porCercania(entrenador*unEntrenador, entrenador* otroEntrenador){
		numero distanciaDelPrimero = posicion_distancia(unEntrenador->posicion, unPokemon->posicion);
		numero distanciaDelSegundo = posicion_distancia(otroEntrenador->posicion, unPokemon->posicion);
		return  distanciaDelPrimero - distanciaDelSegundo;
	}

	list_sort(enReady, (bool(*)(void*, void*)) &porCercania);

	return enReady;
}
