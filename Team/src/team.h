/*
 * team.h
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#ifndef SRC_TEAM_H_
#define SRC_TEAM_H_

#include <stdio.h>
#include <stdlib.h>

#include <commons/config.h>
#include <commons/log.h>

#include <crenito-commons/conexiones/conexiones.h>
#include <crenito-commons/utils.h>


/*Constantes*/
#define CONFIG_PATH "config/team.config"


/*Variables Globales*/

t_config* config;

t_log* logger;
/*--------------*/


int inicializar();
int subscribpcion_colas();

void exit_team();



#endif /* SRC_TEAM_H_ */
