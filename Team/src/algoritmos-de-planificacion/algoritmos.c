#include "planificacion.h"
#include "../team.h"

//Inicializar
void cargar_algoritmo_planificacion(){

	RETARDO_CICLO_CPU = config_get_int_value(config, "RETARDO_CICLO_CPU");
	char*algoritmoLeido = config_get_string_value(config,"ALGORITMO_PLANIFICACION");

	if(string_equals_ignore_case(algoritmoLeido, "FIFO")){
		inicializar_fifo();
		actualizar_datos_del_entrenador = no_operation;
	}

	else if(string_equals_ignore_case(algoritmoLeido, "RR")){
		numero QUANTUM = config_get_int_value(config, "QUANTUM");
		validar_quantum(QUANTUM);

		inicializar_rr(QUANTUM);
		actualizar_datos_del_entrenador = no_operation;
	}

	else{
		double ALFA = config_get_double_value(config, "ALFA");

		numero ESTIMACION_INICIAL = config_get_int_value(config, "ESTIMACION_INICIAL");
		printf("Lei estimacion inicial %d", ESTIMACION_INICIAL);

		inicializar_sjf(ALFA, ESTIMACION_INICIAL, cantidadDeEntrenadores); //Ver cuando tiene valor

		actualizar_datos_del_entrenador = actualizar_datos_sjf;

		if(string_equals_ignore_case(algoritmoLeido, "SJF_CD")){
			entrenador_puede_seguir_ejecutando_segun_algoritmo = puede_seguir_en_sjf_cd;
		}
		else if(string_equals_ignore_case(algoritmoLeido, "SJF_SD")){
			entrenador_puede_seguir_ejecutando_segun_algoritmo = puede_seguir_sin_desalojo;
		}

		else{
			pthread_mutex_lock(&Mutex_AndoLoggeando);
			log_error(logger, "Se ha leido un algoritmo de planificacion desconocido");
			pthread_mutex_unlock(&Mutex_AndoLoggeando);
			exit(1);
		}
	}
}


//********************************* Algoritmos ***************************************

// FIFO
//inicializar
void inicializar_fifo(){
	ALGORITMO_PLANIFICACION = FIFO;
	proximo_a_ejecutar_segun_criterio = proximo_segun_fifo;
	entrenador_puede_seguir_ejecutando_segun_algoritmo = puede_seguir_sin_desalojo;
}

//Proximo a planificar
entrenador*proximo_segun_fifo(cola_entrenadores colaReady){
	return entrenador_esperar_y_desencolar(colaReady);
}

//Criterio de desalojo
bool puede_seguir_sin_desalojo(){ //aplica tambien a sjf s/d
	return true;
}

//************************************************************************************
//RR

//inicializar
void inicializar_rr(numero QUANTUM){
	ALGORITMO_PLANIFICACION = ROUND_ROBBIN;
	DATOS_ALGORITMO.QUANTUM=config_get_int_value(config,"QUANTUM");
	proximo_a_ejecutar_segun_criterio = proximo_segun_rr;
	entrenador_puede_seguir_ejecutando_segun_algoritmo = puede_seguir_en_RR;
}

entrenador*proximo_segun_rr(cola_entrenadores colaReady){
	return proximo_segun_fifo(colaReady);
}

//Desalojo
bool puede_seguir_en_RR(entrenador*unEntrenador, numero tiempo){
	return tiempo<=DATOS_ALGORITMO.QUANTUM || cr_list_is_empty(entrenadoresReady);
}

void validar_quantum(numero QUANTUM){
	if(QUANTUM < RETARDO_CICLO_CPU){
		pthread_mutex_lock(&Mutex_AndoLoggeando);
		log_error(logger, "Se ha leido un Quantum menor al tiempo de cada ciclo de CPU");
		pthread_mutex_unlock(&Mutex_AndoLoggeando);
		exit(1);
	}
}

//************************************************************************************
//SJF

//Inicializar
void inicializar_sjf(double alfa, numero estimacionInicial, numero cantidadDeProcesos){
	DATOS_ALGORITMO.sjf.alfa = alfa;
	DATOS_ALGORITMO.sjf.estimaciones = malloc(sizeof(numero)*cantidadDeProcesos);

	int i=0;
	for(i=0; i<cantidadDeProcesos; i++){
		DATOS_ALGORITMO.sjf.estimaciones[i] = estimacionInicial;
	}

	proximo_a_ejecutar_segun_criterio = proximo_segun_sjf;
}

//Proximo a planificar
entrenador*proximo_segun_sjf(cola_entrenadores colaReady){
	return cola_entrenador_con_menor_estimacion(entrenadoresReady);
}

//Desalojo
bool puede_seguir_en_sjf_cd(entrenador*unEntrenador, numero tiempoQueLlevaEjecutando){

	bool menorEstimacionQueTodos(void*otro){
		return DATOS_ALGORITMO.sjf.estimaciones[((entrenador*)otro)->id] > tiempoQueLlevaEjecutando;
	}

	return cr_list_all(entrenadoresReady, menorEstimacionQueTodos);
}

void actualizar_datos_sjf(entrenador*unEntrenador, numero tiempoUltimaEjecucion){
	numero pid = unEntrenador->id;
	numero alfa = DATOS_ALGORITMO.sjf.alfa;
	numero* estimacion  = &DATOS_ALGORITMO.sjf.estimaciones[pid];
	numero estimacionAnterior = *estimacion;

	*estimacion = alfa*tiempoUltimaEjecucion + (1-alfa)*estimacionAnterior;
}

//******************************* Funciones Auxiliares ********************************
numero estimacion_del_entrenador(entrenador*unEntrenador){
	return DATOS_ALGORITMO.sjf.estimaciones[unEntrenador->id];
}

entrenador*cola_entrenador_con_menor_estimacion(cola_entrenadores colaReady){

	sem_wait(&colaReady->hayMas);

	void*entrenador_con_menor_estimacion(void*unEntrenador, void*otro){
		if(!unEntrenador){
			return otro;
		}

		printf("\n %d <= %d\n", estimacion_del_entrenador(unEntrenador), estimacion_del_entrenador(otro));
		return estimacion_del_entrenador(unEntrenador) <= estimacion_del_entrenador(otro)? unEntrenador: otro;
	}

	pthread_mutex_lock(colaReady->mutex);
	entrenador*proximo = list_fold(colaReady->lista, NULL, entrenador_con_menor_estimacion);
	entrenadores_remover_del_equipo_a(colaReady->lista, proximo->id);
	pthread_mutex_unlock(colaReady->mutex);

	return proximo;
}

void no_operation(){
	//No hace nada
}
