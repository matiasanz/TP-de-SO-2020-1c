/*
 * team.h
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#ifndef SRC_TEAM_H_
#define SRC_TEAM_H_

#include<pthread.h>
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

t_conexion_server conexion_broker;

t_conexion_cliente conexion_appeared_pokemon;
t_conexion_cliente conexion_localized_pokemon;
t_conexion_cliente conexion_caught_pokemon;

/*--------------*/


int inicializar();
void inicializar_conexiones();
void subscribir_colas(void* arg);
void exit_team();



#endif /* SRC_TEAM_H_ */
