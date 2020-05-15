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

t_list* mensajes;

int main(void) {

	team_inicializar();
	log_info(event_logger, "\n\n****************************************\n!!!Jellou World Team!!!\n"); /* prints !!!Hello World!!! */
	log_info(logger, "\n\n");

		mensajes = list_create();

	sem_init(&sem_HayMasPokemonesEnMapa, 0, 0);
	puts("Semaforo se inicio <<hay mas pkm?>>");
	sem_init(&sem_PokemonRemovido, 0, 1);
	puts("Semaforo se inicio <<pokemon removido?>>");
	sem_init(&sem_HayMensajesRecibidos, 0, 0);
	puts("Semaforo se inicio <<hay memnsajes recibidos?>>");

	pthread_t hiloReceptorDeMensajes;
	pthread_create(&hiloReceptorDeMensajes, NULL, (void*) team_recibir_mensajes, mensajes);

	pthread_t hiloProcesadorDeMensajes;
	pthread_create(&hiloProcesadorDeMensajes, NULL, (void*)team_procesar_mensajes, mensajes);

	int cantidadDeHilos=0;
	pthread_t* hilosEntrenadores = inicializar_hilos_entrenadores(&cantidadDeHilos);

	pthread_t hiloPlanificador;
	pthread_create(&hiloPlanificador, NULL, (void*) team_planificar, NULL);

	int i=0;
	for(i=0; i<cantidadDeHilos; i++){
		pthread_join(hilosEntrenadores[i], NULL);
	}

	FinDelProceso = true;

	//Matar planificador

	pthread_join(hiloPlanificador, NULL);

	pthread_join(hiloProcesadorDeMensajes, NULL);

	pthread_join(hiloReceptorDeMensajes, NULL);


	sem_destroy(&sem_PokemonRemovido);
	sem_destroy(&sem_HayMasPokemonesEnMapa);
	sem_destroy(&sem_HayMensajesRecibidos);

	log_info(event_logger, "chau team\n****************************************");

//	list_destroy(mensajes);

	return team_exit(); //Destruye listas, cierra config, cierra log
}

																/*Fin del main*/
//**********************************************************************************

						/*Implementacion de funciones*/

void team_inicializar(){

	config=config_create("config/team.config");

	logger=log_create(config_get_string_value(config, "LOG_FILE"),"TEAM",true,LOG_LEVEL_INFO);

	event_logger = log_create("log/team_event.log", "TEAM_EVENT", true, LOG_LEVEL_INFO);

//	subscribpcion_colas();

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
	mensajesPendientes = pendientes_create();
	historialDePokemones = list_create(); //Ver si vale la pena abstraer
}

void listas_destroy(){
	entrenadores_destroy(equipo);
	mapa_destroy(pokemonesRequeridos);
	pendientes_destroy(mensajesPendientes);
	list_destroy(historialDePokemones);
}

void subscribpcion_colas(){
	//TO-DO//
}
