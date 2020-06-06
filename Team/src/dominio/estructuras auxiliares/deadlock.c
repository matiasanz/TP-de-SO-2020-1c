#include "deadlock.h"
#include "../../hilos del team/detector de deadlock.h"

//******************************************************

candidato_deadlock* candidato_deadlock_create(entrenador*unEntrenador){
	candidato_deadlock* elemento = malloc(sizeof(candidato_deadlock));
				  *elemento = (candidato_deadlock) {unEntrenador, entrenador_recursos_pedidos(unEntrenador), entrenador_recursos_sobrantes(unEntrenador)};
	return elemento;
}

void candidato_destroy(candidato_deadlock* elemento){
	recursos_destroy(elemento->necesidad);
	recursos_destroy(elemento->sobrantes);
	free(elemento);
}

candidatos_deadlock candidatos_create(){
	return list_create();
}

void candidatos_agregar_entrenador(candidatos_deadlock candidatos, entrenador*unEntrenador){
	candidato_deadlock* candidato = candidato_deadlock_create(unEntrenador);
	candidatos_agregar_candidato(candidatos, candidato);
}

void candidatos_agregar_candidato(candidatos_deadlock candidatos, candidato_deadlock* candidato){
	list_add(candidatos, candidato);
}

void candidatos_destroy(candidatos_deadlock candidatos){
	list_destroy_and_destroy_elements(candidatos, (void(*)(void*))&candidato_destroy);
}

//******************************************************
//Pares de intercambio

par_intercambio par_intercambio_crear(candidato_deadlock*unEntrenador, candidato_deadlock*otroEntrenador){
	return (par_intercambio) {unEntrenador, otroEntrenador};
}

par_intercambio* par_intercambio_create(candidato_deadlock* unEntrenador, candidato_deadlock*otro){
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
