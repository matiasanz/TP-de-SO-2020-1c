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

void memoria_actualizar_tamanio_disponible_sin_particionar(uint32_t tamanio_contenido) {

	//TODO: ver este if
	uint32_t delta = esquema_de_memoria_particiones_dinamicas() ? tamanio_contenido : siguiente_potencia_de_dos(tamanio_contenido);
	tamanio_sin_particionar -= delta;
}

void memoria_reset_tamanio_disponible_sin_particionar(uint32_t nuevo_tamanio) {
	tamanio_sin_particionar = nuevo_tamanio;
}

uint32_t calcular_tamanio_particion(uint32_t tamanio_contenido) {
	return max(tamanio_minimo_particion, tamanio_contenido);
}

bool esquema_de_memoria_particiones_dinamicas() {
	return string_equals_ignore_case(algoritmo_memoria, "PARTICIONES");
}
