#include "pares en deadlock.h"

par_intercambio par_intercambio_crear(entrenador*unEntrenador, entrenador*otroEntrenador){
	return (par_intercambio) {unEntrenador->id, otroEntrenador};
}

par_intercambio* par_intercambio_create(entrenador* unEntrenador, entrenador*otro){
	par_intercambio*unaPareja = malloc(sizeof(par_intercambio));
				   *unaPareja = par_intercambio_crear(unEntrenador, otro);
	return unaPareja;
}

pares_intercambio pares_intercambio_create(){
	return list_create();
}

void pares_intercambio_agregar(pares_intercambio parejas, par_intercambio* unaPareja){
	list_add(parejas, unaPareja);
}
