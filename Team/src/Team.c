/*
 ============================================================================
 Name        : Team.c
 Author      : C-renito Casero
 Version     : Checkpoint
 Description : Proceso Team
 ============================================================================
 */

#include "team.h"
#include "hilos-del-team/hilos_team.h"
//#include "tests/tests_team.o"


char* team_get_nombre(int cantidad, char**argumentos){
	return cantidad>1? argumentos[1]: "";
}

int main(int cantidad, char**argumentos ) {

	team_inicializar(cantidad, argumentos);

	Get_pokemones(objetivosGlobales, inventariosGlobales);

	//Para pruebas rapidas sin los otros modulos//
//	pthread_create(&hiloReceptorDeMensajes, NULL, (void*) gamecard_simulator, NULL);

	inicializar_hilos();

	esperar_que_equipo_no_pueda_cazar_mas();

	finalizar_suscripcion_a_colas();

	team_ejecutar_algoritmo_de_deteccion_de_deadlock();

	pthread_join(hiloPlanificador, NULL);

	team_loggear_resultados();

	return team_exit();
}
																/*Fin del main*/
//**********************************************************************************

						/*Implementacion de funciones*/

void team_inicializar(int cantidad, char**argumentos){

	char*NombreEquipo = team_get_nombre(cantidad, argumentos);

	inicializar_config(NombreEquipo);

	inicializar_logs();

	inicializar_listas();

	cantidadDeEntrenadores = list_size(equipo);

	inicializar_estadisticas(cantidadDeEntrenadores);

	cargar_algoritmo_planificacion();

 	inicializar_semaforos();

	log_info(logger, "\n\n*************************************************************************\n"
         		     "                        Inicio del proceso Team %s\n\n", NombreEquipo);
	inicializar_conexiones();

	sleep(1); //

	inicializar_hilos_entrenadores();
}

int team_exit(){

	log_info(logger, "\n\n                              Fin del proceso Team\n"
						      "****************************************************************************");
	finalizar_logs_y_config();
	finalizar_estadisticas();
	listas_destroy();
	finalizar_hilos();
	finalizar_semaforos();

	return EXIT_SUCCESS;
}

/***********************************Funciones auxiliares *************************************/

void validar_lectura(void*lectura, char* MENSAJE_EXCEPCION){
	if(!lectura){
		error_show(MENSAJE_EXCEPCION);
		exit(1);
	}
}

//Logs y config
void inicializar_logs(){
	logger = log_crear("TEAM", "LOG_FILE");
	validar_lectura(logger, "No se encontro el archivo logger");

	event_logger = log_crear("TEAM_EVENT", "LOG_EVENT_FILE");
	validar_lectura(event_logger, "No se encontro el archivo event_logger");
}

void inicializar_config(char* NombreEquipo){
	char* CONFIG_PATH = string_from_format("%steam%s.config", CARPETA_CONFIG, NombreEquipo);

		config=config_create(CONFIG_PATH);

		if(!config){

			error_show("No se encontro el archivo <<%s>>", CONFIG_PATH);
		    free(CONFIG_PATH);
			exit(1);
		}

    free(CONFIG_PATH);
}

void finalizar_logs_y_config(){
	config_destroy(config);
	log_destroy(logger);
	log_destroy(event_logger);
}

//Estadisticas
void inicializar_estadisticas(numero cantidadDeProcesos){

	numero*ciclosPorEntrenador = malloc(sizeof(numero)*cantidadDeProcesos);

	int i=0;
	for(i=0; i<cantidadDeProcesos; i++){
		ciclosPorEntrenador[i]=0;
	}

	Estadisticas = (estadisticas_team) {0,0,ciclosPorEntrenador, 0, 0};
}

void finalizar_estadisticas(){
	free(Estadisticas.ciclosDelEntrenador);
}

char*estadisticas_to_string(estadisticas_team resultados){
	char* string = string_new();

	char*ciclosDeCadaEntrenador = num_array_to_string(Estadisticas.ciclosDelEntrenador, cantidadDeEntrenadores);

	string_append_with_format(&string, ">> Total de Cambios de Contexto ..... %u\n", resultados.cambiosDeContexto);
	string_append_with_format(&string, ">> Ciclos de CPU por entrenador:\n%s", ciclosDeCadaEntrenador);
	string_append_with_format(&string, ">> Total de Ciclos de CPU ........... %u\n", resultados.ciclosCPU);
	string_append_with_format(&string, ">> Deadlocks producidos ............. %u\n", resultados.deadlocksProducidos);
	string_append_with_format(&string, ">> Deadlocks resueltos .............. %u\n", resultados.deadlocksResueltos);

	free(ciclosDeCadaEntrenador);

	return string;
}

char*num_array_to_string(numero*arreglo, int length){
	char* stringFromArray = string_new();

	numero i;
	for(i=0; i<length; i++){
		string_append_with_format(&stringFromArray,"  . Entrenador N°%u .................. %u\n", i, arreglo[i]);
	}

	return stringFromArray;
}

//Listas
void inicializar_listas() {
	equipo = entrenadores_cargar();
	objetivosGlobales   = entrenadores_objetivos_globales(equipo);
	inventariosGlobales = entrenadores_inventarios_globales(equipo);
	recursosEnMapa      = recursos_create();
	pokemonesRecibidos  = mapa_create();
	capturasPendientes  = pendientes_create();
	potencialesDeadlock = candidatos_create();

	historialDePokemones = list_create();
	mapaRequeridos       = list_create();
	entrenadoresReady    = cr_list_create();
	registroDePedidos    = list_create();

//	mensajesCAUGHT    = cr_list_create();
//	mensajesLOCALIZED = cr_list_create();

	//***************
}

void listas_destroy(){
	cr_list_destroy(entrenadoresReady);

	recursos_destroy(objetivosGlobales);
	recursos_destroy(inventariosGlobales);
	recursos_destroy(recursosEnMapa);
	candidatos_destroy(potencialesDeadlock);
	entrenadores_destroy(equipo);

	boolean_sem_wait_end(&BOOLSEM_EsperoMensajes);

	pendientes_destroy(capturasPendientes);

	list_destroy_and_destroy_elements(registroDePedidos, free);
	list_destroy_and_destroy_elements(historialDePokemones, free);
	list_destroy_and_destroy_elements(mapaRequeridos, (void*) pokemon_destroy_hard);

	cr_list_destroy_and_destroy_elements(pokemonesRecibidos, (void*) pokemon_destroy_hard);
//	cr_list_destroy_and_destroy_elements(mensajesCAUGHT    , (void*) mensaje_caught_pokemon_destruir);
//	cr_list_destroy_and_destroy_elements(mensajesLOCALIZED , (void*) mensaje_localized_pokemon_destruir);
}

//Hilos
void inicializar_hilos(){

	pthread_create(&hiloPlanificador, NULL, (void*) team_planificar, NULL);
	pthread_create(&hiloProcesadorDePokemones, NULL, (void*) team_procesar_pokemones, NULL);
	pthread_detach(hiloProcesadorDePokemones);
}

void finalizar_hilos(){
	finalizar_hilos_entrenadores();

//	pthread_join(hiloMensajesAppeard   , NULL);
//	pthread_join(hiloMensajesCAUGHT	   , NULL);
//	pthread_join(hiloMensajesLOCALIZED , NULL);
}

//Semaforos

void inicializar_semaforos(){
	sem_init(&HayEntrenadoresDisponibles, 0, 0);
	sem_init(&EquipoNoPuedaCazarMas     , 0, 0);
	sem_init(&FinDeCiclo_CPU            , 0, 0);
	sem_init(&finDeIntercambio          , 0, 0);

	boolean_sem_init(&BOOLSEM_EsperoMensajes);

	pthread_mutex_init(&Mutex_AndoLoggeando       , NULL);
	pthread_mutex_init(&Mutex_AndoLoggeandoEventos, NULL);

	pthread_mutex_init(&mutexEntrenadores         , NULL);
	pthread_mutex_init(&mutexHistorialEspecies    , NULL);
	pthread_mutex_init(&mutexRepuestos            , NULL);

	pthread_mutex_init(&mutexRecursosDisponibles  , NULL);
	pthread_mutex_init(&mutexPedidos              , NULL);
}

void finalizar_semaforos(){
	sem_destroy(&HayEntrenadoresDisponibles);
	sem_destroy(&FinDeCiclo_CPU);
	sem_destroy(&finDeIntercambio);

	pthread_mutex_destroy(&mutexEntrenadores);
	pthread_mutex_destroy(&mutexHistorialEspecies);
	pthread_mutex_destroy(&mutexRepuestos);
	pthread_mutex_destroy(&mutexRecursosDisponibles);
	pthread_mutex_destroy(&mutexPedidos);
	pthread_mutex_destroy(&Mutex_AndoLoggeando);
	pthread_mutex_destroy(&Mutex_AndoLoggeandoEventos);
}

//Conexiones
void inicializar_conexiones() {

	TIEMPO_RECONEXION = config_get_int_value(config, "TIEMPO_RECONEXION");

	id_proceso = config_get_int_value(config, "ID_PROCESO");

	conexion_broker = conexion_server_crear(config_get_string_value(config, "IP_BROKER")
										  , config_get_string_value(config, "PUERTO_BROKER")
										  , TEAM);

	pthread_mutex_init(&mutex_subscripcion, NULL);

	suscribir_y_escuchar_cola_appeared_pokemon((void*) mensaje_recibido);
	suscribir_y_escuchar_cola_caught_pokemon((void*) mensaje_recibido);
	suscribir_y_escuchar_cola_localized_pokemon((void*) mensaje_recibido);

	conectar_gameboy((void*) mensaje_recibido);
}

void finalizar_conexiones(){
	finalizar_suscripcion_a_colas();
//	conexion_server_destruir(conexion_broker);
//	conexion_host_destruir(conexion_gameboy);
}

void finalizar_suscripcion_a_colas(){
	//******************************************************************** INICIO HARDCODEADO
	boolean_sem_turn_off(&BOOLSEM_PSEUDOGAMECARD);
	//******************************************************************** fin HARDCODEADO
	dejar_de_recibir();

//	pthread_cancel(hilo_appeared_pokemon);
//	pthread_cancel(hilo_localized_pokemon);
//	pthread_cancel(hilo_caught_pokemon);
}
