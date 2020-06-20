#include "planificacion.h"
#include "../team.h"

void consumir_ciclo_cpu(){
	sleep(RETARDO_CICLO_CPU);
	Estadisticas.ciclosCPU++;
}

//Usar en funciones que requieren cpu del entrenador. Ej ir a (Ver abajo de todo)
void entrenador_esperar_y_consumir_cpu(entrenador*unEntrenador){
	sem_wait(&EjecutarEntrenador[unEntrenador->id]);
	consumir_ciclo_cpu();
	Estadisticas.ciclosDelEntrenador[unEntrenador->id]++; //TODO
}

//	sem_signal(&ConsumioCpu); se debe hacer despues de la actividad

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

void ejecutar_entrenador(entrenador* unEntrenador){
	numero tiempo;
	entrenador_pasar_a(unEntrenador, EXECUTE, "Es su turno de ejecutar");

	for(tiempo=0; !entrenador_termino_de_ejecutar(unEntrenador); tiempo+=RETARDO_CICLO_CPU){

		if(!entrenador_puede_seguir_ejecutando_segun_algoritmo(unEntrenador, tiempo)){

			entrenador_pasar_a(unEntrenador, READY, "Ha sido desalojado por algoritmo de planificacion"); //abstraer a desalojar()
			cr_list_add_and_signal(entrenadoresReady, unEntrenador);

			break;
		}

		puts("***************************************** LE DOY CPU");
		sem_post(&EjecutarEntrenador[unEntrenador->id]);

		puts("***************************************** ESPERO INTERESADOS");

		sem_wait(&FinDeCiclo_CPU);
		puts("***************************************** CONSUMIO CPU");

	}

	actualizar_datos_del_entrenador(unEntrenador, tiempo);

}
