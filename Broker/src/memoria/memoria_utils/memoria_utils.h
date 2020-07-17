/*
 * memoria_utils.h
 *
 *  Created on: 15 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_MEMORIA_MEMORIA_UTILS_MEMORIA_UTILS_H_
#define SRC_MEMORIA_MEMORIA_UTILS_MEMORIA_UTILS_H_

#include <sys/time.h>
#include "../../utils/broker_utils.h"

void* memoria_principal;
uint32_t tamanio_total_memoria;
uint32_t tamanio_sin_particionar;
uint32_t frecuencia_compactacion;
uint32_t cantidad_busquedas_fallidas;
uint32_t tamanio_minimo_particion;
pthread_mutex_t mutex_acceso_memoria;
pthread_mutex_t mutex_escritura_eliminacion_memoria;
//FIFO o LRU
void (*algoritmo_reemplazo)(void);
// Best Fit o First Fit
void* (*algoritmo_particion_libre)(int, int*);
// PARTICIONES o BUDDY SYSTEM
char* algoritmo_memoria;

void* (*obtener_particion_libre)(int);

//Particiones dinámicas
t_list* particiones;

//Buddy System
t_dictionary* bloques;

#define max(a,b) ((a) >= (b) ? (a) : (b))

bool hay_espacio_contiguo_diponible(uint32_t tamanio_contenido);
void memoria_actualizar_tamanio_disponible_sin_particionar(uint32_t tamanio_contenido);
void memoria_reset_tamanio_disponible_sin_particionar(uint32_t nuevo_tamanio);
uint32_t calcular_tamanio_particion(uint32_t tamanio_contenido);
bool esquema_de_memoria_particiones_dinamicas();

#endif /* SRC_MEMORIA_MEMORIA_UTILS_MEMORIA_UTILS_H_ */
