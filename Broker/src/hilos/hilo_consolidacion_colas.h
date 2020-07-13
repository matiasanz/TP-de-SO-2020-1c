/*
 * hilo_consolidar_colas.h
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_HILOS_HILO_CONSOLIDACION_COLAS_H_
#define SRC_HILOS_HILO_CONSOLIDACION_COLAS_H_

#include "../broker_utils.h"
#include "../colas.h"
#include "../memoria/mmu.h"
#include "hilo_envio_mensajes.h"

pthread_t hilo_consolidacion_colas;
int frecuencia_consolidacion_colas;

void inicializar_hilo_consolidacion_colas();

#endif /* SRC_HILOS_HILO_CONSOLIDACION_COLAS_H_ */
