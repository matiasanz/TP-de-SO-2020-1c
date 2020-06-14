#include "../estructuras-principales/pokemon.h"

//TAD especie (definido en entrenador)
//cmp
bool especie_cmp(especie_pokemon unaEspecie, especie_pokemon otraEspecie){
	return !strcmp(unaEspecie, otraEspecie);
}

void especies_agregar(especies_pokemones especies, especie_pokemon especie){
	list_add(especies, especie);
}

void especies_agregar_validado(especies_pokemones especies, especie_pokemon unaEspecie){
	if(!unaEspecie){
		error_show("pokemon nulo");
		exit(1);
	}

	especies_agregar(especies, unaEspecie);
}

//TAD pokemon
pokemon pokemon_create(especie_pokemon especie, coordenada coordenadaX, coordenada coordenadaY){
	return (pokemon) {especie, posicion_create(coordenadaX, coordenadaY)};
}

pokemon* pokemon_ptr_create(especie_pokemon especie, t_posicion unaPos){
	pokemon*unPokemon = malloc(sizeof(pokemon));
		   *unPokemon = pokemon_create(especie, unaPos.pos_x, unaPos.pos_y);
    return unPokemon;
}

bool pokemon_misma_especie(pokemon unPokemon, pokemon otroPokemon){
	return especie_cmp(unPokemon.especie, otroPokemon.especie); //string_equals_ignore_case(objetivo->especie, deLista->especie);
}

//Destructor
void pokemon_destroy(pokemon*destruido){
//	free(destruido->especie);
	free(destruido);
}

//**********************************************************************
//TAD MAPA

//Constructor
mapa_pokemones mapa_create(){
	return cr_list_create();
}

void mapa_mapear_pokemon(mapa_pokemones unMapa, pokemon* unPokemon){
	cr_list_add_and_signal(unMapa, unPokemon);
}

pokemon*mapa_first(mapa_pokemones unMapa){
	return cr_list_get(unMapa, 0);
}

bool mapa_especie_mapeada(mapa_pokemones unMapa, especie_pokemon unaEspecie){
	pthread_mutex_lock(unMapa->mutex);
	bool resultado = list_contains_element(unMapa->lista, unaEspecie, (bool(*)(void*, void*))&especie_cmp);
	pthread_mutex_unlock(unMapa->mutex); puts((resultado? "especie mapeada": "especie no mapeada"));

	return resultado;
}

pokemon* mapa_desmapear(mapa_pokemones unMapa){
	return (pokemon*) cr_list_wait_and_remove(unMapa, 0);
}

//Destructor
void mapa_destroy(mapa_pokemones destruido){
	cr_list_destroy_and_destroy_elements(destruido, (void(*)(void*)) &pokemon_destroy);
}
