/*
 ============================================================================
 Name        : Team.c
 Author      : 
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/*TODO implementar las funciones no definidas
 *TODO pasar los archivos que no tienen que ver con programa principal a carpeta "biblioteca", lo cual implica modificar makefile
 *TODO ver que archivos pasar a commons
 */

#include "mapa.h"
#include "pendiente_de_respuesta.h"
#include "mensajes.h"

int main(void) {
	//Pantalla inicial
	cr_saludar();
	puts("¡¡Hello World Team!!!");

	//Inicializo colas
	entrenadores equipo = entrenadores_create();
	mapa unMapa = mapa_create();
	mensajes_pendientes mensajesPendientes = mensajes_pendientes_create();


	//recibo mesajes
	mensaje mensajeRecibido = recibir_mensaje();

	switch(mensajeRecibido.opcode){

		case NUEVO_ENTRENADOR: {
			entrenador* unEntrenador = desempaquetar_mensaje(mensajeRecibido);
			list_iterate(unEntrenador->objetivos, &get);//Le pregunto al gamecard si cada objetivo esta en alguna posicion
			list_add(equipo, unEntrenador);				//si no funca, cambiar por while(objetivo!=null) get(objetivo); ...;
			break;
		}

		case LOCALIZED_POKEMON: {
			pokemon* unPokemon = desempaquetar_mensaje(mensajeRecibido);
			if( es_objetivo_de_alguien(*unPokemon, equipo) ){
				mapear_objetivo(unMapa, unPokemon);

				entrenador* unEntrenador = entrenador_mas_cerca_de(equipo, unPokemon->posicion);
				ir_a(*unEntrenador, unPokemon->posicion);
				int id_mensaje_pendiente = catch(unPokemon->especie);

				agregar_mensaje_pendiente(mensajesPendientes, id_mensaje_pendiente, unEntrenador, unPokemon);

				bloquear(unEntrenador);
			}

	//		else{
	//			//descartar(mensaje); //free entrenador
	//			//descartar(pokemon); //free pokemon
	//		}

			break;
		}

		case CAUGHT:{
			//TODO
		}
	}

	//FALTAN LOS DESTROOOOOY

	return EXIT_SUCCESS;
}

