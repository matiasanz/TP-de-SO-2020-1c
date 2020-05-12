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
//#include "tests/tests_team.o" //comentar si se va a usar string.h

int main(void) { //Programa principal para pruebas puntuales
 	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	team_inicializar();

//	team_procesar_mensajes();
//	team_planificar();
//	mapa_mapear_objetivo(pokemonesRequeridos, pokemon_ptr_create("Squartle", 9, 2));
//	team_planificar();
//	team_hilo_entrenador();

	log_info(event_logger, "hola team");

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
