#include "planificacion.h"

#include "../team.h"

//Proximo a planificar
entrenador*proximo_segun_fifo(cola_entrenadores colaReady){
	return entrenador_esperar_y_desencolar(colaReady);
}

//Puede seguir
bool puede_seguir_en_fifo(){
	return true;
}
bool puede_seguir_en_sjf_sd(){
	return true;
}

//Inicializar
void cargar_algoritmo_planificacion(){ //TODO A FUTURO

	char*algoritmoLeido = config_get_string_value(config,"ALGORITMO_PLANIFICACION");

	if(string_equals_ignore_case(algoritmoLeido, "FIFO")){
		ALGORITMO_PLANIFICACION = FIFO;
		proximo_a_ejecutar_segun_criterio = proximo_segun_fifo;
		entrenador_puede_seguir_ejecutando_segun_algoritmo = puede_seguir_en_fifo;
	}

//	else if(string_equals_ignore_case(algoritmoLeido, "RR")){
////  	datos_algoritmo.QUANTUM=config_get_int_value(config,"QUANTUM");
//		ALGORITMO_PLANIFICACION = ROUND_ROBBIN;
//	}
//
//	else{ //SJF //Sin desalojo va a leer lo mismo que con
//		ALGORITMO_PLANIFICACION = string_equals_ignore_case(algoritmoLeido, "SJF-CD")? SJF_CD: SJF_SD;
//		//	estimacion_inicial=config_get_int_value(config,"ESTIMACION_INICIAL");
//		//	alfa=config_get_int_value(config,"ALFA");
//		//  datosAlgoritmo.tiempo = malloc(sizeof(numero)*cantidadDeEntrenadores);
//		//  datosAlgoritmo.estimaciones = malloc(sizeof(numero)*cantidadDeEntrenadores);
//	}

	else{
		printf("El algoritmo %s no esta implementado... aun ;)", algoritmoLeido);
		exit(1);
	}

	RETARDO_CICLO_CPU = config_get_int_value(config, "RETARDO_CICLO_CPU");
}

//Usar en funciones que requieren cpu del entrenador. Ej ir a (Ver abajo de todo)
void entrenador_consumir_cpu(entrenador*unEntrenador){
	sem_wait(&EjecutarEntrenador[unEntrenador->id]);
	sleep(RETARDO_CICLO_CPU);
}//	sem_signal(&ConsumioCpu); se deberia hacer fuera de la funcion, despues de la actividad que tenia que hacer

void entrenador_consumir_N_cpu(entrenador*unEntrenador, numero cantidad){
	int i;
	for(i=0 /*=1*/; i<cantidad; i++){
		entrenador_consumir_cpu(unEntrenador);
//		sem_post(&consumioCpu);
	}
//Ver si conviene hacer un signal(cpuConsumido) de menos para que lo haga despues de lo que tenga que hacer

//	entrenador_consumir_cpu(unEntrenador);
}

bool entrenador_esta_ejecutando(entrenador*unEntrenador){
	pthread_mutex_lock(&mutexEstadoEntrenador[unEntrenador->id]);
	bool estaEjecutando = entrenador_en_estado(unEntrenador, EXECUTE);
	pthread_mutex_unlock(&mutexEstadoEntrenador[unEntrenador->id]);
	return estaEjecutando;
}

bool entrenador_termino_de_ejecutar(entrenador*unEntrenador){
	bool termino = unEntrenador && !entrenador_esta_ejecutando(unEntrenador);
	return termino;
}

void ejecutar_entrenador(entrenador* unEntrenador){
	numero tiempo;
	entrenador_pasar_a(unEntrenador, EXECUTE, "Es su turno de ejecutar");

	for(tiempo=0; !entrenador_termino_de_ejecutar(unEntrenador); tiempo+=RETARDO_CICLO_CPU){

		if(!entrenador_puede_seguir_ejecutando_segun_algoritmo(unEntrenador, tiempo)){
			entrenador_pasar_a(unEntrenador, READY, "Ha sido desalojado por algoritmo de planificacion");
			break;
		}

		puts("***************************************** LE DOY CPU");
		sem_post(&EjecutarEntrenador[unEntrenador->id]);

		puts("***************************************** ESPERO INTERESADOS");

		sem_wait(&FinDeCiclo_CPU);//		sem_wait(&ConsumioCpu);
		puts("***************************************** CONSUMIO CPU");

	}
}

//Ver si con orden superior se puede desarmar el switch
bool HC_puede_seguir_ejecutando_segun_algoritmo(entrenador*unEntrenador, numero tiempo, t_algoritmo_planificacion algoritmo){
	return true;
}



//{
//	switch(algoritmo){
//		case FIFO: ;
//		/*no break*/
//		case SJF_SD: ;
//			return true;
//
////		case ROUND_ROBBIN: {
////			return tiempo<datosAlgoritmo.QUANTUM || cr_list_is_empty(entrenadoresReady);
////			break;
////		}
//
////		case SJF_CD{
////			return tiempo <= menor_estimacion(entrenadoresReady); //resolver con fold
////		}
//
//		default : {
//			puts("El algoritmo leido no se encuentra implementado... aun! ;)");
//			exit(1);
//			return false;
//		}
//	}
//}

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
//Ejemplo de uso del cpu

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
	int desplazamiento = (posicionFinal > *posicionInicial) - (posicionFinal < *posicionInicial);
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
