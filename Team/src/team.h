/*
 * team.h
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#ifndef SRC_TEAM_H_
#define SRC_TEAM_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "mensajes-utils.h"

/*Constantes*/
#define CONFIG_PATH "config/team.config"


/*Variables Globales*/
sem_t objetivos_team;

/*--------------*/


int inicializar();
void inicializar_conexiones();

void exit_team();



#endif /* SRC_TEAM_H_ */
