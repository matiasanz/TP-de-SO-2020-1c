/*
 * procesar_subscripcion.h
 *
 *  Created on: 14 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_MODOS_PROCESAMIENTO_PROCESAR_SUSCRIPCION_H_
#define SRC_MODOS_PROCESAMIENTO_PROCESAR_SUSCRIPCION_H_

#include "../hilos/hilo_envio_mensajes.h"

// Punto de entrada para cuando se detecta que un proceso se quiere subscribir a una cola
void procesar_suscripcion(int socket, t_paquete_header header);

#endif /* SRC_MODOS_PROCESAMIENTO_PROCESAR_SUSCRIPCION_H_ */
