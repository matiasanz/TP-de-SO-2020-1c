# ifndef _TAD_ENTRENADOR_
# define _TAD_ENTRENADOR_ //PASAR A COMMONS

#include "posicion.h"

typedef char* especie_pokemon;

//TAD Entrenador
typedef struct Entrenador{
	t_list* objetivos; //especie de los pokemones
	t_list* pokemonesCazados;
	coordenadas posicion; //(x,y)
} entrenador;

//	Constructor
	entrenador entrenadorCreate(t_list* especiesObjetivos, coordenada X, coordenada Y);

//	Desplaza un entrenador de una posicion a otra
	void entrenador_ir_a(entrenador*, coordenadas);

//
	bool entrenador_puede_cazar_mas_pokemones(entrenador);

//	Pasa entrenador a estado locked
	void entrenador_bloquear(entrenador*);
	//Se me ocurre setear un booleano, para bloqueo definitivo

//****************************************************************************************
//Cola de entrenadores
typedef t_list* equipo;

//	Constructor
	equipo equipo_create();

//  Retorna el proximo entrenador a ser planificado
	entrenador* equipo_proximo_a_planificar(equipo);

//	Destructor
	void equipo_destroy(equipo);

# endif
