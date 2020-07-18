/*
 * bloque.c
 *
 *  Created on: 15 jul. 2020
 *      Author: utnso
 */

#include "bloque.h"

static void bloque_agregar_a_diccionario(t_bloque* bloque);
static t_bloque* bloque_obtener_padre_dado_tamanio_hijo(uint32_t direccion_base_relativa_hijo, uint32_t tamanio_particion_hijo);
static uint32_t bloque_calcular_tamanio_particion_padre(uint32_t tamanio_particion_hijo);
static t_bloque* buscar_primer_hijo_libre(t_bloque* bloque_padre);
static t_bloque* bloque_buscar_crear_hijos(t_bloque* bloque_padre);

t_bloque* bloque_crear(uint32_t inicio, uint32_t tamanio_contenido, uint32_t id_padre) {

	t_bloque* bloque = malloc(sizeof(t_bloque));

	bloque->particion = particion_inicializar(tamanio_contenido, inicio);
	particion_set_id_mensaje(bloque->particion, generar_id_univoco());
	bloque->en_uso = false;
	bloque->id_padre = id_padre;
	//Todo ver si el calculo del logaritmo debe estar en set_tamanio
	bloque_set_tamanio(bloque, logaritmo_base_dos(particion_get_tamanio(bloque->particion)));


	bloque_agregar_a_diccionario(bloque);

	return bloque;
}

t_bloque* bloque_crear_y_ocupar(uint32_t inicio_bloque, uint32_t tamanio_particion) {

	t_bloque* padre = bloque_obtener_padre_dado_tamanio_hijo(inicio_bloque, tamanio_particion);
	t_bloque* nuevo_bloque = bloque_crear(inicio_bloque, tamanio_particion, bloque_get_id(padre));
	bloque_set_uso(nuevo_bloque);
	return nuevo_bloque;
}

void bloque_destruir(t_bloque* bloque) {
	free(bloque);
}

void bloque_liberar(t_bloque* bloque) {
	bloque->en_uso = false;
}

void bloque_y_buddy_liberar_y_destruir(t_bloque* bloque, t_bloque* buddy) {

	//TODO: Ver este if horrible
	if (particion_get_direccion_base_relativa(bloque->particion) < particion_get_direccion_base_relativa(buddy->particion)) {
		log_asociacion_de_bloques(particion_get_direccion_base_absoluta(bloque->particion),
				particion_get_direccion_base_absoluta(buddy->particion));
	} else {
		log_asociacion_de_bloques(particion_get_direccion_base_absoluta(buddy->particion),
				particion_get_direccion_base_absoluta(bloque->particion));
	}

	bloque_liberar(buddy);
	char* tamanio_string = string_itoa(bloque_get_tamanio(bloque));
	t_list* bloques_de_nivel_n = dictionary_remove(bloques, tamanio_string);

	bool buscar_por_id(t_bloque* bloque_buscado) {
		return bloque_get_id(bloque_buscado) == bloque_get_id(bloque)
				|| bloque_get_id(bloque_buscado) == bloque_get_id(buddy);
	}

	list_remove_and_destroy_by_condition(bloques_de_nivel_n, (void*) buscar_por_id, (void*) bloque_destruir);
	list_remove_and_destroy_by_condition(bloques_de_nivel_n, (void*) buscar_por_id, (void*) bloque_destruir);

	if (list_is_empty(bloques_de_nivel_n))
		return;

	dictionary_put(bloques, tamanio_string, bloques_de_nivel_n);
}

bool bloque_esta_libre(t_bloque* bloque) {
	return !bloque->en_uso;
}

bool bloque_es_raiz(t_bloque* bloque) {
	return bloque_get_tamanio(bloque) == logaritmo_base_dos(tamanio_total_memoria);
}

bool bloque_es_hoja(t_bloque* bloque) {
	return bloque_calcular_tamanio_particion(bloque) == tamanio_minimo_particion;
}

t_list* obtener_bloques_de_nivel_n(uint32_t tamanio_logaritmico) {

	char* tamanio_string = string_itoa(tamanio_logaritmico);
	t_list* bloques_nivel_n = dictionary_get(bloques, tamanio_string);
	free(tamanio_string);

	return bloques_nivel_n;
}

t_list* obtener_bloques_libres_de_nivel_n(uint32_t tamanio) {

	t_list* bloques = obtener_bloques_de_nivel_n(tamanio);
	return bloques != NULL ? list_filter(bloques, (void*) bloque_esta_libre) : NULL;
}

t_list* obtener_particiones_ocupadas_de_nivel_n(uint32_t tamanio) {

	t_list* bloques_nivel_n = obtener_bloques_de_nivel_n(logaritmo_base_dos(tamanio));
	if (bloques_nivel_n == NULL)
		return NULL;

	t_list* particiones = obtener_particiones_de_bloques(bloques_nivel_n);

	return particiones != NULL ? list_filter(particiones, (void*) particion_esta_ocupada) : NULL;
}

t_list* obtener_particiones_de_bloques(t_list* unos_bloques) {

	return list_map(unos_bloques, (void*) bloque_get_particion);
}

t_bloque* bloque_con_tamanio_exacto(t_bloque* bloque, uint32_t tamanio_necesario) {

	if (bloque_calcular_tamanio_particion(bloque) == tamanio_necesario)
		return bloque;

	t_bloque* primer_hijo = bloque_buscar_crear_hijos(bloque);
	return bloque_con_tamanio_exacto(primer_hijo, tamanio_necesario);
}

uint32_t bloque_get_id(t_bloque* bloque) {
	return particion_get_id_mensaje(bloque->particion);
}

uint32_t bloque_get_tamanio(t_bloque* bloque) {

	return bloque->tamanio;
}

char* bloque_get_tamanio_string(t_bloque* bloque) {

//todo ver este casteo
	return string_itoa((int) bloque->tamanio);
}

t_particion* bloque_get_particion(t_bloque* bloque) {
	return bloque->particion;
}

t_bloque* bloque_get_buddy(t_bloque* un_bloque) {

	t_list* bloques_nivel_n = obtener_bloques_de_nivel_n(bloque_get_tamanio(un_bloque));

	bool es_buddy(t_bloque* otro_bloque) {
		return bloque_get_id(un_bloque) != bloque_get_id(otro_bloque)
				&& un_bloque->id_padre == otro_bloque->id_padre;
	}

	return bloques_nivel_n != NULL ? list_find(bloques_nivel_n, (void*) es_buddy) : NULL;
}

t_bloque* bloque_get_padre(t_bloque* bloque) {

	t_list* bloques_nivel_n = obtener_bloques_de_nivel_n(bloque_get_tamanio(bloque) + 1);

	bool es_padre(t_bloque* padre) {
		return bloque->id_padre == bloque_get_id(padre);
	}

	return list_find(bloques_nivel_n, (void*) es_padre);
}

uint32_t bloque_get_id_padre(t_bloque* bloque) {
	return bloque->id_padre;
}

uint32_t bloque_calcular_tamanio_particion(t_bloque* bloque) {
	return calcular_base_dos(bloque_get_tamanio(bloque));
}

void bloque_set_uso(t_bloque* bloque) {

	bloque->en_uso = true;

	if (bloque_calcular_tamanio_particion(bloque) == tamanio_total_memoria)
		return;

	t_bloque* padre = bloque_get_padre(bloque);

	if (bloque_es_hoja(bloque)) {
		//TODO: craer funcion buddy_esta_libre
		t_bloque* buddy = bloque_get_buddy(bloque);

		if (buddy != NULL && bloque_esta_libre(buddy)) return;
	}

	bloque_set_uso(padre);

}

void bloque_set_tamanio(t_bloque* bloque, uint32_t tamanio) {
	bloque->tamanio = tamanio;
}


void bloque_set_id_padre(t_bloque* bloque, uint32_t id_padre) {
	bloque->id_padre = id_padre;
}

static void bloque_agregar_a_diccionario(t_bloque* bloque) {

	t_list* bloques_nivel_n = obtener_bloques_de_nivel_n(bloque_get_tamanio(bloque));

	if (bloques_nivel_n == NULL) {
		bloques_nivel_n = list_create();
	}

	list_add(bloques_nivel_n, bloque);
	dictionary_put(bloques, bloque_get_tamanio_string(bloque), bloques_nivel_n);
}

static t_bloque* bloque_obtener_padre_dado_tamanio_hijo(uint32_t direccion_base_relativa_hijo, uint32_t tamanio_particion_hijo) {

	uint32_t tamanio_padre = bloque_calcular_tamanio_particion_padre(tamanio_particion_hijo);
	t_list* bloques_nivel_n = obtener_bloques_de_nivel_n(logaritmo_base_dos(tamanio_padre));

	bool es_padre(t_bloque* bloque) {

		uint32_t direcion_limite_relativa_hijo = direccion_base_relativa_hijo + tamanio_particion_hijo - 1;

		return particion_get_direccion_base_relativa(bloque->particion) == direccion_base_relativa_hijo
				|| particion_get_direccion_limite_relativa(bloque->particion) == direcion_limite_relativa_hijo;
	}

	return bloques_nivel_n != NULL ? list_find(bloques_nivel_n, (void*) es_padre) : NULL;
}

static uint32_t bloque_calcular_tamanio_particion_padre(uint32_t tamanio_particion_hijo) {
	return siguiente_potencia_de_dos(tamanio_particion_hijo + 1);
}

static t_bloque* buscar_primer_hijo_libre(t_bloque* bloque_padre) {

	t_list* bloques_de_nivel_n = obtener_bloques_de_nivel_n(bloque_get_tamanio(bloque_padre) - 1);

	//TODO ver si hay que fltrar tambien el que tenga direccion base mas chica
	bool es_hijo(t_bloque* bloque) {
		//TODO crear funcion es_padre y es_padre_libre
		return bloque_get_id_padre(bloque) == bloque_get_id(bloque_padre) && bloque_esta_libre(bloque);
	}

	return bloques_de_nivel_n != NULL ? list_find(bloques_de_nivel_n, (void*) es_hijo) : NULL;
}

static t_bloque* bloque_buscar_crear_hijos(t_bloque* bloque_padre) {

	t_bloque* primer_hijo = buscar_primer_hijo_libre(bloque_padre);

	if (primer_hijo != NULL)
		return primer_hijo;

	uint32_t tamanio_contenido = particion_get_tamanio(bloque_padre->particion) / 2;
	primer_hijo = bloque_crear(particion_get_direccion_base_relativa(bloque_padre->particion), tamanio_contenido, bloque_get_id(bloque_padre));

	if (bloque_get_buddy(primer_hijo) != NULL) return primer_hijo;

	bloque_crear(particion_get_direccion_limite_relativa(primer_hijo->particion) + 1, tamanio_contenido, bloque_get_id(bloque_padre));

	return primer_hijo;
}
