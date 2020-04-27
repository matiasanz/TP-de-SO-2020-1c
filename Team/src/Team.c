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

#include "pokemon.h"
#include "pendiente_de_respuesta.h"
#include "mensajes.h"

int main(void) {
	//Pantalla inicial
//	cr_saludar();
	puts("¡¡Hello World Team!!!");

	//Inicializo colas
	entrenadores equipo = entrenadores_create();
	mapa unMapa = mapa_create();
	pendientes mensajesPendientes = pendientes_create();


	//recibo mesaje
	bool indefinidamente=true;
	while(indefinidamente){
			mensaje mensajeRecibido = recibir_mensaje();

			switch(mensajeRecibido.opcode){

				case NUEVO_ENTRENADOR: {
					puts("Se recibio un nuevo entrenador");

					entrenador* unEntrenador = desempaquetar_entrenador(mensajeRecibido.serializado);
					list_iterate(unEntrenador->objetivos, &get);//Le pregunto al gamecard si cada objetivo esta en alguna posicion
					list_add(equipo, unEntrenador);				//agrego el entrenador al equipo
						puts("Se agrego un nuevo entrenador");
					break;
				}

				case LOCALIZED_POKEMON: {
					puts("Se recibio pokemon localizado");

					pokemon* unPokemon = desempaquetar_pokemon(mensajeRecibido.serializado);

					if( es_objetivo_de_alguien(*unPokemon, equipo ) ){
						mapear_objetivo(unMapa, unPokemon);

						entrenador* cazador = entrenador_mas_cerca_de(equipo, unPokemon->posicion);
						ir_a(*cazador, unPokemon->posicion);
						t_id id_mensaje_pendiente = catch(unPokemon->especie);

						agregar_pendiente(mensajesPendientes, id_mensaje_pendiente, cazador, unPokemon);

						bloquear(cazador);
					}

					else{
						puts("pokemon que no es objetivo de nadie, es cartera");
						free(unPokemon); //"descartar al pokemon"
					}

					break;
				}

				case CAUGHT_POKEMON:{
					puts("Se recibio pokemon atrapado. Proximamente");
					//TODO
					break;
				}

				default:
					error_show("Codigo de operacion desconocido");
					exit(1);
		}
	}

	entrenadores_destroy(equipo); //duda implementacion
	mapa_destroy(unMapa);
    pendientes_destroy(mensajesPendientes); //Terminar

	return EXIT_SUCCESS;
}

