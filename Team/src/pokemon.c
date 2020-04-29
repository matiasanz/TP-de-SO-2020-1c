#include "pokemon.h"

pokemon pokemon_create(especie_pokemon especie, coordenada coordenadaX, coordenada coordenadaY){
	pokemon nuevo;
	nuevo.especie = especie;
	posicion_create(&nuevo.posicion, coordenadaX, coordenadaY);
	return nuevo;
}

//cmp
bool pokemon_misma_especie(pokemon unPokemon, pokemon otroPokemon){
	return !strcmp(unPokemon.especie, otroPokemon.especie); //string_equals_ignore_case(objetivo->especie, deLista->especie);
}

bool es_objetivo_de_alguien(pokemon unPokemon, entrenadores equipo){
	return true; //TODO, usar cmp
}

void entrenador_capturar(entrenador*entrenador, pokemon*victima){
	/*Ver si conviene en caso de que sea un objetivo directamente eliminarlo de la lista.
	*Seria mas facil pero no se si respeta la consigna y aparte si no complica los test...*/
	list_add(entrenador->pokemonesCazados, victima->especie);
	printf("Se ha capturado a %s", victima->especie);
}

//Destructor
void pokemon_destroy(pokemon*destruido){
	free(&destruido->especie);
	free(destruido->posicion);
	free(destruido);
}

//**********************************************************************
//TAD MAPA

//Constructor
mapa mapa_create(){
	return list_create();
}



void mapear_objetivo(mapa unMapa, pokemon* objetivo){

	bool distintaEspecieQue(void* pokemon_de_lista){
		pokemon* deLista = pokemon_de_lista;
		return !pokemon_misma_especie(*objetivo, *deLista);
	}

	if(list_all_satisfy(unMapa, &distintaEspecieQue)){
		list_add(unMapa, objetivo);
		printf(" Se mapeo %s en la posicion (%d, %d)\n", objetivo->especie, objetivo->posicion[0], objetivo->posicion[1]);
	}
}

void mapa_desmapear(mapa unMapa, pokemon*unPokemon){
	bool misma_especie(pokemon* deLista){
		return pokemon_misma_especie(*unPokemon, *deLista);
	}

	list_remove_by_condition(unMapa, (bool(*)(void*))&misma_especie);
}

//Destructor
void mapa_destroy(mapa destruido){
	list_destroy_and_destroy_elements(destruido, (void(*)(void*)) &pokemon_destroy);
}
