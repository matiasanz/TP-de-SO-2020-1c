# ifndef _TAD_POKEMON_
# define _TAD_POKEMON_

#include "entrenador.h"

//TAD Pokemon									 Pasar a commons
typedef struct Pokemon{
	especie_pokemon especie; //string
	coordenadas posicion;
}pokemon;

//constructor
pokemon pokemon_create(especie_pokemon, coordenada X, coordenada Y);

//Me dice si algun miembro del equipo tiene por objetivo un pokemon de esa especie
bool es_objetivo_de_alguien(pokemon, entrenadores);

# endif
