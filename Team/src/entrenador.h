# ifndef _TAD_ENTRENADOR_
# define _TAD_ENTRENADOR_ //PASAR A COMMONS

#include "auxiliares.h"

typedef char* especie_pokemon;
typedef t_list* entrenadores;


//TAD Entrenador
typedef struct Entrenador{
	t_list* objetivos; //especie de los pokemones
	coordenadas posicion; //(x,y)
} entrenador;

//Constructor
entrenador entrenadorCreate(t_list* especiesObjetivos, coordenada X, coordenada Y);

//Desplaza un entrenador de una posicion a otra
void ir_a(entrenador unEntrenador, coordenadas);

//Pasa entrenador a estado locked
void bloquear(entrenador*);


//Cola de entrenadores

//Dice que entrenador esta mas cerca de una posicion
entrenador* entrenador_mas_cerca_de(entrenadores, coordenadas);

//Constructor
entrenadores entrenadores_create();


# endif
