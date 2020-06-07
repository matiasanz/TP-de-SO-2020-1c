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
		sem_wait(&HayEntrenadoresDisponibles);//TODO

		equipo_despertar_en_caso_de_APPEARED();

		puts("**********************************************************wait(proximoEnReady)");
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

//Remueve del mapa a los pokemones que ya no hacen falta
void mapa_limpiar_no_requeridos();

// Despierta a los entrenadores dormidos hasta appeard, solamente si hay pokemones en el mapa
void equipo_despertar_en_caso_de_APPEARED(){

	pthread_mutex_lock(pokemonesRequeridos->mutex);
	mapa_limpiar_no_requeridos();
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

//Pasa a ready un entrenador
void entrenador_despertar(entrenador*unEntrenador){
	entrenador_pasar_a(unEntrenador, READY, "Acaba de llegar un pokemon que puede cazar");
	entrenador_agregar_a_cola(unEntrenador, entrenadoresReady);
}

// Pasa a ready al entrenador que este mas cerca del pokemon, de los que estan dormidos a la espera
void entrenadores_despertar_para_catch(entrenadores unEquipo, pokemon* unPokemon){

	pthread_mutex_lock(&mutexEntrenadores);
	entrenadores dormidos = list_filter(unEquipo, (bool(*)(void*)) &entrenador_dormido_hasta_APPEARED);
	pthread_mutex_unlock(&mutexEntrenadores);

	entrenador*unEntrenador = entrenadores_mas_cercano(dormidos, unPokemon->posicion);

	if(unEntrenador) entrenador_despertar(unEntrenador);

	entrenadores_disolver_equipo(dormidos);
}

// Retorna true si el entrenador se encuentra a la espera de la llegada de un pokemon
bool entrenador_dormido_hasta_APPEARED(entrenador*unEntrenador){
	pthread_mutex_lock(&mutexEstadoEntrenador[unEntrenador->id]);
	bool estaDormido = entrenador_en_estado(unEntrenador, LOCKED_HASTA_APPEARED) || entrenador_en_estado(unEntrenador, NEW);
	pthread_mutex_unlock(&mutexEstadoEntrenador[unEntrenador->id]);
	return estaDormido;
}

bool entrenador_en_estado(entrenador* unEntrenador, t_estado ESTADO){
	return unEntrenador->estado == ESTADO;
}

void mapa_limpiar_no_requeridos(){

	puts("****************************voy a limpiar");

	pokemon*unPokemon = list_get(pokemonesRequeridos->lista, 0);
	while(unPokemon!=NULL && !pokemon_es_requerido(*unPokemon)){

		puts("*************************estoy limpiando");

		printf("\n\n>> Se limpio un %s\n\n", unPokemon->especie);

		sem_wait(&pokemonesRequeridos->hayMas);
		list_remove(pokemonesRequeridos->lista, 0);
		pokemon_destroy(unPokemon);

		sem_wait(&HayTareasPendientes);
		unPokemon = list_get(pokemonesRequeridos->lista, 0);
	}
}
