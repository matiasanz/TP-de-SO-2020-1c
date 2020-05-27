/*
 ============================================================================
 Name        : prueba_team.c
 Author      : C-renito Casero
 Version     : Prueba
 Copyright   : Your copyright notice
 Description : Pruebas del Proceso Team in C, Ansi-style
 ============================================================================
 */
#include "team.h"
#include "hilos del team/hilos_team.h"
//#include "tests/tests_team.o"

int main(void) {

	team_inicializar();

	Get_pokemones(objetivosGlobales);

	log_info(event_logger, "\n\n****************************************\n¡¡¡Jellou World Team!!!\n"); /* prints !!!Hello World!!! */
	log_info(logger, "\n\n");

		mensajesAPPEARD = cr_list_create();
		mensajesCAUGHT = cr_list_create();
		mensajesLOCALIZED = cr_list_create();

	inicializar_hilos();

	finalizar_hilos();

	finalizar_semaforos();

	log_info(event_logger, "chau team\n****************************************");

	cr_list_destroy(mensajesAPPEARD);
	cr_list_destroy(mensajesCAUGHT);
	cr_list_destroy(mensajesLOCALIZED);

	return team_exit(); //Destruye listas, cierra config, cierra log
}

																/*Fin del main*/
//**********************************************************************************

						/*Implementacion de funciones*/

void team_inicializar(){
	config=config_create("config/team.config");

	logger=log_create("log/team.log","TEAM",true,LOG_LEVEL_INFO);

	event_logger = log_create("log/team_event.log", "TEAM_EVENT", true, LOG_LEVEL_INFO);

/*Gustavo*/
//	subscribpcion_colas(); (?) Gustavo
//	tiempo_reconexion=config_get_int_value(config,"TIEMPO_RECONEXION");
//	retardo_ciclo_cpu=config_get_int_value(config,"RETARDO_CICLO_CPU");
//	algoritmo_planificacion=config_get_string_value(config,"ALGORITMO_PLANIFICACION");
//	quantum=config_get_int_value(config,"QUANTUM");
//	estimacion_inicial=config_get_int_value(config,"ESTIMACION_INICIAL");
/*-------*/

	inicializar_listas();

 	inicializar_semaforos();

//	inicializar_hilos();

	bool inicio_exitoso = config && event_logger ; //&& listas_iniciadas_correctamente;
	if(!inicio_exitoso){
		error_show("algo malio sal en el inicio :(");
		exit(1);
	}

	FinDelProceso = false; //flag para hilos
}


int team_exit(){
	log_destroy(logger);
	log_destroy(event_logger);
	config_destroy(config);

	listas_destroy();

	return EXIT_SUCCESS;
}

/***********************************Funciones auxiliares *************************************/

//Listas
void inicializar_listas() {
	equipo = entrenadores_cargar();
	objetivosGlobales = entrenadores_objetivos_globales(equipo);
	pokemonesRequeridos = mapa_create();
	capturasPendientes = pendientes_create();
	historialDePokemones = list_create(); //Ver si vale la pena abstraer

	entrenadoresReady = cr_list_create();
}

void listas_destroy(){
	entrenadores_destroy(equipo);
	mapa_destroy(pokemonesRequeridos);
	pendientes_destroy(capturasPendientes);
//	list_destroy_and_destroy_elements(historialDePokemones, free); //TODO puede ser que rompa
}

//Colas
void subscribpcion_colas(){
	//TODO Gustavo// ?????????????????
}

//Hilos
void inicializar_hilos(){
	pthread_create(&hiloReceptorDeMensajes, NULL, (void*) broker_simulator, NULL); //Para pruebas sin broker

	pthread_create(&hiloMensajesAppeard, NULL, (void*)team_suscriptor_cola_APPEARD, mensajesAPPEARD);
	pthread_create(&hiloMensajesCAUGHT, NULL, (void*)team_suscriptor_cola_CAUGHT, mensajesCAUGHT);
	pthread_create(&hiloMensajesLOCALIZED, NULL, (void*)team_suscriptor_cola_LOCALIZED, mensajesLOCALIZED);

	inicializar_hilos_entrenadores();

	pthread_create(&hiloPlanificador, NULL, (void*) team_planificar, NULL);

}

void finalizar_hilos(){
	finalizar_hilos_entrenadores();

	FinDelProceso = true;

	pthread_join(hiloPlanificador, NULL);
	pthread_join(hiloMensajesAppeard, NULL);
	pthread_join(hiloMensajesCAUGHT, NULL);
	pthread_join(hiloMensajesLOCALIZED, NULL);
}

//Semaforos

void inicializar_semaforos(){
	sem_init(&EntradaSalida_o_FinDeEjecucion, 0, 1);
	sem_init(&HayTareasPendientes, 0, 0);
//	sem_init(&HayEntrenadores, 0, 0); VER??

	pthread_mutex_init(&Mutex_AndoLoggeando, NULL);
	pthread_mutex_init(&Mutex_AndoLoggeandoEventos, NULL);

}

void finalizar_semaforos(){
	//Los semaforos de los entrenadores se finalizan cuando se destruye el entrenador
	sem_destroy(&HayTareasPendientes);
	sem_destroy(&EntradaSalida_o_FinDeEjecucion);

	pthread_mutex_destroy(&Mutex_AndoLoggeando);
	pthread_mutex_destroy(&Mutex_AndoLoggeandoEventos);
}
