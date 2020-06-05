#include "deadlock.h"

//******************************************************

//candidato_intercambio* candidato_intercambio_create(entrenador*unEntrenador){
//	candidato_intercambio* elemento = malloc(sizeof(candidato_intercambio));
//				  *elemento = (candidato_intercambio) {unEntrenador, entrenador_recursos_pedidos(unEntrenador), entrenador_recursos_sobrantes(unEntrenador)};
//	return elemento;
//}
//
//void candidato_intercambio_destroy(candidato_intercambio* elemento){
//	recursos_destroy(elemento->necesidad);
//	recursos_destroy(elemento->sobrantes);
//	free(elemento);
//}

//******************************************************
//Pares de intercambio

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
