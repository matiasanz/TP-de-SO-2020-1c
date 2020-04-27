#include "pokemon.h"

pokemon pokemon_create(especie_pokemon especie, int coordenadaX, int coordenadaY){
	pokemon nuevo;
	nuevo.especie = especie;
	posicion_create(&nuevo.posicion, coordenadaX, coordenadaY);
	return nuevo;
}

bool es_objetivo_de_alguien(pokemon unPokemon, entrenadores equipo){
	return true; //TODO
}

//TODO Obs: momentaneamente sin uso
especie_pokemon objetivo_siguiente(entrenador unEntrenador){
	return list_get(unEntrenador.objetivos, 0); //TODO implementar
}
