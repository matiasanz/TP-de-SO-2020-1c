#include "mensajes.h"
#include "pokemon.h" //hardcodeado
//mensajes entre modulos
mensaje recibir_mensaje(){
	int mensajeRecibido;

	puts("ingresar tipo de mensaje manualmente");
	scanf("%d", &mensajeRecibido);

	return (mensaje){mensajeRecibido};
}

void*desempaquetar_mensaje(mensaje mensajeEmpaquetado){ //hardcodeado TODO
	int opcode = mensajeEmpaquetado.opcode;

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
	//Envia mensaje al broker para ser replicado al gamecard, devuelve el id del mensaje pendiente por recibir
	return 1; //TODO
}
