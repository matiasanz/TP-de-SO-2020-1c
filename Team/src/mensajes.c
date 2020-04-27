#include "mensajes.h"
#include "pokemon.h" //hardcodeado
#include <commons/error.h>
//mensajes entre modulos
mensaje recibir_mensaje(){ //hacer que devuelva un mensaje generico de cada tipo

	mensaje unMensaje;	//TODO
	puts("ingresar tipo de mensaje manualmente");
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
	especie_pokemon especie = "pucho";
	coordenada posX = 3;
	coordenada posY = 4;
	//me los va a dar el mensaje cuando desserialice

	pokemon *nuevoPokemon = malloc(sizeof(pokemon));
			*nuevoPokemon = pokemon_create(especie, posX, posY);
	return nuevoPokemon;

}

void get(void* especiePokemon){
	//Envia mensaje al broker para ser replicado al gamecard
}

t_id catch(especie_pokemon especie){
	//Envia mensaje al broker para ser replicado al gamecard, devuelve el id del mensaje pendiente por recibir
	return 1; //TODO
}
