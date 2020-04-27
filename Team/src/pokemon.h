# ifndef _TAD_POKEMON_
# define _TAD_POKEMON_

#include <commons/collections/list.h>
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

//Destructor
void pokemon_destroy(pokemon*);

//******************************************************************************
//TAD MAPA: Es una lista que mapea los pokemones que van llegando del gamecard con el mensaje APPEARD
typedef t_list* mapa; //Mapa de pokemones

//Constructor
mapa mapa_create();

//me guardo la posicion y la especie, solo si es la 1era vez que aparece
void mapear_objetivo(mapa, pokemon*);

//Destructor
void mapa_destroy(mapa);

# endif
