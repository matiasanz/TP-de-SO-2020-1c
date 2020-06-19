#include "detector_de_deadlock.h"

#include "../team.h"

void team_ejecutar_algoritmo_de_deteccion_de_deadlock(){

	esperar_que_equipo_no_pueda_cazar_mas();

//******************************************************************** INICIO HARDCODEADO
	pthread_mutex_lock(&MUTEX_FIN_DE_PROCESO_Para_que_pseudo_broker_deje_de_mandarme_mensajes_BORRAR);
	finDeProceso = true;
	pthread_mutex_unlock(&MUTEX_FIN_DE_PROCESO_Para_que_pseudo_broker_deje_de_mandarme_mensajes_BORRAR);
//******************************************************************** fin HARDCODEADO

	bool hayDeadlock = algoritmo_detectar_deadlock();

	while(hayDeadlock){
		Estadisticas.deadlocksProducidos++;

		loggear_resultado_de_deteccion(hayDeadlock);
		algoritmo_procesar_deadlock();

		Estadisticas.deadlocksResueltos++;

		hayDeadlock = algoritmo_detectar_deadlock();
	}

	loggear_resultado_de_deteccion(hayDeadlock);
}

///*************************************** Detectar Deadlock ************************************************/

bool algoritmo_detectar_deadlock(){
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "Se ejecuto el algoritmo de deteccion de deadlock");
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
	return !list_is_empty(potencialesDeadlock);
}

void loggear_resultado_de_deteccion(bool resultado){
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "Resultado de la deteccion: %s", ( resultado? "Positivo": "Negativo") );
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
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
