#include "../../utils/listas/cr_list.h"
#include "../../utils/team_logs.h"
#include "../../team.h"

t_algoritmo_planificacion config_get_algoritmo(t_config* archivoConfig){
	char*algoritmoLeido = config_get_string_value(config,"ALGORITMO_PLANIFICACION");
	return string_equals_ignore_case(algoritmoLeido, "FIFO")? FIFO
		 : string_equals_ignore_case(algoritmoLeido, "RR")? ROUND_ROBBIN
		 : string_equals_ignore_case(algoritmoLeido, "VRR")? VIRTUAL_ROUND_ROBBIN
	     : string_equals_ignore_case(algoritmoLeido, "SJF_SD")? SJF_SD
	     : string_equals_ignore_case(algoritmoLeido, "SJF_CD")? SJF_CD
	     : string_equals_ignore_case(algoritmoLeido, "HRRN")? HRRN
	     : -1;
}

//Inicializar
void cargar_algoritmo_planificacion(){

	PROCESOS_SIN_FINALIZAR = cantidadDeEntrenadores;
	RETARDO_CICLO_CPU = config_get_retardo_ciclo_cpu();
	ALGORITMO_PLANIFICACION = config_get_algoritmo(config);

	switch(ALGORITMO_PLANIFICACION){

		case FIFO: {
			inicializar_fifo();
			break;
		}

		case ROUND_ROBBIN : {
			numero quantum = config_get_quantum();
			inicializar_rr(quantum);
			break;
		}

		case SJF_SD:
		case SJF_CD: {

			double alfa = config_get_alfa();
			numero estimacionInicial = config_get_estimacion_inicial();

			log_event_datos_sjf(alfa, estimacionInicial);

			inicializar_sjf(alfa, estimacionInicial, cantidadDeEntrenadores);

			criterio_de_desalojo = (ALGORITMO_PLANIFICACION == SJF_CD)? desalojo_en_sjf_cd: sin_desalojo;

			break;
		}

		case HRRN: {
			double alfa = config_get_alfa();
			numero estimacionInicial = config_get_estimacion_inicial();

			inicializar_hrrn(alfa, cantidadDeEntrenadores, estimacionInicial);

			break;
		}

		case VIRTUAL_ROUND_ROBBIN: {
			numero quantum = config_get_quantum();
			inicializar_vrr(quantum, cantidadDeEntrenadores);
			break;
		}

		default: {
			throw_new_algoritmo_desconocido_exception();
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
# define MOTIVO_DESALOJO_RR "fin de Quantum"

//inicializar
void inicializar_rr(numero quantum){
	DATOS_ALGORITMO.QUANTUM = quantum;
	MOTIVO_DESALOJO = MOTIVO_DESALOJO_RR;

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

	bool menorEstimacionQueElEntrenador(void*otro){
		t_estimador estimador = DATOS_ALGORITMO.sjf;
		return  entrenador_tiempo_restante(unEntrenador, tiempoQueLlevaEjecutando, estimador) > entrenador_estimacion(otro, estimador);
	}

	return cr_list_any(entrenadoresReady, menorEstimacionQueElEntrenador);
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

//************************************************************************************
//VRR
void inicializar_vrr(numero quantum, numero cantidadDeProcesos){
	datos_vrr vrr = (datos_vrr){
		quantum,
		malloc(sizeof(numero)*cantidadDeProcesos)
	};

	int i;
	for(i=0; i<cantidadDeProcesos; i++){
		vrr.quantumConsumido[i]=0;
	}

	DATOS_ALGORITMO.vrr = vrr;

	proximo_a_ejecutar_segun_criterio = proximo_segun_vrr;
	criterio_de_desalojo = desalojo_en_vrr;
	MOTIVO_DESALOJO = MOTIVO_DESALOJO_RR;
	actualizar_datos_del_entrenador = actualizar_datos_vrr;
}

bool entrenador_ready_plus(entrenador* unEntrenador){
	return entrenador_virtual_quantum_consumido(unEntrenador);
}

entrenador* cola_get_proximo_en_ready_plus(cola_entrenadores colaReady){
	return list_remove_by_condition(colaReady->lista, (bool(*)(void*)) &entrenador_ready_plus);
}

entrenador* proximo_segun_vrr(cola_entrenadores colaReady){
	sem_wait(&colaReady->hayMas);
	pthread_mutex_lock(&colaReady->mutex);
	entrenador* entrenadorReadyPlus = cola_get_proximo_en_ready_plus(colaReady);
	entrenador* proximo = entrenadorReadyPlus? entrenadorReadyPlus: list_remove(colaReady->lista, 0);
	pthread_mutex_unlock(&colaReady->mutex);

	return proximo;
}

bool desalojo_en_vrr(entrenador*unEntrenador, numero tiempoQueLlevaEjecutando){
	numero quantumConsumido = entrenador_virtual_quantum_por_ejecutar(unEntrenador, tiempoQueLlevaEjecutando);
	return DATOS_ALGORITMO.vrr.QUANTUM < quantumConsumido;
}

void actualizar_datos_vrr(entrenador*unEntrenador, numero tiempoUltimaEjecucion, bool finDeRafaga){

	numero maxQuantum = DATOS_ALGORITMO.vrr.QUANTUM;
	numero* quantumAcumulado = &DATOS_ALGORITMO.vrr.quantumConsumido[unEntrenador->id];
		  * quantumAcumulado = entrenador_virtual_quantum_por_ejecutar(unEntrenador, tiempoUltimaEjecucion);

	if(*quantumAcumulado == maxQuantum){
		*quantumAcumulado = 0;
	}
}

//Auxiliares
numero entrenador_virtual_quantum_consumido(entrenador* unEntrenador){
	return DATOS_ALGORITMO.vrr.quantumConsumido[unEntrenador->id];
}

numero entrenador_virtual_quantum_por_ejecutar(entrenador* unEntrenador, numero tiempo){
	return entrenador_virtual_quantum_consumido(unEntrenador) + tiempo/RETARDO_CICLO_CPU;
}


//******************************* Funciones Auxiliares ********************************

//TAD Estimador
t_estimador estimador_create(double alfa, numero cantidadDeProcesos, numero estimacionInicial){
	t_estimador estimador = (t_estimador){
		malloc(sizeof(numero)*cantidadDeProcesos),
		malloc(sizeof(numero)*cantidadDeProcesos),
		alfa
	};

	int i;
	for(i=0; i<cantidadDeProcesos; i++){
		estimador.estimaciones[i] = estimacionInicial;
		estimador.tiempoRafagaActual[i] = 0;
	}

	return estimador;
}

numero entrenador_tiempo_rafaga_estimado(entrenador*unEntrenador, t_estimador sjf){
	return sjf.estimaciones[unEntrenador->id];
}

numero entrenador_tiempo_rafaga_cumplido(entrenador*unEntrenador, t_estimador sjf){
	return sjf.tiempoRafagaActual[unEntrenador->id];
}

numero entrenador_estimacion(entrenador*unEntrenador, t_estimador sjf){
	numero estimadoRafaga     = entrenador_tiempo_rafaga_estimado(unEntrenador, sjf);
	numero tiempoRafagaActual = entrenador_tiempo_rafaga_cumplido(unEntrenador, sjf);

	return estimadoRafaga - tiempoRafagaActual;
}

numero entrenador_tiempo_restante(entrenador*unEntrenador, numero tiempoEnEjecucion, t_estimador sjf){
	numero tiempoRestante = entrenador_estimacion(unEntrenador, sjf) - tiempoEnEjecucion;
	return ((int)tiempoRestante) > 0? tiempoRestante: 0;
}

void actualizar_estimador(t_estimador* estimador, entrenador* unEntrenador, numero tiempoUltimaEjecucion, bool finDeRafaga){
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


//*****************************
// TAD HRRN
numero* entrenador_get_espera(entrenador*unEntrenador, datos_hrrn hrrn){
	return &hrrn.espera[unEntrenador->id];
}

numero entrenador_response_ratio(entrenador*unEntrenador, datos_hrrn hrrn){
	numero* espera = entrenador_get_espera(unEntrenador, hrrn);
	numero tiempoRestante = entrenador_estimacion(unEntrenador, hrrn.estimador);
	return (*espera + tiempoRestante)/tiempoRestante;
}

entrenador*entrenador_con_menor_estimacion(entrenador*unEntrenador, entrenador*otro){
	t_estimador estimador = DATOS_ALGORITMO.sjf;
	log_event_comparacion_de_estimaciones(unEntrenador, otro);
	return entrenador_estimacion(unEntrenador, estimador) <= entrenador_estimacion(otro, estimador)? unEntrenador: otro;
}

entrenador* entrenador_con_menor_response_ratio(entrenador* unEntrenador, entrenador* otroEntrenador){
	return entrenador_response_ratio(unEntrenador, DATOS_ALGORITMO.hrrn) <= entrenador_response_ratio(otroEntrenador, DATOS_ALGORITMO.hrrn)? unEntrenador: otroEntrenador;
}

entrenador* cola_mejor_entrenador(cola_entrenadores colaReady, entrenador*(*comparador)(entrenador*, entrenador*)){
	sem_wait(&colaReady->hayMas);

	void* mejor_entrenador(void* unEntrenador, void*otro){
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
