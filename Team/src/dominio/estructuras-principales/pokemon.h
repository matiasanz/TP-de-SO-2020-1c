# ifndef _TAD_POKEMON_
# define _TAD_POKEMON_

//#include <commons/collections/list.h>
#include "../estructuras-principales/entrenador.h"
typedef t_list* especies_pokemones;

////TAD Pokemon									 Pasar a commons
//typedef struct Pokemon{
//	especie_pokemon especie; //string
//	t_posicion posicion;
//}pokemon;

//constructor
pokemon pokemon_create(especie_pokemon, coordenada X, coordenada Y);
pokemon* pokemon_ptr_create(especie_pokemon, t_posicion);

//Destructor
//void pokemon_destroy(pokemon*);

//Destruye el pokemon y la especie
void pokemon_destroy_hard(pokemon*);

//  Captura un pokemon - ver donde lo puedo poner; problema de ponerlo en entrenador: entrenador no reconoce pokemon y poke incluye entrenador
	void entrenador_capturar(entrenador*entrenador, pokemon*victima);

	bool pokemon_misma_especie_que(pokemon*, pokemon*);
//******************************************************************************
//TAD MAPA: Es una lista que mapea los pokemones que van llegando del gamecard con el mensaje APPEARD
typedef cr_list* mapa_pokemones; //Mapa de pokemones

//Constructor
mapa_pokemones mapa_create();

//me guardo la posicion y la especie, solo si es la 1era vez que aparece
void mapa_mapear_pokemon(mapa_pokemones, pokemon*);

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
