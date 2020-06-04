#include "../team.h"
#include "hilos_team.h"
#include<semaphore.h>

/*TODO
 * Agregar equipo_despertar_en_caso_de_DEADLOCK();
 */
void team_planificar(){
	puts("**********************************************\nSe va a planificar");

	while(PROCESO_ACTIVO){
		sem_wait(&HayTareasPendientes);
		equipo_despertar_en_caso_de_APPEARED();

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

//Mensaje APPEARD

// Despierta a los entrenadores dormidos hasta appeard, solamente si hay pokemones en el mapa
void equipo_despertar_en_caso_de_APPEARED(){

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

		entrenadores_despertar_para_catch(equipo, unPokemon);
	}
}

// Pasa a ready a los entrenadores que se encuentren a la espera de la llegada de un pokemon
void entrenadores_despertar_para_catch(entrenadores unEquipo, pokemon* unPokemon){

	void entrenador_despertar(entrenador*unEntrenador){
		if(entrenador_dormido_hasta_APPEARED(unEntrenador)){
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

// Retorna true si el entrenador se encuentra a la espera de la llegada de un pokemon
bool entrenador_dormido_hasta_APPEARED(entrenador*unEntrenador){
	pthread_mutex_lock(&mutexEstadoEntrenador[unEntrenador->id]);
	bool estaDormido = entrenador_en_estado(unEntrenador, LOCKED_HASTA_APPEARED) || entrenador_en_estado(unEntrenador, NEW);
	pthread_mutex_unlock(&mutexEstadoEntrenador[unEntrenador->id]);
	return estaDormido;
}

bool entrenador_en_estado(entrenador* unEntrenador, t_estado ESTADO){

//	pthread_mutex_lock(&Mutex_Entrenador[unEntrenador->id]);
	bool estaEnEstado = unEntrenador->estado == ESTADO;
//	pthread_mutex_lock(&Mutex_Entrenador[unEntrenador->id]);

	return estaEnEstado;
}

//Ver si hace falta despertar por caught (que actualmente se hace en caught) y despertar por deadlock

// Caso DEADLOCK detectado:
