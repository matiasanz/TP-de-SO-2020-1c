#include "../../utils/listas/cr_list.h"
#include "../../team.h"
#include "../../utils/team_logs.h"

numero* entrenador_get_espera(entrenador*unEntrenador, datos_hrrn hrrn);

t_algoritmo_planificacion config_get_algoritmo(t_config* archivoConfig){
	char*algoritmoLeido = config_get_string_value(config,"ALGORITMO_PLANIFICACION");
	return string_equals_ignore_case(algoritmoLeido, "FIFO")? FIFO
		 : string_equals_ignore_case(algoritmoLeido, "RR")? ROUND_ROBBIN
	     : string_equals_ignore_case(algoritmoLeido, "SJF_SD")? SJF_SD
	     : string_equals_ignore_case(algoritmoLeido, "SJF_CD")? SJF_CD
	     : string_equals_ignore_case(algoritmoLeido, "HRRN")? HRRN
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

		case HRRN: {
			double alfa = config_get_double_value(config, "ALFA");
			numero estimacionInicial = config_get_int_value(config, "ESTIMACION_INICIAL");

			inicializar_hrrn(alfa, cantidadDeEntrenadores, estimacionInicial);

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
	DATOS_ALGORITMO.sjf = estimador_create(alfa, estimacionInicial, cantidadDeProcesos);

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
		datos_sjf estimador = DATOS_ALGORITMO.sjf;
		return  entrenador_tiempo_restante(unEntrenador, tiempoQueLlevaEjecutando, estimador) <= entrenador_estimacion(otro, estimador);
	}

	return !cr_list_all(entrenadoresReady, menorEstimacionQueTodos);
}

void actualizar_datos_sjf(entrenador*unEntrenador, numero tiempoUltimaEjecucion, bool finDeRafaga){
	actualizar_estimador(&DATOS_ALGORITMO.sjf, unEntrenador, tiempoUltimaEjecucion, finDeRafaga);
}

//***********************************************************************************
//HRRN
void inicializar_hrrn(double alfa, numero cantidadDeProcesos, numero estimacionInicial){

	DATOS_ALGORITMO.hrrn.estimador = estimador_create(alfa, cantidadDeProcesos, estimacionInicial);
	DATOS_ALGORITMO.hrrn.espera = malloc(sizeof(numero)*cantidadDeProcesos);

	int i;
	for(i=0; i<cantidadDeEntrenadores; i++){
		DATOS_ALGORITMO.hrrn.espera[i]=0;
	}

	proximo_a_ejecutar_segun_criterio = proximo_segun_hrrn;
	actualizar_datos_del_entrenador = actualizar_datos_hrrn;
	criterio_de_desalojo = sin_desalojo;
}

entrenador* proximo_segun_hrrn(cola_entrenadores colaReady){
	return cola_entrenador_con_menor_response_ratio(colaReady);
}

void actualizar_datos_hrrn(entrenador*unEntrenador, numero tiempoUltimaEjecucion, bool finDeRafaga){
	actualizar_estimador(&DATOS_ALGORITMO.hrrn.estimador, unEntrenador, tiempoUltimaEjecucion, finDeRafaga);

	void incrementar_espera(void* entrenadorEsperando){
		numero* espera = entrenador_get_espera(entrenadorEsperando, DATOS_ALGORITMO.hrrn);
		(*espera)+=tiempoUltimaEjecucion;
	}

	cr_list_iterate(entrenadoresReady, incrementar_espera);
}

//******************************* Funciones Auxiliares ********************************

//datos sjf
datos_sjf estimador_create(double alfa, numero cantidadDeProcesos, numero estimacionInicial){
	datos_sjf sjf = (datos_sjf){
		malloc(sizeof(numero)*cantidadDeProcesos),
		malloc(sizeof(numero)*cantidadDeProcesos),
		alfa
	};

	int i;
	for(i=0; i<cantidadDeProcesos; i++){
		sjf.estimaciones[i] = estimacionInicial;
		sjf.tiempoRafagaActual[i] = 0;

		printf("Est %d: %u, T=%u\n", i, sjf.estimaciones[i], sjf.tiempoRafagaActual[i]);
	}

	return sjf;
}

numero entrenador_tiempo_rafaga_estimado(entrenador*unEntrenador, datos_sjf sjf){
	return sjf.estimaciones[unEntrenador->id];
}

numero entrenador_tiempo_rafaga_cumplido(entrenador*unEntrenador, datos_sjf sjf){
	return sjf.tiempoRafagaActual[unEntrenador->id];
}

numero entrenador_estimacion(entrenador*unEntrenador, datos_sjf sjf){
	numero estimadoRafaga     = entrenador_tiempo_rafaga_estimado(unEntrenador, sjf);
	numero tiempoRafagaActual = entrenador_tiempo_rafaga_cumplido(unEntrenador, sjf);

	return estimadoRafaga - tiempoRafagaActual;
}

numero entrenador_tiempo_restante(entrenador*unEntrenador, numero tiempoEnEjecucion, datos_sjf sjf){
	numero tiempoRestante = entrenador_estimacion(unEntrenador, sjf) - tiempoEnEjecucion;
	return ((int)tiempoRestante) > 0? tiempoRestante: 0;
}

void actualizar_estimador(datos_sjf* estimador, entrenador* unEntrenador, numero tiempoUltimaEjecucion, bool finDeRafaga){
	numero pid = unEntrenador->id;
	numero* estimacion  = &estimador->estimaciones[pid];
	numero* tiempo      = &estimador->tiempoRafagaActual[pid];

	*tiempo += tiempoUltimaEjecucion;

	if(finDeRafaga){
		numero estimacionAnterior = *estimacion;
		double alfa = estimador->alfa;

		*estimacion = alfa*(*tiempo) + (1-alfa)*estimacionAnterior;
		*tiempo = 0;
	}

	else{
		*estimacion = entrenador_tiempo_restante(unEntrenador, tiempoUltimaEjecucion, *estimador);
	}
}

numero* entrenador_get_espera(entrenador*unEntrenador, datos_hrrn hrrn){
	return &hrrn.espera[unEntrenador->id];
}

numero entrenador_response_ratio(entrenador*unEntrenador, datos_hrrn hrrn){
	numero* espera = entrenador_get_espera(unEntrenador, hrrn);
	numero tiempoRestante = entrenador_estimacion(unEntrenador, hrrn.estimador);
	return (*espera + tiempoRestante)/tiempoRestante;
}

entrenador*entrenador_con_menor_estimacion(entrenador*unEntrenador, entrenador*otro){
	datos_sjf estimador = DATOS_ALGORITMO.sjf;
	log_event_comparacion_de_estimaciones(unEntrenador, otro);
	return entrenador_estimacion(unEntrenador, estimador) <= entrenador_estimacion(otro, estimador)? unEntrenador: otro;
}

entrenador* entrenador_con_menor_response_ratio(entrenador* unEntrenador, entrenador* otroEntrenador){
	return entrenador_response_ratio(unEntrenador, DATOS_ALGORITMO.hrrn) <= entrenador_response_ratio(otroEntrenador, DATOS_ALGORITMO.hrrn)? unEntrenador: otroEntrenador;
}

entrenador* cola_mejor_entrenador(cola_entrenadores colaReady, entrenador*(*comparador)(entrenador*, entrenador*)){
	sem_wait(&colaReady->hayMas);

	void* mejor_entrenador(void* unEntrenador, void*otro){

		if(unEntrenador) printf("id: %u\n", ((entrenador*)unEntrenador)->id);

		return unEntrenador? comparador(unEntrenador, otro): otro;
	}

	pthread_mutex_lock(&colaReady->mutex);
	entrenador*proximo = list_fold(colaReady->lista, NULL, mejor_entrenador);
	entrenadores_remover_del_equipo_a(colaReady->lista, proximo->id);
	pthread_mutex_unlock(&colaReady->mutex);

	return proximo;
}

entrenador*cola_entrenador_con_menor_estimacion(cola_entrenadores colaReady){
	return cola_mejor_entrenador(colaReady, &entrenador_con_menor_estimacion);
}

entrenador*cola_entrenador_con_menor_response_ratio(cola_entrenadores colaReady){
	return cola_mejor_entrenador(colaReady, &entrenador_con_menor_response_ratio);
}

void no_operation(){
	//No hace nada
}
