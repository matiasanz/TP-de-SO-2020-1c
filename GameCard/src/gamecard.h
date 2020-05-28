/*
 * gamecard.h
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_GAMECARD_H_
#define SRC_GAMECARD_H_

#include <crenito-commons/conexiones/conexiones.h>

t_conexion_server conexion_broker;

t_conexion_cliente conexion_catch_pokemon;
t_conexion_cliente conexion_get_pokemon;
t_conexion_cliente conexion_new_pokemon;

void inicializar();
void inicializar_config();
void inicializar_logs();
void inicializar_conexiones();
void subscribir_colas(void* arg);


#endif /* SRC_GAMECARD_H_ */
