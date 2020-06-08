#include "mensajes.h"

void* leer_mensaje_cuando_este_disponible(cr_list* unaLista){
	return cr_list_wait_and_remove(unaLista, 0);
}

void Get(void* especiePokemon) {
	//Envia mensaje al broker para ser replicado al gamecard
	printf("get(%s), ", (especie_pokemon) especiePokemon);
//	log_info(event_logger, ">> get(%s)\n", (especie_pokemon) especiePokemon);
}

//Itera los objetivos, aplicando la funcion Get a cada uno. No editar esta funcion, sino la de arriba
void Get_pokemones(matriz_recursos pokemones){

	void unGetPorPokemon(char* unaEspecie, void*cantidad){
		Get(unaEspecie);
	}

	dictionary_iterator(pokemones, unGetPorPokemon); puts("");
}

t_id unIDhardcodeado=0;

//Envia mensaje al broker para ser replicado al gamecard, devuelve el id del mensaje pendiente por recibir
t_id Catch(especie_pokemon especie) {
	log_info(event_logger, ">> catch(%s)\n", especie);
	return ++unIDhardcodeado;
}
/*A futuro debera absorber la responsabilidad de agregar la captura pendiente y en caso de no poderse
 *conectar al broker, de autoresponderse el mensaje; ademas consume un ciclo de CPU
 */
