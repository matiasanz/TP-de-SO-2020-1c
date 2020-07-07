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

//************************************ Funciones Auxiliares ****************************************

void ejecutar_entrenador(entrenador* unEntrenador){
	numero tiempo;
	entrenador_pasar_a(unEntrenador, EXECUTE, "Es su turno de ejecutar");

	for(tiempo=0; !entrenador_termino_de_ejecutar(unEntrenador); tiempo+=RETARDO_CICLO_CPU){

		if(criterio_de_desalojo(unEntrenador, tiempo)){

			entrenador_pasar_a(unEntrenador, READY, "Ha sido desalojado por algoritmo de planificacion"); //abstraer a desalojar()
			cr_list_add_and_signal(entrenadoresReady, unEntrenador);

			break;
		}

		log_event_cpu_otorgado();
		sem_post(&EjecutarEntrenador[unEntrenador->id]);

		sem_wait(&FinDeCiclo_CPU);
		log_event_cpu_consumido();
	}

	actualizar_datos_del_entrenador(unEntrenador, tiempo);

}
