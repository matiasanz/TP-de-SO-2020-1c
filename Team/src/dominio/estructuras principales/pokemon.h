# ifndef _TAD_POKEMON_
# define _TAD_POKEMON_

//#include <commons/collections/list.h>
#include "entrenador.h"

//TAD Pokemon									 Pasar a commons
typedef struct Pokemon{
	especie_pokemon especie; //string
	t_posicion posicion;
}pokemon;

//constructor
pokemon pokemon_create(especie_pokemon, coordenada X, coordenada Y);
pokemon* pokemon_ptr_create(especie_pokemon, coordenada x, coordenada y);

//Me dice si algun miembro del equipo tiene por objetivo un pokemon de esa especie
bool pokemon_es_objetivo(pokemon, matriz_recursos);

//Destructor
void pokemon_destroy(pokemon*);

//  Captura un pokemon - ver donde lo puedo poner; problema de ponerlo en entrenador: entrenador no reconoce pokemon y poke incluye entrenador
	void entrenador_capturar(entrenador*entrenador, pokemon*victima);


//******************************************************************************
//TAD MAPA: Es una lista que mapea los pokemones que van llegando del gamecard con el mensaje APPEARD
typedef cr_list* mapa_pokemones; //Mapa de pokemones

//Constructor
mapa_pokemones mapa_create();

//me guardo la posicion y la especie, solo si es la 1era vez que aparece
void mapa_mapear_objetivo(mapa_pokemones, pokemon*);

//Retorna true si la especie esta contenida en el mapa
bool mapa_especie_mapeada(mapa_pokemones, especie_pokemon);

//Retorna un puntero al 1er pokemon de la lista
pokemon* mapa_first(mapa_pokemones);

//Retorna el primer pokemon en el mapa y lo remueve de la lista
pokemon* mapa_desmapear(mapa_pokemones);

//Destructor
void mapa_destroy(mapa_pokemones);

//************************************************************
//TAD Especie
bool especie_recibida_con_anterioridad(especie_pokemon, especies_pokemones historial);
void especies_agregar(especies_pokemones, especie_pokemon);
void especies_agregar_validado(especies_pokemones, especie_pokemon);
bool especie_cmp(especie_pokemon, especie_pokemon);

# endif
