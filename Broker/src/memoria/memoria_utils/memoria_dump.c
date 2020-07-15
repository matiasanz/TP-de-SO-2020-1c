/*
 * memoria_dump.c
 *
 *  Created on: 13 jul. 2020
 *      Author: utnso
 */

#include "../memoria_utils/memoria_dump.h"

static t_list* obtener_particiones_dump();
static char* get_dump_cache_string();
static void destruir_particiones_dummy(t_list* particiones_dump);

void log_dump_cache(int signum) {

	char* string = get_dump_cache_string();
	log_info_and_destroy(dump_logger, string);
}

static t_list* obtener_particiones_dump() {
	return esquema_de_memoria_particiones_dinamicas() ?
			pd_obtener_particiones_dump() : bs_obtner_particiones_dump();
}

static char* get_dump_cache_string() {

	char* string = string_new();
	string_append_with_format(&string, "\n");
	string_append_with_format(&string, SEPARADOR_DUMP_CACHE);
	string_append_timestamp(&string);

	pthread_mutex_lock(&mutex_acceso_memoria);

	t_list* particiones_dump = obtener_particiones_dump();
	for (int i = 0; i < list_size(particiones_dump); ++i) {
		string_append_with_format(&string, particion_to_string_dump(list_get(particiones_dump, i), i + 1));
	}

	pthread_mutex_unlock(&mutex_acceso_memoria);

	string_append_with_format(&string, SEPARADOR_DUMP_CACHE);
	destruir_particiones_dummy(particiones_dump);

	return string;
}

void log_debug_dump_cache() {

	//Debug
	//char* string = get_dump_cache_string();
	//log_info_and_destroy(event_logger, string);
}

static void destruir_particiones_dummy(t_list* particiones_dump) {

	/*TODO:
	 *  En particiones dinámicas solo puede haber 1 dummy. Si pasa lo mismo en BS cambiar este codigo por
	 *  list_remove_and_destroy_by_condition
	 */
	t_list* particiones_dummy = list_filter(particiones_dump, (void*) particion_es_dummy);
	list_iterate(particiones_dummy, (void*) particion_destruir);

	list_destroy(particiones_dump);
}
