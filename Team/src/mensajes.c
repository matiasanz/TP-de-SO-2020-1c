#include "mensajes.h"
#include "pokemon.h" //hardcodeado
#include <commons/error.h>

//mensajes entre modulos
mensaje recibir_mensaje() { //hacer que devuelva un mensaje generico de cada tipo

	mensaje unMensaje;	//TODO

	printf(
			"*****************************************\n Ingresar tipo de mensaje manualmente\n>> ");
	scanf("%u", &unMensaje.opcode);

	return unMensaje;
}

entrenador*entrenador_de_prueba() {
	//datos del entrenador hardcodeado TODO
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

	pokemon* pkm = malloc(sizeof(pokemon));
	*pkm= pokemon_create("pucho,", 3, 4);

	return pkm;



}

t_id* desempaquetar_id(void*empaquetado) {
	t_id *i = malloc(sizeof(t_id));
	*i = 1;
	return i; //TODO
}

void get(void* especiePokemon) {
	//Envia mensaje al broker para ser replicado al gamecard
	printf(" get(%s)\n", (especie_pokemon) especiePokemon);
}

t_id catch(especie_pokemon especie) {
	//Envia mensaje al broker para ser replicado al gamecard, devuelve el id del mensaje pendiente por recibir
	printf(" catch(%s)\n", especie);
	return 1; //TODO
}
