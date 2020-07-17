/*
 * buddy_system.c
 *
 *  Created on: 10 jul. 2020
 *      Author: utnso
 */

#include "buddy_system.h"

static void asignar_primeros_bloques();
static void liberar_victima(bool (*algoritmo_victima)(t_particion*, t_particion*));
static t_list* obtener_todas_las_particiones_ocupadas();
static t_bloque* buscar_bloque(t_list* bloques, t_particion* particion);
static void consolidar(t_bloque* bloque);
static t_list* obtener_todos_los_bloques();
static void remover_hijo_con_padres_ocupados(t_list* todos_los_bloques);

void inicializar_buddy_system() {

	bloques = dictionary_create();

	//"FF" o "BF"
	char* algoritmo_libres = config_get_string_value(config, "ALGORITMO_PARTICION_LIBRE");

	algoritmo_particion_libre =
			string_equals_ignore_case(algoritmo_libres, "BF") ?
					(void*) bs_obtener_particion_libre_best_fit : (void*) bs_obtener_particion_libre_first_fit;

	//"FIFO" o "LRU"
	char* algoritmo_reemplazo_string = config_get_string_value(config, "ALGORITMO_REEMPLAZO");

	algoritmo_reemplazo =
			string_equals_ignore_case(algoritmo_reemplazo_string, "FIFO") ?
					(void*) bs_liberar_victima_fifo : (void*) bs_liberar_victima_lru;

	obtener_particion_libre = (void*) bs_obtener_particion_libre;
	asignar_primeros_bloques();
}

t_particion* bs_obtener_particion_libre(int tamanio_contenido) {

	int offset = 0;
	int tamanio_particion = siguiente_potencia_de_dos(tamanio_contenido);

	t_particion* particion = algoritmo_particion_libre(tamanio_particion, &offset);

	if (particion_encontrada(particion))
		return particion;

	if (hay_espacio_contiguo_diponible(tamanio_particion)) {
		t_bloque* bloque = bloque_crear_y_ocupar(offset, tamanio_particion);
		return bloque_get_particion(bloque);
	}

	return NULL;
}

bool bloque_encontrado_best_fit(uint32_t tamanio_acumulado, int32_t tamanio_bloque, t_particion* particion) {
	return tamanio_acumulado == tamanio_total_memoria || particion_encontrada(particion);
}

t_particion* bs_obtener_particion_libre_best_fit(int tamanio_bloque, int* offset) {

	t_list* bloques_nivel_n;
	t_list* particiones_nivel_n;
	t_particion* particion = NULL;

	uint32_t tamanio_acumulado = tamanio_bloque;
	while (!bloque_encontrado_best_fit(tamanio_acumulado, tamanio_bloque, particion)) {

		bloques_nivel_n = obtener_bloques_libres_de_nivel_n(logaritmo_base_dos(tamanio_acumulado));
		//TODO: agregar pregunta por list empty. Ver codigo duplicado
		if (bloques_nivel_n != NULL) {
			particiones_nivel_n = obtener_particiones_de_bloques(bloques_nivel_n);
			int index_candidato = 0;
			particion = obtener_particion_libre_first_fit(particiones_nivel_n, tamanio_acumulado, offset,
					&index_candidato);
			list_destroy(particiones_nivel_n);
		}

		tamanio_acumulado = tamanio_acumulado * 2;
	}

	if (!particion_encontrada(particion))
		return NULL;

	t_bloque* bloque = bloque_con_tamanio_exacto(buscar_bloque(bloques_nivel_n, particion), tamanio_bloque);
	return bloque_get_particion(bloque);
}

bool bloque_encontrado_first_fit(uint32_t tamanio_acumulado, int32_t tamanio_bloque, t_particion* particion) {
	return tamanio_acumulado < tamanio_bloque || particion_encontrada(particion);
}

t_bloque* buscar_bloque(t_list* bloques, t_particion* particion) {

	bool buscar_por_id(t_bloque* bloque) {
		return bloque_get_id(bloque) == particion_get_id_mensaje(particion);
	}
	return list_find(bloques, (void*) buscar_por_id);
}

t_particion* bs_obtener_particion_libre_first_fit(int tamanio_bloque, int* offset) {

	t_list* bloques_nivel_n;
	t_list* particiones_nivel_n;
	uint32_t tamanio_buscado = tamanio_total_memoria / 2;
	t_particion* particion = NULL;
	int index_candidato = 0;

	while (!bloque_encontrado_first_fit(tamanio_buscado, tamanio_bloque, particion)) {

		bloques_nivel_n = obtener_bloques_libres_de_nivel_n(logaritmo_base_dos(tamanio_buscado));
		//TODO agregar pregunta por list_empty
		if (bloques_nivel_n != NULL) {
			particiones_nivel_n = obtener_particiones_de_bloques(bloques_nivel_n);
			particion = obtener_particion_libre_first_fit(particiones_nivel_n, tamanio_buscado, offset, &index_candidato);
			list_destroy(particiones_nivel_n);
		}

		tamanio_buscado = tamanio_buscado / 2;
	}

	if (!particion_encontrada(particion))
		return NULL;

	t_bloque* bloque = bloque_con_tamanio_exacto(buscar_bloque(bloques_nivel_n, particion), tamanio_bloque);
	bloque_set_uso(bloque);

	return bloque_get_particion(bloque);
}

//todo ver este codigo duplicado
void bs_liberar_victima_fifo() {

	liberar_victima((void*) fecha_creacion_anterior);
}

void bs_liberar_victima_lru() {

	liberar_victima((void*) lru_anterior);
}

static void liberar_victima(bool (*algoritmo_victima)(t_particion*, t_particion*)) {

	t_list* unas_particiones = obtener_todas_las_particiones_ocupadas();
	int index_victima = 0;
	t_particion* victima = obtener_victima((void*) algoritmo_victima, &index_victima, unas_particiones);
	t_list* bloques_nivel_n = obtener_bloques_de_nivel_n(logaritmo_base_dos(particion_get_tamanio(victima)));
	t_bloque* bloque = buscar_bloque(bloques_nivel_n, victima);

	pthread_mutex_lock(&mutex_escritura_eliminacion_memoria);

	cola_safe_buscar_y_eliminar_mensaje(particion_get_id_mensaje(victima), particion_get_id_cola(victima));
	consolidar(bloque);

	pthread_mutex_unlock(&mutex_escritura_eliminacion_memoria);

}

static void consolidar(t_bloque* bloque) {

	if (particion_esta_ocupada(bloque->particion))
		return;

	bloque_liberar(bloque);
	t_bloque* buddy = bloque_get_buddy(bloque);

	if (!bloque_esta_libre(buddy) || bloque_es_raiz(bloque))
		return;

	bloque_y_buddy_liberar_y_destruir(bloque, buddy);
	consolidar(bloque_get_padre(bloque));
}
//TODO ver que pasa si no hay bloques
//TODO codigo duplicado con obtener_particiones
static t_list* obtener_todos_los_bloques() {

	t_list* todos_los_bloques = list_create();
	int tamanio = tamanio_minimo_particion;
	int limite =
			tamanio_sin_particionar == tamanio_total_memoria ? tamanio_total_memoria : tamanio_total_memoria / 2;

	while (tamanio <= limite) {

		t_list* bloques_nivel_n = obtener_bloques_de_nivel_n(logaritmo_base_dos(tamanio));

		if (bloques_nivel_n != NULL) {
			//TODO: validar lista vacia
			list_add_all(todos_los_bloques, bloques_nivel_n);
		}

		tamanio = tamanio * 2;
	}
	return todos_los_bloques;
}

static t_list* obtener_todas_las_particiones_ocupadas() {

	t_list* todas_las_particiones = list_create();
	int tamanio = tamanio_total_memoria / 2;

	while (true) {

		t_list* unas_particiones = obtener_particiones_ocupadas_de_nivel_n(tamanio);
		if (unas_particiones == NULL) {
			//TODO ver si va un continue y agregar !list_is_empty
			break;
		} else {
			list_add_all(todas_las_particiones, unas_particiones);
		}

		tamanio = tamanio / 2;
	}
	return todas_las_particiones;
}

static void remover_hijo_con_padres_ocupados(t_list* todos_los_bloques) {

	t_list* copia = list_duplicate(todos_los_bloques);

	void remover(t_bloque* bloque_hijo) {

		if (!bloque_esta_libre(bloque_hijo)) {

			bool es_padre(t_bloque* bloque_padre) {
				return bloque_get_id_padre(bloque_hijo) == bloque_get_id(bloque_padre);
			}

			list_remove_by_condition(todos_los_bloques, (void*) es_padre);
		}
	}

	list_iterate(copia, (void*) remover);
	list_destroy(copia);
}

t_list* bs_obtner_particiones_dump() {

	t_list* todos_los_bloques = obtener_todos_los_bloques();

	remover_hijo_con_padres_ocupados(todos_los_bloques);

	t_list* particiones_dump = obtener_particiones_de_bloques(todos_los_bloques);
	list_destroy(todos_los_bloques);

	return particiones_dump;

}

static void asignar_primeros_bloques() {

	t_bloque* bloque_raiz = bloque_crear(0, tamanio_total_memoria, 0);
	uint32_t mitad_memoria = tamanio_total_memoria / 2;

	bloque_crear(0, mitad_memoria, bloque_get_id(bloque_raiz));
	bloque_crear(mitad_memoria, mitad_memoria, bloque_get_id(bloque_raiz));
}
