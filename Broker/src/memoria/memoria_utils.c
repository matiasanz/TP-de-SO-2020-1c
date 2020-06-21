/*
 * memoria_utils.c
 *
 *  Created on: 15 jun. 2020
 *      Author: utnso
 */

#include "memoria_utils.h"

bool hay_espacio_contiguo_diponible(uint32_t tamanio_contenido) {
	return tamanio_sin_particionar >= tamanio_contenido;
}

void memoria_actualizar_tamanio_disponible(uint32_t tamanio_contenido) {
	tamanio_sin_particionar -= tamanio_contenido;
}

uint32_t calcular_tamanio_minimo_particion(uint32_t tamanio_contenido) {
	return max(tamanio_minimo_particion, tamanio_contenido);
}

bool esquema_de_memoria_particiones_dinamicas() {
	return string_equals_ignore_case(algoritmo_memoria, "PARTICIONES");
}
