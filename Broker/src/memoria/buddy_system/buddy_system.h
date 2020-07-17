/*
 * buddy_system.h
 *
 *  Created on: 10 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_MEMORIA_BUDDY_SYSTEM_BUDDY_SYSTEM_H_
#define SRC_MEMORIA_BUDDY_SYSTEM_BUDDY_SYSTEM_H_

#include "../memoria_modelo/bloque.h"
#include "../../colas.h"
#include "../../utils/broker_log.h"
#include "../particiones_dinamicas/particiones_dinamicas.h"

void inicializar_buddy_system();
t_list* bs_obtner_particiones_dump();

t_particion* bs_obtener_particion_libre_best_fit(int tamanio_contenido, int* offset);
t_particion* bs_obtener_particion_libre_first_fit(int tamanio_contenido, int* offset);
t_particion* bs_obtener_particion_libre(int tamanio_contenido);

void bs_liberar_victima_fifo();
void bs_liberar_victima_lru();

#endif /* SRC_MEMORIA_BUDDY_SYSTEM_BUDDY_SYSTEM_H_ */
