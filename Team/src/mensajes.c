#include "mensajes.h"
#include "pokemon.h" //hardcodeado
#include <commons/error.h>
//mensajes entre modulos
mensaje recibir_mensaje(){ //hacer que devuelva un mensaje generico de cada tipo

	mensaje unMensaje;	//TODO

	puts("*****************************************\n>>Ingresar tipo de mensaje manualmente");
	scanf("%u", &unMensaje.opcode);

	return unMensaje;
}

entrenador*desempaquetar_entrenador(void*empaquetado){
	//datos del entrenador hardcodeado TODO DEFINIR ESTRUCTURAS SERIALIZADAS
		t_list*objetivos = list_create();
		especie_pokemon especie = "pucho";
		list_add(objetivos, especie);
		coordenada posX = 1;
		coordenada posY = 2;

		entrenador*nuevoEntrenador = malloc(sizeof(entrenador));
				  *nuevoEntrenador = entrenadorCreate(objetivos, posX, posY);
		return nuevoEntrenador;
}

pokemon*desempaquetar_pokemon(void*empaquetado) {
	//datos del pokemon hardcodeado TODO DEFINIR ESTRUCTURAS SERIALIZADAS
	t_mensaje_new_pokemon empaquetadoTRUCHO = (t_mensaje_new_pokemon){"pucho", (uint32_t)strlen("pucho"), 3, 4};

	pokemon *nuevoPokemon = malloc(sizeof(pokemon));
			*nuevoPokemon = pokemon_create(string_substring_until(empaquetadoTRUCHO.especie, empaquetadoTRUCHO.especieLength), empaquetadoTRUCHO.posX, empaquetadoTRUCHO.posY);

	return nuevoPokemon;

}

void get(void* especiePokemon){
	//Envia mensaje al broker para ser replicado al gamecard
	printf(">> get(%s)\n", (especie_pokemon)especiePokemon);
}

t_id catch(especie_pokemon especie){
	//Envia mensaje al broker para ser replicado al gamecard, devuelve el id del mensaje pendiente por recibir
	printf(">> catch(%s)\n", especie);
	return 1; //TODO
}
