#include "../team.h"
#include <semaphore.h>
#include "../hilos-del-team/hilos_team.h"

void team_planificar(){

	while(PROCESOS_SIN_FINALIZAR){

		entrenador* proximoEntrenador = proximo_a_ejecutar_segun_criterio(entrenadoresReady);

		validar_entrenador(proximoEntrenador);

		log_event_entrenador_por_ejecutar(proximoEntrenador);
		ejecutar_entrenador(proximoEntrenador);
		log_event_entrenador_termino_de_ejecutar(proximoEntrenador);
	}

}

//************************************ Funciones Auxiliares ****************************************

void ejecutar_entrenador(entrenador* unEntrenador){
	numero tiempo;
	entrenador_pasar_a(unEntrenador, EXECUTE, "Es su turno de ejecutar");

	bool desalojo=false;
	for(tiempo=0; !entrenador_termino_de_ejecutar(unEntrenador); tiempo+=RETARDO_CICLO_CPU){

		if(criterio_de_desalojo(unEntrenador, tiempo)){
			desalojo=true;
			desalojar_entrenador(unEntrenador);
			break;
		}

		log_event_cpu_otorgado();
		sem_post(&EjecutarEntrenador[unEntrenador->id]);

		sem_wait(&FinDeCiclo_CPU);
		log_event_cpu_consumido();
	}

	actualizar_datos_del_entrenador(unEntrenador, tiempo, desalojo);

}

void desalojar_entrenador(entrenador* unEntrenador){
	char*motivo = string_from_format("Ha sido desalojado por %s", MOTIVO_DESALOJO);
	entrenador_pasar_a(unEntrenador, READY, motivo);
	cr_list_add_and_signal(entrenadoresReady, unEntrenador);
	free(motivo);
}
