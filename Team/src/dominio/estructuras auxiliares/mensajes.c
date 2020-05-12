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
	pokemon* pkm = (pokemon*)malloc(sizeof(pokemon));
	*pkm= pokemon_create("Pikachu", 3, 4);
	return pkm;
}

resultado_captura* desempaquetar_resultado(void*empaquetado) {
	resultado_captura* resultado_HARDCODEADO = (resultado_captura*)malloc(sizeof(resultado_captura));
					 * resultado_HARDCODEADO = (resultado_captura) {1, true};
	return resultado_HARDCODEADO; //TODO
}

void get(void* especiePokemon) {
	//Envia mensaje al broker para ser replicado al gamecard
	log_info(logger, ">> get(%s)\n", (especie_pokemon) especiePokemon);
}

t_id catch(especie_pokemon especie) {
	//Envia mensaje al broker para ser replicado al gamecard, devuelve el id del mensaje pendiente por recibir
	log_info(logger, ">> catch(%s)\n", especie);
	return 1; //TODO
}
