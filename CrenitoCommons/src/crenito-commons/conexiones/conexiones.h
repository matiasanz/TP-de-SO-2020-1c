/*
 * conexiones.h
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_
#define SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_

#include "paquete.h"
#include "socket.h"
#include "../utils.h"

int subscripcion_cola(t_id_proceso id_proceso, t_tipo_cola_mensaje nombre_cola, char* ip, char* puerto);
int subscripcion_exitosa(int estado);

#endif /* SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_ */
