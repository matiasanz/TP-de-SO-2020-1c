#include "detector de deadlock.h"
#include "../../team.h"

void team_ejecutar_algoritmo_de_deteccion_de_deadlock(){

	esperar_que_equipo_no_pueda_cazar_mas();

//******************************************************************** INICIO HARDCODEADO

	pthread_mutex_lock(&MUTEX_FIN_DE_PROCESO_BORRARRRRRRRRRRRRRRRR);
	finDeProceso = true;
	pthread_mutex_unlock(&MUTEX_FIN_DE_PROCESO_BORRARRRRRRRRRRRRRRRR);

//******************************************************************** fin HARDCODEADO
	bool hayDeadlock = algoritmo_detectar_deadlock(equipo);

	while(hayDeadlock){
		loggear_deadlock(hayDeadlock);
		algoritmo_procesar_deadlock();
		hayDeadlock = algoritmo_detectar_deadlock(equipo);
	}

	puts("no hay deadlock");

	loggear_deadlock(hayDeadlock);

}

///*************************************** Detectar Deadlock ************************************************/

bool algoritmo_detectar_deadlock(entrenadores unEquipo){
	return !list_is_empty(unEquipo);
}

void loggear_deadlock(bool resultado){
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "Se ejecuto el detector de deadlock, resultado: %s", ( resultado? "Positivo": "Negativo") );
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

////*************************************** Procesar ************************************************//

void candidato_ejecutar(candidato_intercambio*unEntrenador){
	sem_post(&EjecutarEntrenador[unEntrenador->unEntrenador->id]);
}

void algoritmo_procesar_deadlock(){

	candidato_intercambio* unEntrenador = list_get(potencialesDeadlock, 0);
	unEntrenador->unEntrenador->siguienteTarea = INTERCAMBIAR; //HARDCODEADO
//	entrenador_pasar_a(unCandidato->unEntrenador, EXECUTE, "Ha sido seleccionado para intercambiar");
	printf("Signal(Entrenador N°%d)\n", unEntrenador->unEntrenador->id);
	candidato_ejecutar(unEntrenador);

	sem_wait(&finDeIntercambio);
}

//************************************** Auxiliares ***********************************************************

void esperar_que_equipo_no_pueda_cazar_mas(){
	int equipoPuedeCazar=cantidadDeEntrenadores;

	while(equipoPuedeCazar--){
		sem_wait(&EquipoNoPuedaCazarMas);
	}
}
