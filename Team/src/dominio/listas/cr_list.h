#ifndef _TAD_CR_LIST_
#define _TAD_CR_LIST_

#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct{
	t_list* lista;
	sem_t hayMas;
	pthread_mutex_t* mutex;
}cr_list;

//Constructor de lista vacia con semaforos activos
cr_list* cr_list_create();

//Crea una cr_lista a partir de una lista
cr_list* cr_list_from_list();

//Destruye los elementos de la lista
void cr_list_clean_and_destroy_elements(cr_list*, void(*element_destroyer)(void*));

//Destruye lista y semaforos, deja los elementos
void cr_list_destroy(cr_list*);

//Destruye lista, elementos y semaforos
void cr_list_destroy_and_destroy_elements(cr_list*, void(*element_destroyer)(void*));

//Para casos de listas duplicadas, deja el mutex activo por si lo usa otra lista.
void cr_duplicated_list_destroy(cr_list*);

//Agrega un elemento al final y hace signal
int cr_list_add_and_signal(cr_list*, void *element);

//Agrega un elemento al principio y hace signal
void cr_list_add_first_and_signal(cr_list*, void *element);

//Agrega un elemento en una posicion y hace signal
void cr_list_add_index_and_signal(cr_list*, int index, void *element);

//Agrega todos los elementos de una lista y hace tal cantidad de signals
void cr_list_add_all_from_list_and_signal(cr_list*, t_list* other);

//Si hay algo retorna un puntero y sino retorna null
void* cr_list_get(cr_list*, int index);

//Espera y lee un elemento de una lista sin eliminarlo
void * cr_list_wait_and_get(cr_list*, int index);

//Espera y remueve un elemento de una lista
void* cr_list_wait_and_remove(cr_list* unaLista, int index);

//Retorna un puntero al elemento que coincide con el dado segun el criterio
void* cr_list_find(cr_list* unaLista, void* elemento, bool(*cmpTT)(void*, void*));

//Retorna una nueva lista de los que cumplen esa condicion
cr_list* cr_list_filter(cr_list*, bool(*condition)(void*));

//Retorna un puntero al primer elemento de la lista que cumpla la condicion
void* cr_list_get_by_condition(cr_list* unaLista, bool(*closure)(void*));

//Remueve al primer elemento de la lista que cumpla la condicion y lo elimina
void* cr_list_remove_by_condition(cr_list* unaLista, bool(*condicion)(void*));

//Remueve todos los elementos de la lista y los retorna en una lista nueva
t_list* cr_list_remove_all(cr_list*);

//Retorna true si la lista esta vacia
bool cr_list_is_empty(cr_list* unaLista);

//Ordena la lista de acuerdo al criterio dado
void cr_list_sort(cr_list*, bool (*comparator)(void *, void *));

//Aplica la funcion a cada elemento de la lista
void cr_list_iterate(cr_list*, void(*funcion)(void*));

//Retorna true si todos los elementos de la lista cumplen la condicion
bool cr_list_all(cr_list*, bool(*condition)(void*));

//Retorna true si al menos un elemento de la lista cumple la condicion
bool cr_list_any(cr_list*, bool(*condition)(void*));

//Retorna true si algun elemento de la lista coincide, segun el criterio dado
bool cr_list_contains_element(cr_list*, void* elemento, bool (*comparator)(void *, void *));

//****************************************************************************************

//Retorna true si algun elemento de la lista coincide, segun el criterio dado
bool list_contains_element(t_list*, void* elemento, bool (*comparator)(void *, void *));

//TODO descripcion (se entiende igual)
void* list_remove_by_comparation(t_list* unaLista, void* elemento, bool (*comparator)(void *, void *));

//Retorna un puntero al elemento si es que lo encontro
void* list_get_by_comparation(t_list* unaLista, void* elemento, bool (*comparator)(void *, void *));

//****************************************************************************************
#include <commons/collections/dictionary.h>

//Retorna la key de algun elemento que cumpla la condicion
	char* dictionary_get_any_key(t_dictionary* self, bool(*condition)(char*, void*));

# endif
