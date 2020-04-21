/*
 * servidor.h
 *
 *  Created on: 3 mar. 2019
 *      Author: utnso
 */

#ifndef BROKER_H_
#define BROKER_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "utilidades-broker.h"
#include <crenito-commons/constants.h>


t_config* config;

void inicializar(void);
t_log* iniciar_logger(void);
t_config* leer_config(void);

#endif /* BROKER_H_ */
