#include "detector_de_deadlock.h"
#include "../team.h"

void team_ejecutar_algoritmo_de_deteccion_de_deadlock(){

	bool hayDeadlock = algoritmo_detectar_deadlock();

	while(hayDeadlock){
		Estadisticas.deadlocksProducidos++;

		log_enunciado_resultado_de_algoritmo_de_deteccion_de_deadlock(hayDeadlock);
		algoritmo_procesar_deadlock();

		Estadisticas.deadlocksResueltos++;

		hayDeadlock = algoritmo_detectar_deadlock();
	}

	log_enunciado_resultado_de_algoritmo_de_deteccion_de_deadlock(hayDeadlock);
}

///*************************************** Detectar Deadlock ************************************************/

bool algoritmo_detectar_deadlock(){
	log_enunciado_inicio_de_algoritmo_de_deteccion_de_deadlock();
	return !list_is_empty(potencialesDeadlock);
}

////*************************************** Procesar ************************************************//

void candidato_despertar_para_intercambio(candidato_intercambio*unCandidato){
	entrenador_despertar(unCandidato->interesado, "Ha sido seleccionado para intercambiar");
}

void algoritmo_procesar_deadlock(){

	candidato_intercambio* unCandidato = list_get(potencialesDeadlock, 0);

	candidato_despertar_para_intercambio(unCandidato);

	sem_wait(&finDeIntercambio);
}

//************************************** Auxiliares ***********************************************************

void esperar_que_equipo_no_pueda_cazar_mas(){
	int equipoPuedeCazar=cantidadDeEntrenadores;

	while(equipoPuedeCazar--){
		sem_wait(&EquipoNoPuedaCazarMas);
	}
}
