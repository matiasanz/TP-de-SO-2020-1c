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

	inicializar_hilos();

	team_ejecutar_algoritmo_de_deteccion_de_deadlock();

	//team_mostrar_resultados();

	log_info(event_logger, "chau team\n****************************************");

	return team_exit();
}
																/*Fin del main*/
//**********************************************************************************

						/*Implementacion de funciones*/

void team_inicializar(){
	config=config_create("config/team.config");

	logger=log_create("log/team.log","TEAM",true,LOG_LEVEL_INFO);

	event_logger = log_create("log/team_event.log", "TEAM_EVENT", true, LOG_LEVEL_INFO);

	RETARDO_CICLO_CPU = config_get_int_value(config,"RETARDO_CICLO_CPU");

	//no hace falta que algoritmo y los datos esten agrupados en misma estructura
	ALGORITMO_PLANIFICACION = FIFO;//cargar_algoritmo_planificacion();

	inicializar_listas();

 	inicializar_semaforos();

//	inicializar_hilos();

	bool inicio_exitoso = config && event_logger ; //&& listas_iniciadas_correctamente;
	if(!inicio_exitoso){
		error_show("algo malio sal en el inicio :(");
		exit(1);
	}
}


int team_exit(){

	finalizar_hilos();
	finalizar_semaforos();
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
	inventariosGlobales = entrenadores_inventarios_globales(equipo);
	recursosEnMapa = recursos_create(); //TODO
	pokemonesRequeridos = mapa_create();
	capturasPendientes = pendientes_create();
	potencialesDeadlock = candidatos_create();
//Ver si vale la pena abstraer
	historialDePokemones = list_create();

	entrenadoresReady = cr_list_create();

	mensajesAPPEARD   = cr_list_create();
	mensajesCAUGHT    = cr_list_create();
	mensajesLOCALIZED = cr_list_create();

	//***************
}

void listas_destroy(){
	cr_list_destroy(mensajesAPPEARD);
	cr_list_destroy(mensajesCAUGHT);
	cr_list_destroy(mensajesLOCALIZED);

	entrenadores_destroy(equipo);
	cr_list_destroy(entrenadoresReady);
	mapa_destroy(pokemonesRequeridos);
	pendientes_destroy(capturasPendientes);
	recursos_destroy(objetivosGlobales);
	recursos_destroy(inventariosGlobales);
	recursos_destroy(recursosEnMapa);
	candidatos_destroy(potencialesDeadlock);
	list_destroy(historialDePokemones);
}

//Hilos
void inicializar_hilos(){
	pthread_create(&hiloReceptorDeMensajes, NULL, (void*) broker_simulator, NULL); //Para pruebas sin broker

	inicializar_hilos_entrenadores();

	pthread_create(&hiloMensajesAppeard, NULL, (void*)team_suscriptor_cola_APPEARD, mensajesAPPEARD);
	pthread_create(&hiloMensajesCAUGHT, NULL, (void*)team_suscriptor_cola_CAUGHT, mensajesCAUGHT);
	pthread_create(&hiloMensajesLOCALIZED, NULL, (void*)team_suscriptor_cola_LOCALIZED, mensajesLOCALIZED);

	pthread_create(&hiloPlanificador, NULL, (void*) team_planificar, NULL);

}

void finalizar_hilos(){
	finalizar_hilos_entrenadores();

	pthread_join(hiloReceptorDeMensajes, NULL);
	pthread_join(hiloPlanificador	   , NULL);
	pthread_join(hiloMensajesAppeard   , NULL);
	pthread_join(hiloMensajesCAUGHT	   , NULL);
	pthread_join(hiloMensajesLOCALIZED , NULL);
}

//Semaforos

void inicializar_semaforos(){
	sem_init(&HayTareasPendientes, 0, 0);
	sem_init(&HayEntrenadoresDisponibles, 0, 0);
	sem_init(&EquipoNoPuedaCazarMas, 0, 0);
	sem_init(&EntradaSalida_o_FinDeEjecucion, 0, 0);
	sem_init(&finDeIntercambio, 0, 0);

	pthread_mutex_init(&Mutex_AndoLoggeando       , NULL);
	pthread_mutex_init(&Mutex_AndoLoggeandoEventos, NULL);

	pthread_mutex_init(&mutexEntrenadores         , NULL);
	pthread_mutex_init(&mutexHistorialEspecies    , NULL);
	pthread_mutex_init(&mutexInventariosGlobales  , NULL);
	pthread_mutex_init(&mutexRecursosEnMapa       , NULL);
	pthread_mutex_init(&mutexRecursosEnMapa       , NULL);
}

void finalizar_semaforos(){
	sem_destroy(&HayTareasPendientes);
	sem_destroy(&HayEntrenadoresDisponibles);
	sem_destroy(&EntradaSalida_o_FinDeEjecucion);
	sem_destroy(&finDeIntercambio);

	pthread_mutex_destroy(&mutexEntrenadores);
	pthread_mutex_destroy(&mutexInventariosGlobales);
	pthread_mutex_destroy(&mutexHistorialEspecies);
	pthread_mutex_destroy(&Mutex_AndoLoggeando);
	pthread_mutex_destroy(&Mutex_AndoLoggeandoEventos);
}
