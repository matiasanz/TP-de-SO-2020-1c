#include "cr_list.h"

bool list_contains_element(t_list* unaLista, void* elemento, bool (*comparator)(void *, void *)){
	return list_get_by_comparation(unaLista, elemento, comparator);
}

void list_remove_and_destroy_all_satisfying(t_list*lista, bool (*condicion)(void*), void (*element_destroyer)(void*)){
	int cantidadDeOcurrencias = list_count_satisfying(lista, condicion);

	while(cantidadDeOcurrencias--){
		list_remove_and_destroy_by_condition(lista, condicion, element_destroyer);
	}
}

void* list_remove_by_comparation(t_list* unaLista, void* elemento, bool (*comparator)(void *, void *)){
	bool es_el_elemento(void*deLista){
		return comparator(elemento, deLista);
	}

	return list_remove_by_condition(unaLista, &es_el_elemento);
}

void* list_get_by_comparation(t_list* unaLista, void* elemento, bool (*comparator)(void *, void *)){
	bool es_el_elemento(void*deLista){
		return comparator(elemento, deLista);
	}

	return list_find(unaLista, &es_el_elemento);
}

//*****************************************************************************************

char* dictionary_get_any_key(t_dictionary* self, bool(*condition)(char*, void*)){
	int table_index;
	for (table_index = 0; table_index < self->table_max_size; table_index++) {
		t_hash_element *element = self->elements[table_index];

		while (element != NULL) {

			if(condition(element->key, element->data)){
				return element->key;
			}

			element = element->next;
		}
	}

	return NULL;
}

//******************************************************************************************


//Ver a futuro, no logre que anduviera
void* cr_list_entre_mutex(cr_list*unaLista, void*(*funcionDeLista)(t_list*, void*), void* argumento){
	pthread_mutex_lock(&unaLista->mutex);
	void* retorno = funcionDeLista(unaLista->lista, argumento);
	pthread_mutex_unlock(&unaLista->mutex);

	return retorno;
}

cr_list* cr_list_create(){
	return cr_list_from_list(list_create());
}

cr_list* cr_list_from_list(t_list*unaLista){

	cr_list* nueva = malloc(sizeof(cr_list));
	nueva->lista = unaLista;
	pthread_mutex_init(&nueva->mutex, NULL);
	sem_init(&nueva->hayMas, 0, list_size(unaLista));

	return nueva;
}

void cr_list_destroy(cr_list* unaLista){
	list_destroy(unaLista->lista);
	sem_destroy(&unaLista->hayMas);
	pthread_mutex_destroy(&unaLista->mutex);
	free(unaLista);
}

void cr_list_destroy_and_destroy_elements(cr_list* unaLista, void(*element_destroyer)(void*)){
	list_clean_and_destroy_elements(unaLista->lista, element_destroyer);
	cr_list_destroy(unaLista);
}

void cr_list_clean_and_destroy_elements(cr_list* unaLista, void(*element_destroyer)(void*)){
	pthread_mutex_lock(&unaLista->mutex);
	list_clean_and_destroy_elements(unaLista->lista, element_destroyer);
	pthread_mutex_unlock(&unaLista->mutex);
}

/**
* @NAME: cr_listadd
* @DESC: Agrega un elemento al final de la lista
*/
int cr_list_add_and_signal(cr_list* unaLista, void *element){
	pthread_mutex_lock(&unaLista->mutex);
	int i = list_add(unaLista->lista, element);
	pthread_mutex_unlock(&unaLista->mutex);
	sem_post(&unaLista->hayMas);

	return i;
}

void cr_list_add_first_and_signal(cr_list* unaLista, void *element){
	cr_list_add_index_and_signal(unaLista, 0, element);
}

void cr_list_add_index_and_signal(cr_list* unaLista, int index, void *element){
	pthread_mutex_lock(&unaLista->mutex);
	list_add_in_index(unaLista->lista, index, element);
	pthread_mutex_unlock(&unaLista->mutex);
	sem_post(&unaLista->hayMas);
}

void cr_list_add_all_from_list_and_signal(cr_list* unaLista, t_list* other){
	int cantidad = list_size(other);

	pthread_mutex_lock(&unaLista->mutex);
	list_add_all(unaLista->lista, other);
	pthread_mutex_unlock(&unaLista->mutex);

	int i=0;
	for(i=0; i<cantidad; i++){
		sem_post(&unaLista->hayMas);
	}
}

void* cr_list_get(cr_list* unaLista, int index){
	pthread_mutex_lock(&unaLista->mutex);
	void*elemento = list_get(unaLista->lista, index);
	pthread_mutex_unlock(&unaLista->mutex);

	return elemento;
}

void * cr_list_wait_and_get(cr_list* unaLista, int index){
	sem_wait(&unaLista->hayMas); //Para que lea necesita que haya

	pthread_mutex_lock(&unaLista->mutex);
	void*elemento = list_get(unaLista->lista, index);
	pthread_mutex_unlock(&unaLista->mutex);

	sem_post(&unaLista->hayMas); //Lee pero no retira

	return elemento;
}


void* cr_list_wait_and_remove(cr_list* unaLista, int index){
	sem_wait(&unaLista->hayMas);

	pthread_mutex_lock(&unaLista->mutex);
	void*elemento = list_remove(unaLista->lista, index);
	pthread_mutex_unlock(&unaLista->mutex);

	return elemento;
}

void* cr_list_remove_by_condition(cr_list* unaLista, bool(*condicion)(void*)){
//	return cr_list_entre_mutex(unaLista, (void*(*)(t_list*, void*)) &list_remove_by_condition, &condicion);

	pthread_mutex_lock(&unaLista->mutex);
	void* elemento = list_remove_by_condition(unaLista->lista, condicion);
	pthread_mutex_unlock(&unaLista->mutex);

	return elemento;
}

void* cr_list_remove_by_comparation(cr_list* unaLista, void*element, bool(*cmpTK)(void*, void*)){
	bool comparacion(void*deLista){
		return cmpTK(element, deLista);
	}

//	return cr_list_entre_mutex(unaLista, (void*(*)(t_list*, void*))list_remove_by_condition, (void*) &comparacion);

	pthread_mutex_lock(&unaLista->mutex);
	void* elemento = list_remove_by_condition(unaLista->lista, &comparacion);
	pthread_mutex_unlock(&unaLista->mutex);

	return elemento;
}

t_list* cr_list_remove_all(cr_list*unaLista){
	pthread_mutex_lock(&unaLista->mutex);
	cr_list*duplicada = unaLista;
	unaLista = cr_list_create();
	pthread_mutex_unlock(&unaLista->mutex);

	t_list*elementosRemovidos = duplicada->lista;
	cr_list_destroy(duplicada);

	return elementosRemovidos;
}

cr_list* cr_list_filter(cr_list* unaLista, bool(*condition)(void*)){
	pthread_mutex_lock(&unaLista->mutex);
	t_list*filtrada = list_filter(unaLista->lista, condition);
	pthread_mutex_unlock(&unaLista->mutex);

	return cr_list_from_list(filtrada);
}

bool cr_list_is_empty(cr_list* unaLista){
	pthread_mutex_lock(&unaLista->mutex);
	bool estaVacia = list_is_empty(unaLista->lista);
	pthread_mutex_unlock(&unaLista->mutex);

	return estaVacia;
}

void* cr_list_find(cr_list* unaLista, void* elemento, bool(*cmpTT)(void*, void*)){

	bool comparar(void*deLaLista){
		return cmpTT(elemento, deLaLista);
	}

	return (void*) cr_list_get_by_condition(unaLista, (void*) comparar);
}

void* cr_list_get_by_condition(cr_list* unaLista, bool(*closure)(void*)){
	pthread_mutex_lock(&unaLista->mutex);
	void*elemento = list_find(unaLista->lista, closure);
	pthread_mutex_unlock(&unaLista->mutex);
	return elemento;
}

void cr_list_sort(cr_list* unaLista, bool (*comparator)(void *, void *)){
	pthread_mutex_lock(&unaLista->mutex);
	list_sort(unaLista->lista, comparator);
	pthread_mutex_unlock(&unaLista->mutex);
}

void cr_list_iterate(cr_list*unaLista, void(*closure)(void*)){
	pthread_mutex_lock(&unaLista->mutex);
	list_iterate(unaLista->lista, closure);
	pthread_mutex_unlock(&unaLista->mutex);
}

bool cr_list_all(cr_list*unaLista, bool(*condition)(void*)){
	pthread_mutex_lock(&unaLista->mutex);
	bool todosCumplen = list_all_satisfy(unaLista->lista, condition);
	pthread_mutex_unlock(&unaLista->mutex);

	return todosCumplen;
}

bool cr_list_any(cr_list*unaLista, bool(*condition)(void*)){
	pthread_mutex_lock(&unaLista->mutex);
	bool algunoCumple = list_any_satisfy(unaLista->lista, condition);
	pthread_mutex_unlock(&unaLista->mutex);

	return algunoCumple;
}

bool cr_list_contains_element(cr_list* unaLista, void* elemento, bool (*comparator)(void *, void *)){
	pthread_mutex_lock(&unaLista->mutex);
	bool siONo = list_contains_element(unaLista->lista, elemento, comparator);
	pthread_mutex_unlock(&unaLista->mutex);
	return siONo;
}
