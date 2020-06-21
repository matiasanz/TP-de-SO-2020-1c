/*
 * procesar_subscripcion.h
 *
 *  Created on: 14 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_MODOS_PROCESAMIENTO_PROCESAR_SUSCRIPCION_H_
#define SRC_MODOS_PROCESAMIENTO_PROCESAR_SUSCRIPCION_H_

#include "../colas.h"
#include "../memoria/mmu.h"
// Punto de entrada para cuando se detecta que un proceso se quiere subscribir a una cola
void procesar_suscripcion(int socket, t_paquete_header header);
void enviar_mensaje_a_suscriptor(t_mensaje_cache* msj, t_suscriptor* suscriptor);
#endif /* SRC_MODOS_PROCESAMIENTO_PROCESAR_SUSCRIPCION_H_ */
