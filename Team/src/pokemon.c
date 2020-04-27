#include "pokemon.h"

pokemon pokemon_create(especie_pokemon especie, coordenada coordenadaX, coordenada coordenadaY){
	pokemon nuevo;
	nuevo.especie = especie;
	posicion_create(&nuevo.posicion, coordenadaX, coordenadaY);
	return nuevo;
}

bool es_objetivo_de_alguien(pokemon unPokemon, entrenadores equipo){
	return true; //TODO
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

	bool mismaEspecieQue(void* pokemon_de_lista){
		pokemon* deLista = pokemon_de_lista;
		return string_equals_ignore_case(objetivo->especie, deLista->especie);
	}

	if(list_all_satisfy(unMapa, &mismaEspecieQue)){
		list_add(unMapa, objetivo);
	}
}

//Destructor
void mapa_destroy(mapa destruido){
	list_destroy_and_destroy_elements(destruido, (void(*)(void*)) &pokemon_destroy);
}
