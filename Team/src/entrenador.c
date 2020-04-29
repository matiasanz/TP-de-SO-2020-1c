#include "entrenador.h"
#include "posicion.h"
#include <commons/string.h>

//Constructor Entrenador
entrenador entrenadorCreate(t_list* especiesDePokemones, coordenada coordenadaX, coordenada coordenadaY){ //Va en gameBoy y crea al empaquetado
	entrenador nuevo;
		nuevo.objetivos = especiesDePokemones;
		nuevo.pokemonesCazados = list_create();
		posicion_create(&nuevo.posicion, coordenadaX, coordenadaY);

	return nuevo;
}

entrenador* entrenadores_mas_cercano(entrenadores equipo, coordenadas posicion){
	//agregar validaciones;
	if(list_is_empty(equipo)){
		error_show("El equipo esta vacio");
		exit(1);
	}
	return equipo->head->data;
}

void entrenador_ir_a(entrenador* unEntrenador, coordenadas posicion){
	int i;

	for(i=0; i<DIMENSION_MAPA; i++){
		unEntrenador->posicion[i] = posicion[i];
	}
}

bool entrenador_puede_cazar_mas_pokemones(entrenador unEntrenador){
	return list_size(unEntrenador.objetivos) == list_size(unEntrenador.pokemonesCazados);
}

void entrenador_bloquear(entrenador* unEntrenador){
	puts("Se ha bloqueado al entrenador");
	//TODO; ver si vale la pena definir o si lo puedo detectar por condicion de atributos
}

void especie_destroy(especie_pokemon* especie){
	free(especie);
}

void entrenador_destroy(entrenador* destruido){
//Ver por que no le gusta
//	list_destroy_and_destroy_elements(destruido->objetivos, (void(*)(void*)) &especie_destroy);
	posicion_destroy(destruido->posicion);
}

//***************************************************************************
//Constructor de equipo
entrenadores entrenadores_create(){
	return list_create();
}

//Destructor de equipo
void entrenadores_destroy(entrenadores equipo){
	list_destroy_and_destroy_elements(equipo, (void(*)(void*)) &entrenador_destroy);
}

