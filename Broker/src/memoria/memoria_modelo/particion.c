/*
 * particion.c
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#include "particion.h"

//Funciones Privadas
t_particion* particion_crear_y_ocupar(uint32_t tamanio, uint32_t inicio) {

	t_particion* particion = particion_inicializar(tamanio, inicio);

	memoria_actualizar_tamanio_disponible_sin_particionar(tamanio);
	//TODO: ojo con este if, real programmers do not use ifs.
	if (esquema_de_memoria_particiones_dinamicas()) {
		list_add(particiones, particion);
	}

	return particion;
}

t_particion* particion_crear_libre(uint32_t tamanio, uint32_t inicio, uint32_t index) {
	//TODO: esto está acoplado a particiones dinámicas
	t_particion* particion = particion_inicializar(tamanio, inicio);
	list_add_in_index(particiones, index, particion);

	return particion;
}

t_particion* particion_crear_dummy(uint32_t tamanio, uint32_t inicio) {

	t_particion* particion = particion_inicializar(tamanio, inicio);
	particion->id_mensaje = -1;

	return particion;
}

void particion_destruir(t_particion* particion) {
	free(particion);
}

void particion_liberar(t_particion* particion) {

	gettimeofday(&particion->creacion, NULL);
	particion->en_uso = false;
	particion->ultimo_acceso = particion->creacion;
}

bool particion_tiene_tamanio_suficiente(t_particion* particion, uint32_t tamanio_contenido) {
	return particion_esta_libre(particion) && particion_get_tamanio(particion) >= tamanio_contenido;
}

bool particion_es_best_fit(t_particion* particion, uint32_t tamanio_contenido, uint32_t min_fragmentacion) {

	return particion_tiene_tamanio_suficiente(particion, tamanio_contenido)
			&& (particion_get_tamanio(particion) - tamanio_contenido < min_fragmentacion);
}

bool particion_esta_libre(t_particion* particion) {
	return !particion_esta_ocupada(particion);
}

bool particion_esta_ocupada(t_particion* particion) {
	return particion->en_uso;
}

bool particion_encontrada(t_particion* particion) {
	return particion != NULL;
}

bool particion_es_dummy(t_particion* particion) {
	return particion_get_id_mensaje(particion) == -1;
}

bool fecha_creacion_anterior(t_particion* victima_actual, t_particion* candidato) {
	return particion_esta_ocupada(candidato)
			&& get_fecha_en_microsegundos(victima_actual->creacion)
					> get_fecha_en_microsegundos(candidato->creacion);
}

bool lru_anterior(t_particion* victima_actual, t_particion* candidato) {
	return particion_esta_ocupada(candidato)
			&& get_fecha_en_microsegundos(victima_actual->ultimo_acceso)
					> get_fecha_en_microsegundos(candidato->ultimo_acceso);
}

t_particion* particion_fusionar(t_particion* una_particion, t_particion* otra_particion) {

	particion_liberar(una_particion);
	una_particion->limite = otra_particion->limite;

	return una_particion;
}

t_particion* particion_con_tamanio_exacto(t_particion* particion, uint32_t tamanio_necesario,
		uint32_t index_candidato) {
	// Delta representa la potencial fragmentación interna
	uint32_t delta = particion_get_tamanio(particion) - tamanio_necesario;
	// Si no hay fragmentación, estamos ok y no se realiza ninguna transformación
	if (delta == 0)
		return particion;
	// Reduce el tamaño de la particion acorde al tamaño del contenido
	particion_recalcular_limite(particion, tamanio_necesario);
	//El sobrante es una nueva particion libre que se inserta en la lista
	particion_crear_libre(delta, particion->limite + 1, index_candidato + 1);

	return particion;
}

void particion_recalcular_limite(t_particion* particion, uint32_t nuevo_tamanio) {
	particion->limite = particion->base + nuevo_tamanio - 1;
}

void particion_recalcular_direcciones(t_particion* particion, uint32_t nueva_base) {

	int tamanio = particion_get_tamanio(particion);

	particion->base = nueva_base;
	particion_recalcular_limite(particion, tamanio);
}

char* particion_to_string(t_particion* particion) {

	char *to_string = string_new();

	string_append_with_format(&to_string, " Detalle de la partición: \n");
	string_append_with_format(&to_string, " id mensaje: %d \n", particion_get_id_mensaje(particion));
	string_append_with_format(&to_string, " tipo: %s \n", get_nombre_cola(particion_get_id_cola(particion)));
	string_append_with_format(&to_string, " base: %d \n", particion_get_direccion_base_relativa(particion));
	string_append_with_format(&to_string, " limite: %d \n", particion->limite);
	string_append_with_format(&to_string, " tamaño: %d \n", particion_get_tamanio(particion));
	string_append_with_format(&to_string, " creación (μs): %lu \n",
			get_fecha_en_microsegundos(particion->creacion));
	string_append_with_format(&to_string, " último acceso (μs): %lu \n",
			get_fecha_en_microsegundos(particion->ultimo_acceso));

	return to_string;
}

char* particion_to_string_dump(t_particion* particion, int index) {

	char *to_string = string_new();

	string_append_with_format(&to_string, " Partición: %d: ", index);
	string_append_with_format(&to_string, "%06p", particion_get_direccion_base_absoluta(particion));
	//Debug: imprime las direcciones relativas
	//string_append_with_format(&to_string, "%ju", (uintmax_t) (particion_get_direccion_base_absoluta(particion) - memoria_principal));
	string_append_with_format(&to_string, " - ");
	string_append_with_format(&to_string, "%06p.	", particion_get_direccion_limite_absoluta(particion));
	//Debug: imprime las direcciones relativas
	//string_append_with_format(&to_string, "%ju.	     ", (uintmax_t) (particion_get_direccion_limite_absoluta(particion) - memoria_principal));
	string_append_with_format(&to_string, "[%c]	", particion_esta_libre(particion) ? 'L' : 'X');
	string_append_with_format(&to_string, "Size: %db	", particion_get_tamanio(particion));
	if (particion_esta_libre(particion)) {
		string_append_with_format(&to_string, "\n");
		return to_string;
	}
	string_append_with_format(&to_string, "LRU: %lu	", get_fecha_en_microsegundos(particion -> ultimo_acceso));
	string_append_with_format(&to_string, "Cola: %s   ", get_nombre_cola(particion_get_id_cola(particion)));
	string_append_with_format(&to_string, "ID: %d \n", particion_get_id_mensaje(particion));

	return to_string;
}

void string_append_particion(char** string, t_particion* particion) {

	char* string_particion = particion_to_string(particion);
	string_append_with_format(string, string_particion);
	free(string_particion);
}

uint32_t particion_get_tamanio(t_particion* particion) {
	return particion->limite - particion->base + 1;
}

uint32_t particion_get_id_mensaje(t_particion* particion) {
	return particion->id_mensaje;
}

uint32_t particion_get_id_cola(t_particion* particion) {
	return particion->id_cola;
}

uint32_t particion_get_direccion_base_relativa(t_particion* particion) {
	return particion->base;
}

uint32_t particion_get_direccion_limite_relativa(t_particion* particion) {
	return particion->limite;
}

void* particion_get_direccion_base_absoluta(t_particion* particion) {
	return memoria_principal + particion_get_direccion_base_relativa(particion);
}

void* particion_get_direccion_limite_absoluta(t_particion* particion) {
	return particion_get_direccion_base_absoluta(particion) + particion_get_tamanio(particion) - 1;
}

void particion_set_uso(t_particion* particion) {

	particion->en_uso = true;
	gettimeofday(&particion->creacion, NULL);
	particion->ultimo_acceso = particion->creacion;
}

void particion_actualizar_fecha_ultimo_acceso(t_particion* particion) {
	gettimeofday(&particion->ultimo_acceso, NULL);
}

void particion_set_id_cola(t_particion* particion, t_id_cola id_cola) {
	particion->id_cola = id_cola;
}

void particion_set_id_mensaje(t_particion* particion, uint32_t id_mensaje) {
	particion->id_mensaje = id_mensaje;
}

t_particion* particion_inicializar(uint32_t tamanio, uint32_t inicio) {

	t_particion* particion = malloc(sizeof(t_particion));

	particion_liberar(particion);
	particion->base = inicio;
	particion->limite = inicio + tamanio - 1;
	particion->id_cola = -1;
	particion->id_mensaje = 0;

	return particion;
}


