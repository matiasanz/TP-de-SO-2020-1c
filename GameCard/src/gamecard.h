/*
 * gamecard.h
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_GAMECARD_H_
#define SRC_GAMECARD_H_

#include<commons/log.h>
#include<commons/config.h>
#include <crenito-commons/conexiones/conexiones.h>

t_config* config;
//Logger para uso exclusivo de informacion pedida por el tp
t_log* logger;
//Logger para grabar informacion adicional/errores
t_log* event_logger;

void inicializar();
void subscribpcion_colas();


#endif /* SRC_GAMECARD_H_ */
