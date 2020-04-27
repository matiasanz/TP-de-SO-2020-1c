/*
 ============================================================================
 Name        : Team.c
 Author      : 
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//#include "auxiliares.h"
#include "entrenador.h"

typedef t_list* mapa;
typedef t_list* mensajes_pendientes;
typedef int mensaje;

//TAD Pokemon; obs: tanto team como gamecard interactuan con objeto pokemon; va commons?
typedef struct Pokemon{
	especie_pokemon especie; //string
	int* posicion;
}pokemon;

#define NUEVO_ENTRENADOR  0
#define LOCALIZED_POKEMON 1
#define CAUGHT 			  2

//constructores
mapa mapa_create();
mensajes_pendientes mensajes_pendientes_create();

//acciones de las estructuras
//	mapa
void mapear_objetivo(mapa unMapa, pokemon unPokemon); //me guardo la posicion y la especie, solo si es la 1era vez que aparece.

//  entrenador
bool es_objetivo_de_alguien(pokemon, entrenadores);
void bloquear(entrenador*);

//  mensaje_pendiente
void agregar_mensaje_pendiente(mensajes_pendientes, int id, entrenador*, pokemon*);

//mensajes entre modulos
mensaje recibir_mensaje();
void*desempaquetar_mensaje(mensaje);
void get(void* especiePokemon);
int catch(especie_pokemon); //ver si no seria la posicion

//Programa principal
int main(void) {
//	cr_saludar();

	char* string = string_new();
	string_append(&string, "!!Hello World Team!!!");
	puts(string); /* prints !!!Hello World!!! */

	//**************************************************
	entrenadores equipo = entrenadores_create();
	mapa unMapa = mapa_create();
	mensajes_pendientes mensajesPendientes = mensajes_pendientes_create();

	mensaje mensajeRecibido = recibir_mensaje();

	switch(mensajeRecibido){

	case NUEVO_ENTRENADOR: ;
		entrenador* unEntrenador = desempaquetar_mensaje(mensajeRecibido);
		list_iterate(unEntrenador->objetivos, &get);//Le pregunto al gamecard si cada objetivo esta en alguna posicion
		list_add(equipo, unEntrenador);				//si no funca, cambiar por while(objetivo!=null) get(objetivo); ...;

		break;

	case LOCALIZED_POKEMON: ;
		pokemon* unPokemon = desempaquetar_mensaje(mensajeRecibido);
		if( es_objetivo_de_alguien(*unPokemon, equipo) ){
			mapear_objetivo(unMapa, *unPokemon);

			entrenador* unEntrenador = entrenador_mas_cerca_de(equipo, unPokemon->posicion);
			ir_a(*unEntrenador, unPokemon->posicion);
			int id_mensaje_pendiente = catch(unPokemon->especie);

			agregar_mensaje_pendiente(mensajesPendientes, id_mensaje_pendiente, unEntrenador, unPokemon);

			bloquear(unEntrenador);
		}
//
//		else{
//			//descartar(mensaje);
//			//descartar(pokemon); //free pokemon
//		}

		break;

	}

	return EXIT_SUCCESS;
}

//****************************************************************** fin main



//TAD Pokemon
	pokemon pokemon_create(especie_pokemon especie, int coordenadaX, int coordenadaY){
		pokemon nuevo;
		nuevo.especie = especie;
		posicion_create(&nuevo.posicion, coordenadaX, coordenadaY);
		return nuevo;
	}

	bool es_objetivo_de_alguien(pokemon unPokemon, entrenadores equipo){
		return true; //TODO
	}

	especie_pokemon objetivo_siguiente(entrenador unEntrenador){ //TODO VERR donde usar en nueva implementacion
		return list_get(unEntrenador.objetivos, 0);
	}


//  mensaje_pendiente
void agregar_mensaje_pendiente(mensajes_pendientes mensajesPendientes, int id_mensaje_pendiente, entrenador* unEntrenador, pokemon* unPokemon){
	//TODO
}

//constructores de listas


mapa mapa_create(){
	return list_create();
}

mensajes_pendientes mensajes_pendientes_create(){
	return list_create();
}

//acciones de las estructuras
//	mapa
void mapear_objetivo(mapa unMapa, pokemon unPokemon){ //me guardo la posicion y la especie, solo si es la 1era vez que aparece.
	//TODO
}

//mensajes entre modulos
mensaje recibir_mensaje(){
	int mensajeRecibido;
	puts("ingresar mensaje manualmente");
	scanf("%d", &mensajeRecibido);
	return mensajeRecibido;
}

void*desempaquetar_mensaje(mensaje mensaje){
	int opcode = mensaje;

	switch(opcode){
		case NUEVO_ENTRENADOR: {
			//datos del entrenador
			t_list*objetivos = list_create();
			especie_pokemon especie = "pucho";
			list_add(objetivos, especie);
			coordenada posX = 1;
			coordenada posY = 2;
			//me los va a dar el mensaje cuando desserialice

			entrenador*nuevoEntrenador = malloc(sizeof(entrenador));
					  *nuevoEntrenador = entrenadorCreate(objetivos, posX, posY);
			return nuevoEntrenador;
			break;
		}

		case LOCALIZED_POKEMON: {
			//datos del pokemon
			especie_pokemon especie = "pucho";
			coordenada posX = 3;
			coordenada posY = 4;
			//me los va a dar el mensaje cuando desserialice

			pokemon *nuevoPokemon = malloc(sizeof(pokemon));
					*nuevoPokemon = pokemon_create(especie, posX, posY);
			return nuevoPokemon;
		}
	}

	error_show("id mensaje desconocido");

	return NULL;
}

void get(void* especiePokemon){
	//Envia mensaje al broker para ser replicado al gamecard
}

int catch(especie_pokemon especie){
	//Envia mensaje al broker para ser replicado al gamecard
	return 1;
}


