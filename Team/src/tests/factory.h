#ifndef _FACTORY_
#define _FACTORY_

#include "../dominio/estructuras principales/pokemon.h"
#include "../dominio/estructuras auxiliares/mensajes.h"

//constructor simplificado de entrenador
entrenador entrenador_CREATE(t_list* especiesObjetivos, coordenada X, coordenada Y);

//entrenador con 1 objetivo "pucho"
entrenador*factory_entrenador_de_prueba();

//hacer entrenador con n objetivos, que reciba las especies

//pucho en (3,6)
pokemon*factory_pokemon_de_prueba();

//Crenito Jr en (x,y)
pokemon*factory_pokemon_de_prueba_en(coordenada x, coordenada y);

//Retorna una lista de entrenadores con objetivo "cantidad" en (cantidad, cantidad+1)
entrenadores factory_entrenadores_de_prueba(int cantidad);

char*itoa(int i);

# endif
