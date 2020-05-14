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

int main(void) { //Programa principal para pruebas puntuales

	team_inicializar();
	log_info(event_logger, "****************************************\n!!!Hello World Team!!!"); /* prints !!!Hello World!!! */

	t_list* mensajes = list_create();

		mensaje* unMensaje = malloc(sizeof(mensaje));
		*unMensaje = (mensaje) {APPEARD_POKEMON_, pokemon_ptr_create("Charmander", 2, 9)};
		list_add(mensajes, unMensaje);
		unMensaje = malloc(sizeof(mensaje));
		*unMensaje = (mensaje) {APPEARD_POKEMON_, pokemon_ptr_create("Squirtle", 7, 4)};
		list_add(mensajes, unMensaje);
		unMensaje = malloc(sizeof(mensaje));
		*unMensaje = (mensaje) {APPEARD_POKEMON_, pokemon_ptr_create("Pikachu", 7, 4)};
		list_add(mensajes, unMensaje);//
		unMensaje = malloc(sizeof(mensaje));


//	mapa_mapear_objetivo(pokemonesRequeridos, pokemon_ptr_create("Squartle", 9, 2));
//	team_planificar();
//	team_hilo_entrenador();

//	mapa_mapear_objetivo(pokemonesRequeridos, pokemon_ptr_create("Pichu", 1, 2));
//	mapa_mapear_objetivo(pokemonesRequeridos, pokemon_ptr_create("Pikachu", 6, 7));
//	mapa_mapear_objetivo(pokemonesRequeridos, pokemon_ptr_create("Bulbasaur", 1, 0));
//	mapa_mapear_objetivo(pokemonesRequeridos, pokemon_ptr_create("Squartle", 9, 2));


	puts("se inicio haymaspkm");
	sem_init(&sem_HayMasPokemonesEnMapa, 0, 0);
	puts("se inicio pokemonRemovido");
	sem_init(&sem_PokemonRemovido, 0, 1);

//	team_procesar_mensajes(mensajes);
//	sem_post(&sem_HayMasPokemonesEnMapa);
//	sem_post(&sem_HayMasPokemonesEnMapa);
//	sem_post(&sem_HayMasPokemonesEnMapa);
//	sem_post(&sem_HayMasPokemonesEnMapa);
//	team_planificar();

	pthread_t hiloProcesadorDeMensajes;
	pthread_create(&hiloProcesadorDeMensajes, NULL, (void*)team_procesar_mensajes, mensajes);

	int cantidadDeHilos=0;
	pthread_t* hilosEntrenadores = inicializar_hilos_entrenadores(&cantidadDeHilos);

	pthread_t hiloPlanificador;
	pthread_create(&hiloPlanificador, NULL, (void*) team_planificar, NULL);


//	sleep(1);

	t_id* unid = malloc(sizeof(t_id));
	*unid = 0;
	*unMensaje = (mensaje) {CAUGHT_POKEMON_, unid};
	list_add(mensajes, unMensaje);//
	*unid = 1;
		*unMensaje = (mensaje) {CAUGHT_POKEMON_, unid};
		list_add(mensajes, unMensaje);//
		*unid = 2;
			*unMensaje = (mensaje) {CAUGHT_POKEMON_, unid};
			list_add(mensajes, unMensaje);//


	int i=0;
	for(i=0; i<cantidadDeHilos; i++){
		pthread_join(hilosEntrenadores[i], NULL);
	}

	pthread_join(hiloPlanificador, NULL);
//
	pthread_join(hiloProcesadorDeMensajes, NULL);

	sem_destroy(&sem_HayMasPokemonesEnMapa);

	log_info(event_logger, "chau team\n****************************************");

//	list_destroy(mensajes);

	return team_exit(); //Destruye listas, cierra config, cierra log
}

//**********************************************************************************
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
