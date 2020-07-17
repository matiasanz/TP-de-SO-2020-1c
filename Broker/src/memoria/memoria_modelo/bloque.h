/*
 * bloque.h
 *
 *  Created on: 15 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_MEMORIA_MEMORIA_MODELO_BLOQUE_H_
#define SRC_MEMORIA_MEMORIA_MODELO_BLOQUE_H_

#include "math.h"
#include "particion.h"
#include "../../utils/broker_log.h"

typedef struct {
	t_particion* particion;
	bool en_uso; //indica si la partición está ocupada o es un bloque padre y algún hijo está ocupado
	uint32_t id_padre;
	uint32_t tamanio; //TODO ver si puede ser autocalculable
} t_bloque;

//Creación y destrucción
t_bloque* bloque_crear(uint32_t inicio, uint32_t tamanio_contenido, uint32_t id_padre);
t_bloque* bloque_crear_y_ocupar(uint32_t inicio_bloque, uint32_t tamanio_bloque);

void bloque_destruir(t_bloque* bloque);
void bloque_liberar(t_bloque* bloque);
void bloque_y_buddy_liberar_y_destruir(t_bloque* bloque, t_bloque* buddy);

//Funciones de consulta
bool bloque_esta_libre(t_bloque* bloque);
bool bloque_es_raiz(t_bloque* bloque);
bool bloque_es_hoja(t_bloque* bloque);

t_list* obtener_bloques_de_nivel_n(uint32_t tamanio);
t_list* obtener_bloques_libres_de_nivel_n(uint32_t tamanio);
t_list* obtener_particiones_ocupadas_de_nivel_n(uint32_t tamanio);
t_list* obtener_particiones_de_bloques(t_list* unos_bloques);

//Transforimaciones
t_bloque* bloque_con_tamanio_exacto(t_bloque* bloque, uint32_t tamanio_contenido);

//Getters
uint32_t bloque_get_id(t_bloque* bloque);
uint32_t bloque_get_tamanio(t_bloque* bloque);
char* bloque_get_tamanio_string(t_bloque* bloque);
t_particion* bloque_get_particion(t_bloque* bloque);
t_bloque* bloque_get_buddy(t_bloque* un_bloque);
t_bloque* bloque_get_padre(t_bloque* bloque);
uint32_t bloque_get_id_padre(t_bloque* bloque);

uint32_t bloque_calcular_tamanio_particion(t_bloque* bloque);

//Setters
void bloque_set_uso(t_bloque* bloque);
void bloque_set_tamanio(t_bloque* bloque, uint32_t tamanio);
void bloque_set_id_padre(t_bloque* bloque, uint32_t id_padre);

#endif /* SRC_MEMORIA_MEMORIA_MODELO_BLOQUE_H_ */
