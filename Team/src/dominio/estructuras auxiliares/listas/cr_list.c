#include "cr_list.h"

//Ver a futuro
void* cr_list_entre_mutex(cr_list*unaLista, void*(*funcionDeLista)(t_list*, void*), void* argumento){
	pthread_mutex_lock(unaLista->mutex);
	void* retorno = funcionDeLista(unaLista->lista, argumento);
	pthread_mutex_unlock(unaLista->mutex);

	return retorno;
}

cr_list* cr_list_construct(t_list*lista, pthread_mutex_t*semaforoMutex){
	cr_list* nueva = malloc(sizeof(cr_list));
	nueva->lista = lista;
	nueva->mutex = semaforoMutex;

	sem_init(&nueva->hayMas, 0, list_size(lista));

	return nueva;
}

cr_list* cr_list_create(){
	pthread_mutex_t* unMutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(unMutex, NULL);
	return cr_list_construct(list_create(), unMutex);
}

void cr_list_destroy(cr_list* unaLista){
	list_destroy(unaLista->lista);
	sem_destroy(&unaLista->hayMas);
	pthread_mutex_destroy(unaLista->mutex);
	free(unaLista);
}

void cr_list_destroy_and_destroy_elements(cr_list* unaLista, void(*element_destroyer)(void*)){
	list_clean_and_destroy_elements(unaLista->lista, element_destroyer);
	cr_list_destroy(unaLista);
}

/**
* @NAME: cr_listadd
* @DESC: Agrega un elemento al final de la lista
*/
int cr_list_add_and_signal(cr_list* unaLista, void *element){
	pthread_mutex_lock(unaLista->mutex);
	int i = list_add(unaLista->lista, element);
	pthread_mutex_unlock(unaLista->mutex);
	sem_post(&unaLista->hayMas);

	return i;
}

void cr_list_add_first_and_signal(cr_list* unaLista, void *element){
	cr_list_add_index_and_signal(unaLista, 0, element);
}

void cr_list_add_index_and_signal(cr_list* unaLista, int index, void *element){
	pthread_mutex_lock(unaLista->mutex);
	list_add_in_index(unaLista->lista, index, element);
	pthread_mutex_unlock(unaLista->mutex);
	sem_post(&unaLista->hayMas);
}

void cr_list_add_all_from_list_and_signal(cr_list* unaLista, t_list* other){
	int cantidad = list_size(other);

	pthread_mutex_lock(unaLista->mutex);
	list_add_all(unaLista->lista, other);
	pthread_mutex_unlock(unaLista->mutex);

	int i=0;
	for(i=0; i<cantidad; i++){
		sem_post(&unaLista->hayMas);
	}
}

void* cr_list_get(cr_list* unaLista, int index){
	pthread_mutex_lock(unaLista->mutex);
	void*elemento = list_get(unaLista->lista, index);
	pthread_mutex_unlock(unaLista->mutex);

	return elemento;
}

void * cr_list_wait_and_get(cr_list* unaLista, int index){
	sem_wait(&unaLista->hayMas); //Para que lea necesita que haya

	pthread_mutex_lock(unaLista->mutex);
	void*elemento = list_get(unaLista->lista, index);
	pthread_mutex_unlock(unaLista->mutex);

	sem_post(&unaLista->hayMas); //Lee pero no retira

	return elemento;
}


void* cr_list_wait_and_remove(cr_list* unaLista, int index){
	sem_wait(&unaLista->hayMas);

	pthread_mutex_lock(unaLista->mutex);
	void*elemento = list_remove(unaLista->lista, index);
	pthread_mutex_unlock(unaLista->mutex);

	return elemento;
}

void* cr_list_remove_by_condition(cr_list* unaLista, bool(*condicion)(void*)){
//	return cr_list_entre_mutex(unaLista, (void*(*)(t_list*, void*)) &list_remove_by_condition, &condicion);

	pthread_mutex_lock(unaLista->mutex);
	void* elemento = list_remove_by_condition(unaLista->lista, condicion);
	pthread_mutex_unlock(unaLista->mutex);

	return elemento;
}

void* cr_list_remove_by_comparation(cr_list* unaLista, void*element, bool(*cmpTK)(void*, void*)){
	bool comparacion(void*deLista){
		return cmpTK(element, deLista);
	}

//	return cr_list_entre_mutex(unaLista, (void*(*)(t_list*, void*))list_remove_by_condition, (void*) &comparacion);

	pthread_mutex_lock(unaLista->mutex);
	void* elemento = list_remove_by_condition(unaLista->lista, &comparacion);
	pthread_mutex_unlock(unaLista->mutex);

	return elemento;
}

cr_list* cr_list_filter(cr_list* unaLista, bool(*condition)(void*)){
	pthread_mutex_lock(unaLista->mutex);
	t_list*filtrada = list_filter(unaLista->lista, condition);
	pthread_mutex_unlock(unaLista->mutex);

	return cr_list_construct(filtrada, unaLista->mutex);
}

bool cr_list_is_empty(cr_list* unaLista){
	pthread_mutex_lock(unaLista->mutex);
	bool estaVacia = list_is_empty(unaLista->lista);
	pthread_mutex_unlock(unaLista->mutex);

	return estaVacia;
}

void* cr_list_find(cr_list* unaLista, void* elemento, bool(*cmpTT)(void*, void*)){

	bool comparar(void*deLaLista){
		return cmpTT(elemento, deLaLista);
	}

	return (void*) cr_list_get_by_condition(unaLista, (void*) comparar);
}

void* cr_list_get_by_condition(cr_list* unaLista, bool(*closure)(void*)){
	pthread_mutex_lock(unaLista->mutex);
	void*elemento = list_find(unaLista->lista, closure);
	pthread_mutex_unlock(unaLista->mutex);
	return elemento;
}

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
//void *list_replace(cr_list, int index, void* element);
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
//void *list_find(t_list *, bool(*closure)(void*));
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
