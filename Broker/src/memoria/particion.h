/*
 * particion.h
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_MEMORIA_PARTICION_H_
#define SRC_MEMORIA_PARTICION_H_

#include "memoria_utils.h"
#include <crenito-commons/utils.h>

typedef struct {
	uint32_t id_mensaje;
	t_id_cola id_cola;
	uint32_t base;
	uint32_t limite;
	uint32_t en_uso;
	struct timeval creacion;
	struct timeval ultimo_acceso;
} t_particion;

//Creación y destrucción
t_particion* particion_crear_y_ocupar(uint32_t tamanio, uint32_t inicio);
t_particion* particion_crear_libre(uint32_t tamanio, uint32_t inicio, uint32_t index);
void particion_destruir(t_particion* particion);
void particion_liberar(t_particion* particion);

//Funciones de consulta
bool particion_tiene_tamanio_suficiente(t_particion* particion, uint32_t tamanio_contenido);
bool particion_es_best_fit(t_particion* particion, uint32_t tamanio_contenido, uint32_t min_fragmentacion);
bool particion_esta_libre(t_particion* particion);
bool particion_esta_ocupada(t_particion* particion);
bool particion_encontrada(t_particion* particion);

//Algoritmos eleccion de victima
bool fecha_creacion_anterior(t_particion* una_particion, t_particion* otra_particion);
bool lru_anterior(t_particion* una_particion, t_particion* otra_particion);

// Transformaciones
t_particion* particion_fusionar(t_particion* una_particion, t_particion* otra_particion);
t_particion* particion_con_tamanio_exacto(t_particion* particion, uint32_t tamanio_contenido, uint32_t index_candidato);
void particion_recalcular_limite(t_particion* particion, uint32_t nuevo_tamanio);
void particion_recalcular_direcciones(t_particion* particion, uint32_t nueva_base);

// Log
char* particion_to_string(t_particion* particion);
void  particion_log_almacenamiento(t_log* un_logger, t_particion* particion);
void  particion_log_eliminacion(t_log* un_logger, t_particion* particion);

//Getters
uint32_t particion_get_tamanio(t_particion* particion);
uint32_t particion_get_base(t_particion* particion);
uint32_t particion_get_id_mensaje(t_particion* particion);
uint32_t particion_get_id_cola(t_particion* particion);
void* particion_get_direccion_contenido(t_particion* particion);

//Setters
void particion_set_uso(t_particion* particion);
void particion_set_id_cola(t_particion* particion, t_id_cola id_cola);
void particion_set_id_mensaje(t_particion* particion);

#endif /* SRC_MEMORIA_PARTICION_H_ */
