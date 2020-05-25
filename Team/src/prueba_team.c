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

//cr_list*mensajesAPPEARD; //globales
//cr_list*mensajesCAUGHT;


int main(void) {
	team_inicializar();
	log_info(event_logger, "\n\n****************************************\n!!!Jellou World Team!!!\n"); /* prints !!!Hello World!!! */
	log_info(logger, "\n\n");

		mensajesAPPEARD = cr_list_create();
		mensajesCAUGHT = cr_list_create();

 	inicializar_semaforos();

	pthread_create(&hiloReceptorDeMensajes, NULL, (void*) broker_simulator, NULL);
	pthread_create(&hiloMensajesAppeard, NULL, (void*)team_suscriptor_cola_APPEARD, mensajesAPPEARD);
	pthread_create(&hiloMensajesCAUGHT, NULL, (void*)team_suscriptor_cola_CAUGHT, mensajesCAUGHT);
//	pthread_create(&hiloMensajesLOCALIZED, NULL, (void*)team_suscriptor_cola_APPEARD(), colaDeMensajesAPPEARD);

	int cantidadDeEntrenadores=0;
	pthread_t* hilosEntrenadores = inicializar_hilos_entrenadores(&cantidadDeEntrenadores);

	pthread_create(&hiloPlanificador, NULL, (void*) team_planificar, NULL);

	int i=0;
	for(i=0; i<cantidadDeEntrenadores; i++){
		pthread_join(hilosEntrenadores[i], NULL);
	}


//////Matar planificador

	pthread_join(hiloMensajesAppeard, NULL);
	pthread_join(hiloMensajesCAUGHT, NULL);

	finalizar_hilos();

	finalizar_semaforos();

	log_info(event_logger, "chau team\n****************************************");

	cr_list_destroy(tareasPendientes);
	cr_list_destroy(mensajesAPPEARD);
	cr_list_destroy(mensajesCAUGHT);

	return team_exit(); //Destruye listas, cierra config, cierra log
}

																/*Fin del main*/
//**********************************************************************************

						/*Implementacion de funciones*/

void team_inicializar(){
	config=config_create("config/team.config");

	char*algo = config_get_string_value(config, "LOG_FILE");

	printf("%s", algo);

	logger=log_create("log/team.log","TEAM",true,LOG_LEVEL_INFO);

	event_logger = log_create("log/team_event.log", "TEAM_EVENT", true, LOG_LEVEL_INFO);


//	subscribpcion_colas();

/*Gustavo*/
//	tiempo_reconexion=config_get_int_value(config,"TIEMPO_RECONEXION");
//	retardo_ciclo_cpu=config_get_int_value(config,"RETARDO_CICLO_CPU");
//	algoritmo_planificacion=config_get_string_value(config,"ALGORITMO_PLANIFICACION");
//	quantum=config_get_int_value(config,"QUANTUM");
//	estimacion_inicial=config_get_int_value(config,"ESTIMACION_INICIAL");
/*-------*/

	inicializar_listas();

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

//**************************************************************
//Funciones auxiliares:

void inicializar_listas() {
	equipo = entrenadores_create();
	entrenadores_cargar(equipo);
	pokemonesRequeridos = mapa_create();
	capturasPendientes = pendientes_create();
	historialDePokemones = list_create(); //Ver si vale la pena abstraer
}

void listas_destroy(){
	entrenadores_destroy(equipo);
	mapa_destroy(pokemonesRequeridos);
	pendientes_destroy(capturasPendientes);
	list_destroy(historialDePokemones);
}

void subscribpcion_colas(){
	//TODO Gustavo//
}

void inicializar_semaforos(){
	sem_init(&sem_HayMensajesRecibidos, 0, 0);//

	sem_init(&EntradaSalida_o_FinDeEjecucion, 0, 1);
	sem_init(&HayTareasPendientes, 0, 0);

	pthread_mutex_init(&Mutex_AndoLoggeando, NULL);
	pthread_mutex_init(&Mutex_AndoLoggeandoEventos, NULL);
//	pthread_mutex_init(&mutexFinDeProceso, NULL);
}

void finalizar_semaforos(){
	sem_destroy(&sem_HayMensajesRecibidos);
}

void finalizar_hilos(){
	FinDelProceso = true;

	pthread_join(hiloPlanificador, NULL);
	pthread_join(hiloProcesadorDeMensajes, NULL);
	pthread_join(hiloReceptorDeMensajes, NULL);
}
