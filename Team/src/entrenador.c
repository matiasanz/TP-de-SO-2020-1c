#include "auxiliares.h"
#include "entrenador.h"

//Constructor Entrenador
entrenador entrenadorCreate(t_list* especiesDePokemones, coordenada coordenadaX, coordenada coordenadaY){ //Va en gameBoy y crea al empaquetado
	entrenador nuevo;
		nuevo.objetivos = especiesDePokemones;
		posicion_create(&nuevo.posicion, coordenadaX, coordenadaY);

	return nuevo;
}

entrenador* entrenador_mas_cerca_de(entrenadores equipo, coordenadas posicion){
	//agregar validaciones;
	return equipo->head->data;
}

void ir_a(entrenador unEntrenador, coordenadas posicion){
	int i;

	for(i=0; i<DIMENSION_MAPA; i++){
		unEntrenador.posicion[i] = posicion[i];
	}
}

void bloquear(entrenador* unEntrenador){
	//TODO; ver si vale la pena definir o si lo puedo detectar
}


//Constructor de equipo
entrenadores entrenadores_create(){
	return list_create();
}
