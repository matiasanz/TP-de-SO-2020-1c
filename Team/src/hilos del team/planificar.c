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
		equipo_despertar_en_caso_de_APPEARD();

		t_id* idProximoEntrenador = entrenadores_id_proximo_a_planificar(entrenadoresReady);

		if(!idProximoEntrenador){
			error_show("Se intento leer un id nulo");
			exit(1);
		}

		printf(">>>Planificar: signal(Entrenador N°%u)\n", *idProximoEntrenador);
		sem_post(&EjecutarEntrenador[*idProximoEntrenador]); //signal(id);
		sem_wait(&EntradaSalida_o_FinDeEjecucion);
	}

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Se finalizo hilo de planificacion");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

}

/***************************************** Funciones Auxiliares ***************************************************/

void equipo_despertar_en_caso_de_APPEARD(){

	pthread_mutex_lock(pokemonesRequeridos->mutex);
	if(list_is_empty(pokemonesRequeridos->lista)){
		/*no hacer nada*/
		pthread_mutex_unlock(pokemonesRequeridos->mutex);
	}

	else{
		pokemon*unPokemon = list_get(pokemonesRequeridos->lista, 0);
		pthread_mutex_unlock(pokemonesRequeridos->mutex);

		if(!unPokemon){
			error_show("Se intento leer un pokemon inexistente\n");
			exit(1);
		}

		entrenadores_despertar(equipo, unPokemon);
	}
}

void entrenadores_despertar(entrenadores unEquipo, pokemon* unPokemon){

	void entrenador_despertar(entrenador*unEntrenador){
		if(entrenador_dormido_hasta_APPEARD(unEntrenador)){
			entrenador_pasar_a(unEntrenador, READY, "Acaba de llegar un pokemon que puede cazar");
			entrenador_agregar_a_cola(unEntrenador, entrenadoresReady);
		}
	}

	pthread_mutex_lock(&mutexEntrenadores);
	list_iterate(unEquipo, (void(*)(void*)) &entrenador_despertar);
	pthread_mutex_unlock(&mutexEntrenadores);

	bool porCercania(entrenador*unEntrenador, entrenador* otroEntrenador){
		numero distanciaDelPrimero = posicion_distancia(unEntrenador->posicion, unPokemon->posicion);
		numero distanciaDelSegundo = posicion_distancia(otroEntrenador->posicion, unPokemon->posicion);
		return  distanciaDelPrimero >= distanciaDelSegundo;
	}

	cr_list_sort(entrenadoresReady, (bool(*)(void*, void*)) &porCercania);
}

bool entrenador_dormido_hasta_APPEARD(entrenador*unEntrenador){
	pthread_mutex_lock(&mutexEstadoEntrenador[unEntrenador->id]);
	bool estaDormido = unEntrenador->estado == LOCKED_HASTA_APPEARD || unEntrenador->estado ==NEW;
	pthread_mutex_unlock(&mutexEstadoEntrenador[unEntrenador->id]);
	return estaDormido;
}
