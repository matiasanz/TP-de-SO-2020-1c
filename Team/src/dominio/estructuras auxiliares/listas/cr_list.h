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

//constructor de lista vacia con semaforos activos
cr_list* cr_list_create();

//Destruye lista y semaforos, deja los elementos
void cr_list_destroy(cr_list*);

//Destruye lista, elementos y semaforos
void cr_list_destroy_and_destroy_elements(cr_list*, void(*element_destroyer)(void*));

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

//Retorna true si la lista esta vacia
bool cr_list_is_empty(cr_list* unaLista);
///**
//* @NAME: cr_listmap
//* @DESC: Retorna una nueva lista con los
//* con los elementos transformados
//*/
//cr_list cr_list_map(cr_list, void*(*transformer)(void*));
//
///**
//* @NAME: cr_listreplace
//* @DESC: Coloca un elemento en una de la posiciones
//* de la lista retornando el valor anterior
//*/
//void * cr_list_replace(cr_list, int index, void* element);
//
///**
//* @NAME: cr_listreplace_and_destroy_element
//* @DESC: Coloca un valor en una de la posiciones
//* de la lista liberando el valor anterior
//*/
//void cr_list_replace_and_destroy_element(cr_list, int index, void* element, void(*element_destroyer)(void*));
//
///**
//* @NAME: cr_listremove
//* @DESC: Remueve un elemento de la lista de
//* una determinada posicion y lo retorna.
//*/
//void *list_remove(t_list *, int index);
//
///**
//* @NAME: cr_listremove_and_destroy_element
//* @DESC: Remueve un elemento de la lista de una
//* determinada posicion y libera la memoria.
//*/
//void cr_list_remove_and_destroy_element(t_list *, int index, void(*element_destroyer)(void*));
//
///**
//* @NAME: cr_listremove_by_condition
//* @DESC: Remueve el primer elemento de la lista
//* que haga que condition devuelva != 0.
//*/
//void *list_remove_by_condition(t_list *, bool(*condition)(void*));
//
///**
//* @NAME: cr_listremove_and_destroy_by_condition
//* @DESC: Remueve y destruye el primer elemento de
//* la lista que hagan que condition devuelva != 0.
//*/
//void cr_list_remove_and_destroy_by_condition(t_list *, bool(*condition)(void*), void(*element_destroyer)(void*));
//
///**
//* @NAME: cr_listclean
//* @DESC: Quita todos los elementos de la lista.
//*/
//void cr_list_clean(t_list *);
//
///**
//* @NAME: cr_listclean
//* @DESC: Quita y destruye todos los elementos de la lista
//*/
//void cr_list_clean_and_destroy_elements(t_list *self, void(*element_destroyer)(void*));
//
///**
//* @NAME: cr_listiterate
//* @DESC: Itera la lista llamando al closure por cada elemento
//*/
//void cr_list_iterate(t_list *, void(*closure)(void*));
//
///**
//* @NAME: cr_listfind
//* @DESC: Retorna el primer valor encontrado, el cual haga que condition devuelva != 0
//*/
//
///**
//* @NAME: cr_listsize
//* @DESC: Retorna el tamaño de la lista
//*/
//int cr_list_size(t_list *);
//
///**
//* @NAME: cr_listis_empty
//* @DESC: Verifica si la lista esta vacia
//*/
//int cr_list_is_empty(t_list *);
//
///**
//* @NAME: cr_listsort
//* @DESC: Ordena la lista segun el comparador
//* El comparador devuelve si el primer parametro debe aparecer antes que el
//* segundo en la lista
//*/
//void cr_list_sort(t_list *, bool (*comparator)(void *, void *));
//
///**
//* @NAME: cr_listsorted
//* @DESC: Retorna una lista nueva ordenada segun el comparador
//* El comparador devuelve si el primer parametro debe aparecer antes que el
//* segundo en la lista
//*/
//cr_list cr_list_sorted(t_list *, bool (*comparator)(void *, void *));
//
///**
//* @NAME: cr_listcount_satisfying
//* @DESC: Cuenta la cantidad de elementos de
//* la lista que cumplen una condición
//*/
//int cr_list_count_satisfying(cr_list self, bool(*condition)(void*));
//
///**
//* @NAME: cr_listany_satisfy
//* @DESC: Determina si algún elemento
//* de la lista cumple una condición
//*/
//bool cr_list_any_satisfy(cr_list self, bool(*condition)(void*));
//
///**
//* @NAME: cr_listany_satisfy
//* @DESC: Determina si todos los elementos
//* de la lista cumplen una condición
//*/
//bool cr_list_all_satisfy(cr_list self, bool(*condition)(void*));
//
///**
//* @NAME: cr_listduplicate
//* @DESC: Crea una lista nueva con los mismos elementos que la original.
//**/
//cr_list cr_list_duplicate(cr_list self);
//
///**
// * @NAME: cr_listfold
// * @DESC: Devuelve un valor que resulta de aplicar la
// * operacion entre todos los elementos de la lista, partiendo desde el primero.
// *
// * La funcion 'operation' debe recibir 2 dos valores, uno del tipo del valor initial (seed)
// * y otro del tipo de los elementos de la lista.
// */
//void* cr_list_fold(cr_list self, void* seed, void*(*operation)(void*, void*));


# endif
