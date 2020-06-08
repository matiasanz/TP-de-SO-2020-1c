#include "planificacion.h"
#include "../../team.h"


//Usar en funciones que requieren cpu del entrenador. Ej ir a (Ver abajo de todo)
void entrenador_consumir_cpu(entrenador*unEntrenador){
	sem_wait(&EjecutarEntrenador[unEntrenador->id]);
	sleep(RETARDO_CICLO_CPU);
}//	sem_signal(&ConsumioCpu); se deberia hacer fuera de la funcion, despues de la actividad que tenia que hacer

bool entrenador_termino_de_ejecutar(entrenador*unEntrenador){
	pthread_mutex_lock(&mutexEstadoEntrenador[unEntrenador->id]);
	bool termino = !entrenador_en_estado(unEntrenador, EXECUTE);
	pthread_mutex_unlock(&mutexEstadoEntrenador[unEntrenador->id]);

	return termino;
}

void ejecutar_entrenador(entrenador* unEntrenador){
	numero tiempo=0;
	sem_post(&EjecutarEntrenador[unEntrenador->id]);
	for(tiempo=RETARDO_CICLO_CPU; puede_seguir_ejecutando_segun_algoritmo(unEntrenador, tiempo, ALGORITMO_PLANIFICACION); tiempo+=RETARDO_CICLO_CPU){
//		sem_wait(&ConsumioCpu);
		sem_post(&EjecutarEntrenador[unEntrenador->id]);

		if(entrenador_termino_de_ejecutar(unEntrenador)){
			break;
		}
	}
}

bool puede_seguir_ejecutando_segun_algoritmo(entrenador*unEntrenador, numero tiempo, t_algoritmo algoritmo){
	switch(algoritmo){
		case FIFO: ;
		/*no break*/
		case SJF_SD: ;
			return true;

//		case ROUND_ROBBIN: {
//			return tiempo<QUANTUM
//			break;
//		}

//		case SJF_CD{
//			return tiempo <= menor_estimacion(entrenadores_ready);
//		}

		default : {
			puts("El algoritmo leido no se encuentra implementado... aun! ;)");
			exit(1);
			return false;
		}
	}
}

//TODO ver como usar orden superior y asignar una sola funcion al momento de setear algoritmo
//entrenador* proximoAEjecutar(cola_entrenadores colaReady){ //volver a lista con mutex propio
//
//	entrenador*semilla = cr_list_wait_and_remove(colaReady, 0);
//
//	switch(ALGORITMO_PLANIFICACION){
//		case FIFO: ;
//		/*no break*/
//		case ROUND_ROBBIN ;
//			return semilla;
//
//		default: {
//
//			void*entrenador_con_menor_tiempo(entrenador*unEntrenador, entrenador*otro){
//				return
//			}
//
//			return cr_list_fold(colaReady, semilla, );
//		}
//	}
//}

//************************************************************************************

////Ejemplo de uso del cpu
void desplazar_unidimensional(coordenada* posicionInicial, coordenada posicionFinal);
void entrenador_dar_un_paso_hacia(entrenador*unEntrenador, t_posicion posicionFinal);
void entrenador_ir_hacia(entrenador* unEntrenador, t_posicion posicionFinal){
	printf("El entrenador partio de la posicion [%u %u]\n", unEntrenador->posicion.pos_x, unEntrenador->posicion.pos_y);

//	pthread_mutex_lock(&mutexPosicionEntrenador[unEntrenador->id]);
	while(!entrenador_llego_a(unEntrenador, posicionFinal)){
		entrenador_consumir_cpu(unEntrenador);
		entrenador_dar_un_paso_hacia(unEntrenador, posicionFinal);
		//		sem_post(&ConsumioCPU);
	}
//	pthread_mutex_unlock(&mutexPosicionEntrenador[unEntrenador->id]);
}

void desplazar_unidimensional(coordenada* posicionInicial, coordenada posicionFinal){
	int desplazamiento = posicionFinal/abs(posicionFinal);
	*posicionInicial += desplazamiento;
}

void entrenador_dar_un_paso_hacia(entrenador*unEntrenador, t_posicion posicionFinal){

	t_posicion* posicionActual = &unEntrenador->posicion;

	if(posicionActual->pos_x != posicionFinal.pos_x){
		desplazar_unidimensional(&posicionActual->pos_x, posicionFinal.pos_x);
	}

	else if(posicionActual->pos_y != posicionFinal.pos_y){
		desplazar_unidimensional(&posicionActual->pos_y, posicionFinal.pos_y);
	}

//	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El entrenador se desplazo a la posicion [%u %u]\n", unEntrenador->posicion.pos_x, unEntrenador->posicion.pos_y);
//	pthread_mutex_unlock(&Mutex_AndoLoggeando);

}

//**************************************************************
//TODO
//t_algoritmo cargar_algoritmo_planificacion(){ //TODO A FUTURO
//
//	char*algoritmoLeido = config_get_string_value(config,"ALGORITMO_PLANIFICACION");
//	  //	hacer un switch del criterio
//	  //	quantum=config_get_int_value(config,"QUANTUM");
//	  //	estimacion_inicial=config_get_int_value(config,"ESTIMACION_INICIAL");
//	/*-------*/
//
//
//	if(string_equals_ignore_case(algoritmoLeido, "FIFO")){
//
//		return FIFO;
//	}
//
//	if(string_equals_ignore_case(algoritmoLeido, "RR")){
//
//		return ROUND_ROBBIN;
//	}
//
//	if(string_equals_ignore_case(algoritmoLeido, "SJF-CD")){
//		return SJF_CD;
//	}
//
//	if(string_equals_ignore_case(algoritmoLeido, "SJF-SD")){
//		return SJF_SD;
//	}
//
//	error_show("Se leyo un algoritmo de planificacion desconocido");
//	exit(1);
//
//	return NULL;
//}
