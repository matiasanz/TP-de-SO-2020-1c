/*
 ============================================================================
 Name        : Team.c
 Author      : 
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "team.h"
#include "hilos_del_team/hilos_team.h"
//#include "tests/tests_team.o" //Desactualizados

int main(void) {

	team_inicializar();

	Get_pokemones(objetivosGlobales);

	log_info(event_logger, "\n\n****************************************\n¡¡¡Jejejellou World Team!!!\n"); /* prints !!!Hello World!!! */
	log_info(logger, "\n\n");

	//Para pruebas sin broker
	pthread_create(&hiloReceptorDeMensajes, NULL, (void*) broker_simulator, NULL);

	inicializar_hilos();

	//Por ahora no hace nada en el medio

	finalizar_hilos(); //En realidad llega hasta aca por los hilos que no joinean nunca

	finalizar_semaforos();

	log_info(event_logger, "chau team\n****************************************");

	return team_exit();
}

																/*Fin del main*/
//**********************************************************************************

						/*Implementacion de funciones*/

void team_inicializar(){
	config=config_create(CONFIG_PATH);

	logger=log_create("log/team.log","TEAM",true,LOG_LEVEL_INFO);

	event_logger = log_create("log/team_event.log", "TEAM_EVENT", true, LOG_LEVEL_INFO);

	inicializar_listas();
 	inicializar_semaforos();
	inicializar_conexiones();

	bool inicio_exitoso = config && event_logger ;
	if(!inicio_exitoso){
		error_show("algo malio sal en el inicio :(");
		exit(1);
	}
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
	inventariosGlobales = entrenadores_inventarios_globales(equipo);
	recursosEnMapa = recursos_create();
	pokemonesRequeridos = mapa_create();
	capturasPendientes = pendientes_create();
	historialDePokemones = list_create();
	entrenadoresReady = cr_list_create();
	mensajesAPPEARED = cr_list_create();
	mensajesCAUGHT = cr_list_create();
	mensajesLOCALIZED = cr_list_create();
}

void listas_destroy(){
	entrenadores_destroy(equipo);
	cr_list_destroy(entrenadoresReady);
	mapa_destroy(pokemonesRequeridos);
	pendientes_destroy(capturasPendientes);
	recursos_destroy(objetivosGlobales);
	recursos_destroy(inventariosGlobales);
	recursos_destroy(recursosEnMapa);
	list_destroy(historialDePokemones);
	cr_list_destroy(mensajesAPPEARED);
	cr_list_destroy(mensajesCAUGHT);
	cr_list_destroy(mensajesLOCALIZED);
}

//Hilos
void inicializar_hilos(){
	inicializar_hilos_entrenadores();

	pthread_create(&hiloMensajesAppeard, NULL, (void*)team_suscriptor_cola_APPEARD, mensajesAPPEARED);
	pthread_create(&hiloMensajesCAUGHT, NULL, (void*)team_suscriptor_cola_CAUGHT, mensajesCAUGHT);
	pthread_create(&hiloMensajesLOCALIZED, NULL, (void*)team_suscriptor_cola_LOCALIZED, mensajesLOCALIZED);

	pthread_create(&hiloPlanificador, NULL, (void*) team_planificar, NULL);

}

void finalizar_hilos(){
	finalizar_hilos_entrenadores();

	pthread_mutex_lock(&MUTEX_FIN_DE_PROCESO_BORRARRRRRRRRRRRRRRRR);
	finDeProceso = true;
	pthread_mutex_unlock(&MUTEX_FIN_DE_PROCESO_BORRARRRRRRRRRRRRRRRR);

	pthread_join(hiloReceptorDeMensajes, NULL);
	pthread_join(hiloPlanificador	   , NULL);
	pthread_join(hiloMensajesAppeard   , NULL);
	pthread_join(hiloMensajesCAUGHT	   , NULL);
	pthread_join(hiloMensajesLOCALIZED , NULL);
}

//Semaforos

void inicializar_semaforos(){
	sem_init(&HayTareasPendientes, 0, 0);
	sem_init(&EntradaSalida_o_FinDeEjecucion, 0, 0);

	pthread_mutex_init(&mutexEntrenadores         , NULL);
	pthread_mutex_init(&mutexHistorialEspecies    , NULL);
	pthread_mutex_init(&mutexInventariosGlobales  , NULL);
	pthread_mutex_init(&mutexRecursosEnMapa       , NULL);
}

void finalizar_semaforos(){
	sem_destroy(&HayTareasPendientes);
	sem_destroy(&EntradaSalida_o_FinDeEjecucion);

	pthread_mutex_destroy(&mutexEntrenadores);
	pthread_mutex_destroy(&mutexInventariosGlobales);
	pthread_mutex_destroy(&mutexHistorialEspecies);
	pthread_mutex_destroy(&mutexRecursosEnMapa);

}

//Conexiones
void inicializar_conexiones() {

	conexion_broker = conexion_server_crear(
			config_get_string_value(config, "IP_BROKER"),
			config_get_string_value(config, "PUERTO_BROKER"), TEAM);

	pthread_mutex_init(&mutex_subscripcion, NULL);

	subscribir_y_escuchar_cola_appeared_pokemon((void*) mensaje_recibido);
	subscribir_y_escuchar_cola_caught_pokemon((void*) mensaje_recibido);
	subscribir_y_escuchar_cola_localized_pokemon((void*) mensaje_recibido);

	conectar_gameboy((void*) mensaje_recibido);
}
