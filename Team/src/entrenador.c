#include "entrenador.h"
#include "posicion.h"
#include <commons/string.h>
#include "mensajes.h"
#include <math.h>

//Constructor Entrenador
entrenador entrenadorCreate(t_list* especiesDePokemones, coordenada coordenadaX, coordenada coordenadaY){ //Va en gameBoy y crea al empaquetado
	entrenador nuevo;
		nuevo.objetivos = especiesDePokemones;
		nuevo.pokemonesCazados = list_create();
		posicion_create(&nuevo.posicion, coordenadaX, coordenadaY);

	return nuevo;
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
equipo equipo_create(){
	equipo unEquipo = list_create();

	puts("Se leyo un nuevo entrenador");

	entrenador* unEntrenador = entrenador_de_prueba();
	list_iterate(unEntrenador->objetivos, &get);//Le pregunto al gamecard si cada objetivo esta en alguna posicion
	list_add(unEquipo, unEntrenador);				//agrego el entrenador al equipo

	puts("Se agrego un nuevo entrenador");

	return unEquipo;
}

double posicion_distancia(coordenadas unaPosicion, coordenadas otraPosicion){
	int i; double sumaDeCuadrados=0;
	for(i=0; i<DIMENSION_MAPA; i++){
		sumaDeCuadrados+= (double) unaPosicion[i]*unaPosicion[i] + otraPosicion[i]*otraPosicion[i];
	}
	return 5;//sqrt(sumaDeCuadrados);//TODO
}

entrenador* equipo_mas_cercano(equipo unEquipo, coordenadas unaPosicion){

	entrenador*entrenador_mas_cercano(entrenador*uno, entrenador*otro){
		if(!uno){
			return otro;
		}

		return posicion_distancia(uno->posicion, unaPosicion)>=posicion_distancia(otro->posicion, unaPosicion)? uno : otro;
	}

	return list_fold(unEquipo, NULL, (void*(*)(void*, void*))&entrenador_mas_cercano);
}

//Ver si vale la pena poner CRITERIO, con enum
entrenador* equipo_proximo_a_planificar(equipo entrenadores){
	return list_remove(entrenadores, 0); //FIFO
}

//Destructor de equipo
void equipo_destroy(equipo entrenadores){
	list_destroy_and_destroy_elements(entrenadores, (void(*)(void*)) &entrenador_destroy);
}

