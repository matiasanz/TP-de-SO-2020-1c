/*
 * team.h
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#ifndef SRC_TEAM_H_
#define SRC_TEAM_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "hilo_appeared_pokemon.h"
#include "hilo_caught_pokemon.h"
#include "hilo_localized_pokemon.h"

/*Constantes*/
#define CONFIG_PATH "config/team.config"


/*Variables Globales*/
sem_t objetivos_team;

/*--------------*/


int inicializar();
void inicializar_conexiones();
void subscribir_y_escuchar_cola_localized_pokemon();

void exit_team();



#endif /* SRC_TEAM_H_ */
