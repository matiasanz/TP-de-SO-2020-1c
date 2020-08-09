#include "../../utils/listas/cr_list.h"
#include "../../team.h"
#include "../../utils/team_logs.h"

t_algoritmo_planificacion config_get_algoritmo(t_config* archivoConfig){
	char*algoritmoLeido = config_get_string_value(config,"ALGORITMO_PLANIFICACION");
	return string_equals_ignore_case(algoritmoLeido, "FIFO")? FIFO
		 : string_equals_ignore_case(algoritmoLeido, "RR")? ROUND_ROBBIN
	     : string_equals_ignore_case(algoritmoLeido, "SJF_SD")? SJF_SD
	     : string_equals_ignore_case(algoritmoLeido, "SJF_CD")? SJF_CD
	     : -1;
}

//Inicializar
void cargar_algoritmo_planificacion(){

	PROCESOS_SIN_FINALIZAR = cantidadDeEntrenadores;
	RETARDO_CICLO_CPU = config_get_int_value(config, "RETARDO_CICLO_CPU");
	ALGORITMO_PLANIFICACION = config_get_algoritmo(config);

	switch(ALGORITMO_PLANIFICACION){

		case FIFO: {
			inicializar_fifo();
			break;
		}

		case ROUND_ROBBIN : {
			numero quantum = config_get_int_value(config, "QUANTUM");
			inicializar_rr(quantum);
			break;
		}

		case SJF_SD:
		case SJF_CD: {

			double alfa = config_get_double_value(config, "ALFA");
			numero estimacionInicial = config_get_int_value(config, "ESTIMACION_INICIAL");

			log_event_datos_sjf(alfa, estimacionInicial);

			inicializar_sjf(alfa, estimacionInicial, cantidadDeEntrenadores); //Ver cuando tiene valor

			criterio_de_desalojo = (ALGORITMO_PLANIFICACION == SJF_CD)? desalojo_en_sjf_cd: sin_desalojo;

			break;
		}

		default: {
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
	MOTIVO_DESALOJO = "fin de QUANTUM";

	proximo_a_ejecutar_segun_criterio = proximo_segun_rr;
	criterio_de_desalojo = desalojo_en_rr;
	actualizar_datos_del_entrenador = (void*) no_operation;
}

entrenador*proximo_segun_rr(cola_entrenadores colaReady){
	return proximo_segun_fifo(colaReady);
}

//Desalojo
bool desalojo_en_rr(entrenador*unEntrenador, numero tiempo){
	return (tiempo/RETARDO_CICLO_CPU) > DATOS_ALGORITMO.QUANTUM;
}

//************************************************************************************
//SJF

//Inicializar
void inicializar_sjf(double alfa, numero estimacionInicial, numero cantidadDeProcesos){
	DATOS_ALGORITMO.sjf.alfa = alfa;
	DATOS_ALGORITMO.sjf.estimaciones = malloc(sizeof(numero)*cantidadDeProcesos);
	DATOS_ALGORITMO.sjf.tiempoRafagaActual = malloc(sizeof(numero)*cantidadDeProcesos);

	int i=0;
	for(i=0; i<cantidadDeProcesos; i++){
		DATOS_ALGORITMO.sjf.estimaciones[i] = estimacionInicial;
		DATOS_ALGORITMO.sjf.tiempoRafagaActual[i] = 0;
	}

	proximo_a_ejecutar_segun_criterio = proximo_segun_sjf;
	actualizar_datos_del_entrenador = actualizar_datos_sjf;

	MOTIVO_DESALOJO = "estimacion mayor a la de otro entrenador";
}

//Proximo a planificar
entrenador*proximo_segun_sjf(cola_entrenadores colaReady){
	return cola_entrenador_con_menor_estimacion(entrenadoresReady);
}

//Desalojo
bool desalojo_en_sjf_cd(entrenador*unEntrenador, numero tiempoQueLlevaEjecutando){

	bool menorEstimacionQueTodos(void*otro){
		return  entrenador_tiempo_restante(unEntrenador, tiempoQueLlevaEjecutando) <= entrenador_estimacion(otro);
	}

	return !cr_list_all(entrenadoresReady, menorEstimacionQueTodos);
}

void actualizar_datos_sjf(entrenador*unEntrenador, numero tiempoUltimaEjecucion, bool finDeRafaga){
	numero pid = unEntrenador->id;
	numero* estimacion  = &DATOS_ALGORITMO.sjf.estimaciones[pid];
	numero* tiempo = &DATOS_ALGORITMO.sjf.tiempoRafagaActual[pid];

	*tiempo += tiempoUltimaEjecucion;

	if(finDeRafaga){
		numero estimacionAnterior = *estimacion;
		numero alfa = DATOS_ALGORITMO.sjf.alfa;

		*estimacion = alfa*(*tiempo) + (1-alfa)*estimacionAnterior;
		*tiempo = 0;
	}

	else{
		*estimacion = entrenador_tiempo_restante(unEntrenador, tiempoUltimaEjecucion);
	}
}

//******************************* Funciones Auxiliares ********************************
numero entrenador_tiempo_rafaga_estimado(entrenador*unEntrenador){
	return DATOS_ALGORITMO.sjf.estimaciones[unEntrenador->id];
}

numero entrenador_tiempo_rafaga_cumplido(entrenador*unEntrenador){
	return DATOS_ALGORITMO.sjf.tiempoRafagaActual[unEntrenador->id];
}

numero entrenador_estimacion(entrenador*unEntrenador){
	numero estimadoRafaga     = entrenador_tiempo_rafaga_estimado(unEntrenador);
	numero tiempoRafagaActual = entrenador_tiempo_rafaga_cumplido(unEntrenador);

	return estimadoRafaga - tiempoRafagaActual;
}

numero entrenador_tiempo_restante(entrenador*unEntrenador, numero tiempoEnEjecucion){
	numero tiempoRestante = entrenador_estimacion(unEntrenador) - tiempoEnEjecucion;
	return ((int)tiempoRestante) > 0? tiempoRestante: 0;
}

entrenador*cola_entrenador_con_menor_estimacion(cola_entrenadores colaReady){

	sem_wait(&colaReady->hayMas);

	void*entrenador_con_menor_estimacion(void*unEntrenador, void*otro){
		if(!unEntrenador){
			return otro;
		}

		log_event_comparacion_de_estimaciones(unEntrenador, otro);

		return entrenador_estimacion(unEntrenador) <= entrenador_estimacion(otro)? unEntrenador: otro;
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
