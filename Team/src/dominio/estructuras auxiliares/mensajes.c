#include "mensajes.h"

//mensajes entre modulos
mensaje recibir_mensaje() { //hacer que devuelva un mensaje generico de cada tipo

	mensaje unMensaje;	//TODO

	printf(
			"*****************************************\n Ingresar tipo de mensaje manualmente\n>> ");
	scanf("%u", &unMensaje.opcode);

	return unMensaje;
}

pokemon*desempaquetar_pokemon(void*empaquetado) {
	//datos del pokemon hardcodeado TODO


	return empaquetado? (pokemon*) empaquetado: NULL; //A ver si asi anda


//	pokemon* pkm = (pokemon*)malloc(sizeof(pokemon));
//	*pkm= pokemon_create("Pikachu", 3, 4);
//	return pkm;
}

resultado_captura* desempaquetar_resultado(void*empaquetado) {
	resultado_captura* resultado_HARDCODEADO = (resultado_captura*)malloc(sizeof(resultado_captura));
					 * resultado_HARDCODEADO = (resultado_captura) {1, true};
	return resultado_HARDCODEADO; //TODO
}

void get(void* especiePokemon) {
	//Envia mensaje al broker para ser replicado al gamecard
	printf("%s, ", (especie_pokemon) especiePokemon);
//	log_info(event_logger, ">> get(%s)\n", (especie_pokemon) especiePokemon);
}

void get_pokemones(especies_pokemones pokemones){

//	especies_pokemones getted = list_create();
//
//	bool is_getted(especie_pokemon unaEspecie){
//
//		bool repetido(especie_pokemon*otra){
//			return especie_cmp(unaEspecie, otra);
//		}
//
//		return list_any_satisfy(getted, (bool*) repetido);
//	}
//
//	void getSinRepetidos(especie_pokemon unaEspecie){
//		if(!is_getted(unaEspecie))
//			get(unaEspecie);
//	}

	list_iterate(pokemones, get); puts("");
	list_destroy(pokemones);
}

t_id catch(especie_pokemon especie) {
	//Envia mensaje al broker para ser replicado al gamecard, devuelve el id del mensaje pendiente por recibir
	log_info(event_logger, ">> catch(%s)\n", especie);
	return 1; //TODO
}
