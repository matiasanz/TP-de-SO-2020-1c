/*
 * memoria.h
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_MEMORIA_MMU_H_
#define SRC_MEMORIA_MMU_H_

#include "memoria_utils.h"
#include "mensaje_cache.h"
#include "particiones_dinamicas/particiones_dinamicas.h"
#include "buddy_system/buddy_system.h"

t_log* dump_logger;

//Dump
void log_dump_cache(int signum);

void inicializar_memoria();
t_mensaje_cache* guardar_en_memoria(void* mensaje, t_id_cola id_cola);
//Dado un mensaje recibido, lo serializa para guardarlo en la cache
void* compactar_contenido_mensaje(void* msj_recibido, t_mensaje_cache_metadata* metadata, t_id_cola id_cola);
//Restaura un mensaje guardado en la cache
void* restaurar_mensaje_desde_cache(t_mensaje_cache* msj);

#endif /* SRC_MEMORIA_MMU_H_ */
