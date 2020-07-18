#include "../../utils/listas/cr_list.h"
#include "../../team.h"

//Inicializar
void cargar_algoritmo_planificacion(){

	PROCESOS_SIN_FINALIZAR = cantidadDeEntrenadores;

	RETARDO_CICLO_CPU = config_get_int_value(config, "RETARDO_CICLO_CPU");
	char*algoritmoLeido = config_get_string_value(config,"ALGORITMO_PLANIFICACION");

	if(string_equals_ignore_case(algoritmoLeido, "FIFO")){
		inicializar_fifo();
	}

	else if(string_equals_ignore_case(algoritmoLeido, "RR")){
		numero QUANTUM = config_get_int_value(config, "QUANTUM");

		inicializar_rr(QUANTUM);
	}

	else{
		double ALFA = config_get_double_value(config, "ALFA");

		numero ESTIMACION_INICIAL = config_get_int_value(config, "ESTIMACION_INICIAL");

		pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
		log_info(event_logger, "Lei estimacion inicial %d", ESTIMACION_INICIAL);
		pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

		inicializar_sjf(ALFA, ESTIMACION_INICIAL, cantidadDeEntrenadores); //Ver cuando tiene valor

		actualizar_datos_del_entrenador = actualizar_datos_sjf;

		if(string_equals_ignore_case(algoritmoLeido, "SJF_CD")){
			criterio_de_desalojo = desalojo_en_sjf_cd;
		}
		else if(string_equals_ignore_case(algoritmoLeido, "SJF_SD")){
			criterio_de_desalojo = sin_desalojo;
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
	criterio_de_desalojo = sin_desalojo;
	actualizar_datos_del_entrenador = (void*) no_operation;
}

//Proximo a planificar
entrenador*proximo_segun_fifo(cola_entrenadores colaReady){
	return entrenador_esperar_y_desencolar(colaReady);
}

//Criterio de desalojo
bool sin_desalojo(){ //aplica tambien a sjf s/d
	return false;
}

//************************************************************************************
//RR

//inicializar
void inicializar_rr(numero QUANTUM){
	ALGORITMO_PLANIFICACION = ROUND_ROBBIN;
	DATOS_ALGORITMO.QUANTUM=config_get_int_value(config,"QUANTUM");
	proximo_a_ejecutar_segun_criterio = proximo_segun_rr;
	criterio_de_desalojo = desalojo_en_RR;
	actualizar_datos_del_entrenador = (void*) no_operation;
	MOTIVO_DESALOJO = "fin de QUANTUM";
}

entrenador*proximo_segun_rr(cola_entrenadores colaReady){
	return proximo_segun_fifo(colaReady);
}

//Desalojo
bool desalojo_en_RR(entrenador*unEntrenador, numero tiempo){
	return (tiempo/RETARDO_CICLO_CPU) > DATOS_ALGORITMO.QUANTUM && !cr_list_is_empty(entrenadoresReady);
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
	MOTIVO_DESALOJO = "estimacion mayor a la de otro entrenador";
}

//Proximo a planificar
entrenador*proximo_segun_sjf(cola_entrenadores colaReady){
	return cola_entrenador_con_menor_estimacion(entrenadoresReady);
}

numero entrenador_estimacion(entrenador*unEntrenador){
	return DATOS_ALGORITMO.sjf.estimaciones[unEntrenador->id];
}

numero entrenador_tiempo_restante(entrenador*unEntrenador, numero tiempoEnEjecucion){
	numero tiempoRestante = entrenador_estimacion(unEntrenador) - tiempoEnEjecucion;
	return ((int)tiempoRestante) > 0? tiempoRestante: 0;
}

//Desalojo
bool desalojo_en_sjf_cd(entrenador*unEntrenador, numero tiempoQueLlevaEjecutando){

	bool menorEstimacionQueTodos(void*otro){
		return  entrenador_tiempo_restante(unEntrenador, tiempoQueLlevaEjecutando) <= entrenador_estimacion(otro);
	}

	return !cr_list_all(entrenadoresReady, menorEstimacionQueTodos);
}

void actualizar_datos_sjf(entrenador*unEntrenador, numero tiempoUltimaEjecucion, bool desalojo){
	numero pid = unEntrenador->id;
	numero alfa = DATOS_ALGORITMO.sjf.alfa;
	numero* estimacion  = &DATOS_ALGORITMO.sjf.estimaciones[pid];
	numero estimacionAnterior = *estimacion;

	*estimacion = desalojo? entrenador_tiempo_restante(unEntrenador, tiempoUltimaEjecucion)
			     : alfa*tiempoUltimaEjecucion + (1-alfa)*estimacionAnterior;
}

//******************************* Funciones Auxiliares ********************************
numero estimacion_del_entrenador(entrenador*unEntrenador){
	return DATOS_ALGORITMO.sjf.estimaciones[unEntrenador->id];
}

void log_event_comparacion_de_estimaciones(entrenador* unEntrenador, entrenador* otro){
//	printf("\n Comparo %d <= %d\n", estimacion_del_entrenador(unEntrenador), estimacion_del_entrenador(otro));
}

entrenador*cola_entrenador_con_menor_estimacion(cola_entrenadores colaReady){

	sem_wait(&colaReady->hayMas);

	void*entrenador_con_menor_estimacion(void*unEntrenador, void*otro){
		if(!unEntrenador){
			return otro;
		}

		log_event_comparacion_de_estimaciones(unEntrenador, otro);

		return estimacion_del_entrenador(unEntrenador) <= estimacion_del_entrenador(otro)? unEntrenador: otro;
	}

	pthread_mutex_lock(&colaReady->mutex);
	entrenador*proximo = list_fold(colaReady->lista, NULL, entrenador_con_menor_estimacion);
	entrenadores_remover_del_equipo_a(colaReady->lista, proximo->id);
	pthread_mutex_unlock(&colaReady->mutex);

	return proximo;
}

void no_operation(){
	//No hace nada
}
