#include "mensajes.h"

numero idGet=0;

t_posicion*posicion_ptr_create(coordenada x, coordenada y){
	t_posicion*unaPos = malloc(sizeof(t_posicion));
		      *unaPos = posicion_create(x,y);

    return unaPos;
}

mensaje_localized_pokemon* localized_pokemon_crear(especie_pokemon especie){
    mensaje_localized_pokemon* localized_pokemon = malloc(sizeof(mensaje_localized_pokemon));

	localized_pokemon->especie = especie;
	localized_pokemon->posiciones = list_create();


	list_add(localized_pokemon->posiciones, posicion_ptr_create(2, 7));

	list_add(localized_pokemon->posiciones, posicion_ptr_create(8, 1));

	list_add(localized_pokemon->posiciones, posicion_ptr_create(6, 2));

	localized_pokemon->posiciones_lenght = list_size(localized_pokemon->posiciones);
	return localized_pokemon;
}

pokemon*desempaquetar_pokemon(void*empaquetado) {
	return (pokemon*) empaquetado;
}

void* desempaquetar_resultado(void*empaquetado) {
	return empaquetado;
}

mensaje_localized_pokemon* desempaquetar_localized(void*empaquetado){
	return empaquetado;
}

void Get(void* especiePokemon) {
	//Envia mensaje al broker para ser replicado al gamecard
	printf("get(%s), ", (especie_pokemon) especiePokemon);
//	log_info(event_logger, ">> get(%s)\n", (especie_pokemon) especiePokemon);
}

void Get_pokemones(matriz_recursos pokemones){

	puts("hola");
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
