/*
 * memoria.h
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_MEMORIA_MMU_H_
#define SRC_MEMORIA_MMU_H_

#include "memoria_modelo/mensaje_cache.h"
#include "memoria_utils/memoria_dump.h"
#include "memoria_utils/memoria_utils.h"

void inicializar_memoria();
t_mensaje_cache* guardar_en_memoria(void* msj_recibido, t_id_cola id_cola, t_mensaje_header msj_header);
//Dado un mensaje recibido, lo serializa para guardarlo en la cache
void* compactar_contenido_mensaje(void* msj_recibido, t_mensaje_cache_metadata* metadata, t_id_cola id_cola);
//Restaura un mensaje guardado en la cache
void* restaurar_mensaje_desde_cache(t_mensaje_cache* msj, t_mensaje_header* header);

#endif /* SRC_MEMORIA_MMU_H_ */
