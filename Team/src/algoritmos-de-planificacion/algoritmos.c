#include "planificacion.h"
#include "../team.h"

//FIFO

//Proximo a planificar
entrenador*proximo_segun_fifo(cola_entrenadores colaReady){
	return entrenador_esperar_y_desencolar(colaReady);
}

//Criterio de desalojo
bool puede_seguir_sin_desalojo(){ //aplica tambien a sjf s/d
	return true;
}

//inicializar
void inicializar_fifo(){
	ALGORITMO_PLANIFICACION = FIFO;
	proximo_a_ejecutar_segun_criterio = proximo_segun_fifo;
	entrenador_puede_seguir_ejecutando_segun_algoritmo = puede_seguir_sin_desalojo;
}

//************************************************************************************
//RR

//inicializar
void inicializar_rr(numero QUANTUM){
	ALGORITMO_PLANIFICACION = ROUND_ROBBIN;
	DATOS_ALGORITMO.QUANTUM=config_get_int_value(config,"QUANTUM");

	//setear funciones
}

entrenador*proximo_segun_rr(cola_entrenadores colaReady){
	return proximo_segun_fifo(colaReady);
}

//Desalojo
bool puede_seguir_en_RR(entrenador*unEntrenador, numero tiempo){
	return tiempo<DATOS_ALGORITMO.QUANTUM && !cr_list_is_empty(entrenadoresReady);
}

//************************************************************************************
//SJF

//Inicializar
void inicializar_sjf(double alfa, numero estimacionInicial, numero cantidadDeProcesos){
	DATOS_ALGORITMO.sjf.alfa = alfa; //config_get_double_value(config, "ALFA");
	DATOS_ALGORITMO.sjf.estimaciones = malloc(sizeof(numero)*cantidadDeProcesos);

	int i=0;
	for(i=0; i<cantidadDeProcesos; i++){
		DATOS_ALGORITMO.sjf.estimaciones[i] = estimacionInicial;
		DATOS_ALGORITMO.sjf.tiempo[i] = 0;
	}

//	proximo_a_ejecutar_segun_criterio = proximo_segun_sjf;
}

////Proximo a planificar
//entrenador*proximo_segun_sjf(cola_entrenadores colaReady){
//	return cola_entrenador_con_menor_estimacion(entrenadoresReady);
//}

//Desalojo
//bool puede_seguir_en_sjf_cd(entrenador*unEntrenador, numero tiempoQueLlevaEjecutando){
//
//	bool menorEstimacionQueTodos(void*otro){
//		return DATOS_ALGORITMO.sjf.estimaciones[((entrenador*)otro)->id] > tiempoQueLlevaEjecutando;
//	}
//
//	return cr_list_all(entrenadoresReady, menorEstimacionQueTodos);
//}

//void actualizar_datos(entrenador*unEntrenador, numero tiempoUltimaEjecucion){
//	numero pid = unEntrenador->id;
//	numero alfa = DATOS_ALGORITMO.sjf.alfa;
//	numero estimacionAnterior = DATOS_ALGORITMO.sjf.estimaciones[pid];
//	numero* estimacion  = &DATOS_ALGORITMO.sjf.estimaciones[pid];
//
//	*estimacion = alfa*tiempoUltimaEjecucion + (1-alfa)*estimacionAnterior;
//}

//Finalizar
void finalizar_sjf(numero cantidadDeProcesos){
	//Duda: deberia usar otra funcion global, que tendrian que implementar todos pero en gral no harian nada
	int i=0;
	for(i=0; i<cantidadDeProcesos; i++){
		free(&DATOS_ALGORITMO.sjf.estimaciones[i]);
		free(&DATOS_ALGORITMO.sjf.tiempo[i]);
	}
}

//******************************* Funciones Auxiliares ********************************
numero estimacion_del_entrenador(entrenador*unEntrenador){
	return DATOS_ALGORITMO.sjf.estimaciones[unEntrenador->id];
}

entrenador*cola_entrenador_con_menor_estimacion(cola_entrenadores colaReady){

	void*entrenador_con_menor_estimacion(void*unEntrenador, void*otro){
		return estimacion_del_entrenador(unEntrenador) >= estimacion_del_entrenador(otro)? unEntrenador: otro;
	}

	pthread_mutex_lock(colaReady->mutex);
	entrenador*proximo = list_fold(colaReady->lista, NULL, entrenador_con_menor_estimacion);
	entrenadores_remover_del_equipo_a(colaReady->lista, proximo->id);
	pthread_mutex_unlock(colaReady->mutex);

	return proximo;
}
