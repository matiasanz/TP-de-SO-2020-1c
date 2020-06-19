#include "planificacion.h"
#include "../team.h"

void consumir_ciclo_cpu(){
	sleep(RETARDO_CICLO_CPU);
	//incrementar contador TODO
}

//Usar en funciones que requieren cpu del entrenador. Ej ir a (Ver abajo de todo)
void entrenador_esperar_y_consumir_cpu(entrenador*unEntrenador){
	sem_wait(&EjecutarEntrenador[unEntrenador->id]);
	consumir_ciclo_cpu();
}

//	sem_signal(&ConsumioCpu); se debe hacer despues de la actividad

void entrenador_consumir_N_cpu(entrenador*unEntrenador, numero cantidad){
	int i;
	for(i=1; i<cantidad; i++){
		entrenador_esperar_y_consumir_cpu(unEntrenador);
		consumir_ciclo_cpu();
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

//************************************************************************************
//Ejemplo de uso del cpu, ya la estan usando. TODO Mover a otro archivo

void desplazar_unidimensional(coordenada* posicionInicial, coordenada posicionFinal);
void entrenador_dar_un_paso_hacia(entrenador*unEntrenador, t_posicion posicionFinal);
void entrenador_ir_hacia(entrenador* unEntrenador, t_posicion posicionFinal){
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El entrenador partio de la posicion [%u %u]\n", unEntrenador->posicion.pos_x, unEntrenador->posicion.pos_y);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);

	bool llegoALaPosicion = entrenador_llego_a(unEntrenador, posicionFinal);

	while(!llegoALaPosicion){
		puts("**********voy a dar un paso");

		pthread_mutex_lock(&mutexPosicionEntrenador[unEntrenador->id]);
		entrenador_dar_un_paso_hacia(unEntrenador, posicionFinal);
		llegoALaPosicion = entrenador_llego_a(unEntrenador, posicionFinal);
		pthread_mutex_unlock(&mutexPosicionEntrenador[unEntrenador->id]);

		puts("di un paso");

		sem_post(&FinDeCiclo_CPU);
		entrenador_esperar_y_consumir_cpu(unEntrenador);
	}
}

void desplazar_unidimensional(coordenada* posicionInicial, coordenada posicionFinal){
	int desplazamiento = (posicionFinal > *posicionInicial) - (posicionFinal < *posicionInicial);
	*posicionInicial += desplazamiento;
}

void entrenador_dar_un_paso_hacia(entrenador*unEntrenador, t_posicion posicionFinal){

	t_posicion* posicionActual = &unEntrenador->posicion;

	if(posicionActual->pos_x != posicionFinal.pos_x){
		desplazar_unidimensional(&posicionActual->pos_x, posicionFinal.pos_x);
	}

	else{
		desplazar_unidimensional(&posicionActual->pos_y, posicionFinal.pos_y);
	}

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "\nEl entrenador N°%u se desplazo a la posicion [%u %u]\n", unEntrenador->id, unEntrenador->posicion.pos_x, unEntrenador->posicion.pos_y);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);

}
