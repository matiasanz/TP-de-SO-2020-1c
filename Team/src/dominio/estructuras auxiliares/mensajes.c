#include "mensajes.h"

numero idGet=0;

mensaje_localized_pokemon* localized_pokemon_crear(especie_pokemon especie){
    mensaje_localized_pokemon* localized_pokemon = malloc(sizeof(mensaje_localized_pokemon));

	localized_pokemon->especie_lenght = strlen(especie) + 1;
	localized_pokemon->especie = especie;
	localized_pokemon->posiciones = list_create();

	t_posicion*unaPos = malloc(sizeof(t_posicion));
	  		  *unaPos = posicion_create(2,7);
	list_add(localized_pokemon->posiciones, unaPos);

			   unaPos = malloc(sizeof(t_posicion));
	  		  *unaPos = posicion_create(3,5);
	list_add(localized_pokemon->posiciones, unaPos);

	 	 	   unaPos = malloc(sizeof(t_posicion));
	  		  *unaPos = posicion_create(3,6);
	list_add(localized_pokemon->posiciones, unaPos);

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

void Get_pokemones(especies_pokemones pokemones){
	list_iterate(pokemones, Get); puts("");
	list_destroy(pokemones);
}

t_id unIDhardcodeado=0;

t_id Catch(especie_pokemon especie) {
	//Envia mensaje al broker para ser replicado al gamecard, devuelve el id del mensaje pendiente por recibir
	log_info(event_logger, ">> catch(%s)\n", especie);
	return ++unIDhardcodeado;
}
