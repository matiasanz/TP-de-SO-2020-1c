#include "../planificacion/planificacion.h"
#include "../../team.h"

void consumir_ciclo_cpu(){
	sleep(RETARDO_CICLO_CPU);
	Estadisticas.ciclosCPU++;
}

//Usar en funciones que requieren cpu del entrenador. Ej ir a (Ver abajo de todo)
void entrenador_esperar_y_consumir_cpu(entrenador*unEntrenador){
	sem_wait(&EjecutarEntrenador[unEntrenador->id]);
	consumir_ciclo_cpu();
	Estadisticas.ciclosDelEntrenador[unEntrenador->id]++;
}

//	Despues de finalizar la actividad posterior a la espera, se debe hacer sem_signal(&ConsumioCpu);

void entrenador_consumir_N_cpu(entrenador*unEntrenador, numero cantidad){
	int i;
	for(i=1; i<cantidad; i++){
		entrenador_esperar_y_consumir_cpu(unEntrenador);
		sem_post(&FinDeCiclo_CPU);
	}

	entrenador_esperar_y_consumir_cpu(unEntrenador);
}

bool entrenador_esta_ejecutando(entrenador*unEntrenador){
	pthread_mutex_lock(&mutexEstadoEntrenador[unEntrenador->id]);
	bool estaEjecutando = entrenador_en_estado(unEntrenador, EXECUTE);
	pthread_mutex_unlock(&mutexEstadoEntrenador[unEntrenador->id]);
	return estaEjecutando;
}

bool entrenador_termino_de_ejecutar(entrenador*unEntrenador){
	return !entrenador_esta_ejecutando(unEntrenador);
}


