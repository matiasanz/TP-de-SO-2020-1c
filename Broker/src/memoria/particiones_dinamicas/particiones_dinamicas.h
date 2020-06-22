/*
 * particiones_dinamicas.h
 *
 *  Created on: 15 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_MEMORIA_PARTICIONES_DINAMICAS_PARTICIONES_DINAMICAS_H_
#define SRC_MEMORIA_PARTICIONES_DINAMICAS_PARTICIONES_DINAMICAS_H_

#include "../particion.h"
#include "../memoria_utils.h"
#include "../../colas.h"

t_particion* pd_obtener_particion_libre_first_fit(int tamanio_contenido, int* offset);
t_particion* pd_obtener_particion_libre_best_fit(int tamanio_contenido, int* offset);

void pd_liberar_victima_fifo();
void pd_liberar_victima_lru();

void compactar_memoria();
bool hay_particiones_ocupadas();
void reubicar_particiones_y_compactar_contenido(uint32_t tamanio_ocupado);

#endif /* SRC_MEMORIA_PARTICIONES_DINAMICAS_PARTICIONES_DINAMICAS_H_ */