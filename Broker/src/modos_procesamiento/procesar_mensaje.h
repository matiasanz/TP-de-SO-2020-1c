/*
 * procesar_mensaje.h
 *
 *  Created on: 14 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_MODOS_PROCESAMIENTO_PROCESAR_MENSAJE_H_
#define SRC_MODOS_PROCESAMIENTO_PROCESAR_MENSAJE_H_

#include "../colas.h"
#include "../memoria/mmu.h"
#include "../hilos/hilo_envio_mensajes.h"
// Punto de entrada para cuando se detecta que un proceso envia un mensaje
void procesar_mensaje(int socket, t_paquete_header header);

#endif /* SRC_MODOS_PROCESAMIENTO_PROCESAR_MENSAJE_H_ */
