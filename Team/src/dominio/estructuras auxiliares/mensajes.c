#include "mensajes.h"

pokemon*desempaquetar_pokemon(void*empaquetado) {
	return (pokemon*) empaquetado;
}

void* desempaquetar_resultado(void*empaquetado) {
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
