#include <crenito-commons/mensajes/mensaje_localized_pokemon.h>
#include "mensajes.h"

//Mensajes que salen del TEAM [Tomarse el tiempo de leer comentarios]

void Get(void* especiePokemon) {
	//Envia mensaje al broker para ser replicado al gamecard
	printf("get(%s), ", (especie_pokemon) especiePokemon);
}

//Itera los objetivos, aplicando la funcion Get a cada uno. No editar esta funcion, sino la de arriba
void Get_pokemones(matriz_recursos pokemones){

	void unGetPorPokemon(char* unaEspecie, void*cantidad){
		Get(unaEspecie);
	}

	dictionary_iterator(pokemones, unGetPorPokemon); puts("");
}

t_id unIDhardcodeado=0;

t_id Catch(especie_pokemon especie) {
	//Envia mensaje al broker para ser replicado al gamecard, devuelve el id del mensaje pendiente por recibir
	log_info(event_logger, ">> catch(%s)\n", especie);
	return ++unIDhardcodeado;
}
