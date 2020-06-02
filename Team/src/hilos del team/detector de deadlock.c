#include "../team.h"
#include "../dominio/estructuras auxiliares/pares en deadlock.h"
#include "hilos_team.h"

#define TIEMPO_ENTRE_DETECCIONES 15 //segundos

void entrenadores_disolver_equipo(entrenadores unEquipo){
	list_destroy(unEquipo);
}

pares_intercambio intercambiosPendientes;

//Es un gran TODO

bool algoritmo_detectar_deadlock(entrenadores enDeadlock);
void algoritmo_procesar_deadlock(entrenadores entrenadoresEnDeadlock);

void team_ejecutar_algoritmo_de_deteccion_de_deadlock_HARDCODEADO(){

	entrenadores entrenadoresEnDeadlock = NULL;

	while(PROCESO_ACTIVO){
		sleep(TIEMPO_ENTRE_DETECCIONES);

		bool hayDeadlock = algoritmo_detectar_deadlock(entrenadoresEnDeadlock);

		algoritmo_procesar_deadlock(entrenadoresEnDeadlock);
		entrenadores_disolver_equipo(entrenadoresEnDeadlock);

		pthread_mutex_lock(&Mutex_AndoLoggeando);
		log_info(logger, "Se ejecuto el detector de deadlock, resultado: %s", ( hayDeadlock? "Positivo": "Negativo") );
		pthread_mutex_unlock(&Mutex_AndoLoggeando);
	}
}

///*************************************** Funciones Auxiliares ************************************************/

bool algoritmo_detectar_deadlock_HARDCODEADO(entrenadores enDeadlock){

	//TODO

	pthread_mutex_lock(&mutexEntrenadores);
	enDeadlock = entrenadores_en_estado(equipo, LOCKED_HASTA_DEADLOCK);
	//Problema del filter: puedo agarrar a alguno que justo esta en medio de un intercambio
	//en lugar de hacer filter removerlos antes de aplicar el algoritmo de deteccion
	pthread_mutex_unlock(&mutexEntrenadores);

	return true;
}

bool entrenador_puede_intercambiar_con(entrenador*unEntrenador, entrenador*otro){
	//TODO
	return true;
}

entrenador* entrenadores_pareja_de_intercambio_para(entrenadores unEquipo, entrenador*unEntrenador){
	bool esParejaDeIntercambio(void*otro){
		return entrenador_puede_intercambiar_con(unEntrenador, otro);
	}
	return list_remove_by_condition(unEquipo, esParejaDeIntercambio);
}

void algoritmo_procesar_deadlock(entrenadores entrenadoresEnDeadlock){
	while(!list_is_empty(entrenadoresEnDeadlock)){ //Limitar condicion en caso de que cantidad no sea par y al final vaya a quedar uno solo
		entrenador*unEntrenador = list_remove(entrenadoresEnDeadlock, 0);

		entrenador*otroEntrenador = entrenadores_pareja_de_intercambio_para(entrenadoresEnDeadlock, unEntrenador);

		par_intercambio* parejaDeIntercambio = par_intercambio_create(unEntrenador, otroEntrenador);

		entrenador_pasar_a(unEntrenador, READY, "Puede intercambiar figus con otro entrenador");
		//Ver como agregar el id del otro, si vale la pena crear funcion especifica

		list_add(intercambiosPendientes, parejaDeIntercambio);
	}
}
