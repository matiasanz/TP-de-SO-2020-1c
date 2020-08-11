#include "planificacion.h"
#include "../../team.h"
#include "../../hilos-del-team/hilos_team.h"

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
		fin_de_ciclo_cpu();
	}

	entrenador_esperar_y_consumir_cpu(unEntrenador);
}

void entrenador_otorgar_ciclo_de_cpu(entrenador* unEntrenador){
	log_event_cpu_otorgado();
	sem_post(&EjecutarEntrenador[unEntrenador->id]);
}

void esperar_fin_de_ciclo_cpu(){
	sem_wait(&FinDeCiclo_CPU);
	log_event_cpu_consumido();
}

void fin_de_ciclo_cpu(){
	sem_post(&FinDeCiclo_CPU);
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

bool cambio_de_contexto(t_estado inicial, t_estado final){
	return inicial==EXECUTE || final==EXECUTE;
}


