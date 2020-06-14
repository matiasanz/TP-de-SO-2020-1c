#include "planificacion.h"
#include "../team.h"

//FIFO

//Proximo a planificar
entrenador*proximo_segun_fifo(cola_entrenadores colaReady){
	return entrenador_esperar_y_desencolar(colaReady);
}

//Criterio de desalojo
bool puede_seguir_sin_desalojo(){
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
	////  	datos_algoritmo.QUANTUM=config_get_int_value(config,"QUANTUM");
	//		ALGORITMO_PLANIFICACION = ROUND_ROBBIN;
}

//************************************************************************************
//SJF

//Inicializar
void inicializar_sjf(double alfa, numero estimacionInicial, numero cantidadDeProcesos){
	DATOS_ALGORITMO.sjf.alfa = alfa; //config_get_double_value(config, "ALFA");
	DATOS_ALGORITMO.sjf.estimaciones = malloc(sizeof(numero)*cantidadDeEntrenadores);

	int i=0;
	for(i=0; i<cantidadDeProcesos; i++){
		DATOS_ALGORITMO.sjf.estimaciones[i] = estimacionInicial;
		DATOS_ALGORITMO.sjf.tiempo[i] = 0;
	}
}

//Proximo a planificar

//******************************* Funciones Auxiliares ********************************
