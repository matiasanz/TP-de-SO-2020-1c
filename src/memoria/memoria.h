/*
 * memoria.h
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_MEMORIA_MEMORIA_H_
#define SRC_MEMORIA_MEMORIA_H_

#include "../particiones_dinamicas/particiones_dinamicas.h"
#include "mensaje_cache.h"

void inicializar_memoria();

t_particion* guardar_en_cache(void* mensaje, t_id_cola id_cola);

//Dado un mensaje recibido, lo serializa para guardarlo en la cache
void* mensaje_cache_serializar(void* msj_recibido, t_particion* particion);

//Restaura un mensaje guardado en la cache
void* mensaje_cache_restaurar_contenido(t_particion* msj);
void* memoria_calcular_direccion(t_particion* particion);


#endif /* SRC_MEMORIA_MEMORIA_H_ */
