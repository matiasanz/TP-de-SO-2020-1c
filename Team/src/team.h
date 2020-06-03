/*
 * team.h
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#ifndef SRC_TEAM_H_
#define SRC_TEAM_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "utils/mensajes-utils.h"

#include "estructuras_principales/pokemon.h"
#include "lector_config/lector_config.h"
#include "estructuras_secundarias/captura_pendiente.h"
#include "estructuras_secundarias/mensajes.h"
#include "header_global_team.h"
#include <pthread.h>

//funciones del team
void team_inicializar();
int team_exit();

//funciones auxiliares
void inicializar_listas();
void listas_destroy();

void subscribpcion_colas(); //(?)

void inicializar_semaforos();
void finalizar_semaforos();
void inicializar_hilos();
void finalizar_hilos();

/*Constantes*/
#define CONFIG_PATH "config/team.config"

/*Mis Listas (son variables globales, para que puedan acceder todos los hilos)*/
	entrenadores equipo;
	cola_entrenadores entrenadoresReady;
	mapa_pokemones pokemonesRequeridos;
	matriz_recursos objetivosGlobales;
	matriz_recursos inventariosGlobales; //cambiar nombre por recursosDisponiblesGlobales
	matriz_recursos recursosEnMapa; //cambiar nombre por recursosPorAdquirir o algo mas representativo
	pendientes capturasPendientes; //cambiar tipo de dato por capturas_pendientes
	especies_pokemones historialDePokemones;

	cr_list*mensajesAPPEARED;
	cr_list*mensajesCAUGHT;
	cr_list*mensajesLOCALIZED;


int inicializar();
void inicializar_conexiones();

void exit_team();



#endif /* SRC_TEAM_H_ */
