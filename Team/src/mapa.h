# ifndef _MAPA_
# define _MAPA_
//Es una lista que mapea los pokemones que van llegando del gamecard con el mensaje APPEARD

#include <commons/collections/list.h>
#include "pokemon.h"

typedef t_list* mapa;

//Constructor
mapa mapa_create();

//me guardo la posicion y la especie, solo si es la 1era vez que aparece
void mapear_objetivo(mapa unMapa, pokemon unPokemon);


# endif
