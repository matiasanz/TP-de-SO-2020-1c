#include "../team.h"
#include <semaphore.h>
#include "../hilos-del-team/hilos_team.h"

void ejecutar_entrenador(entrenador*);

void team_planificar(){

	while(PROCESOS_SIN_FINALIZAR){

		entrenador* proximoEntrenador = proximo_a_ejecutar_segun_criterio(entrenadoresReady);

		if(!proximoEntrenador){
			error_show("Se intento leer un entrenador nulo");
			exit(1);
		}

		printf("signal(entrenador %d)\n", proximoEntrenador->id);
		ejecutar_entrenador(proximoEntrenador);
		puts("*************************************** termino de ejecutar");
	}

	sem_post(&FinDePlanificacion);
}
