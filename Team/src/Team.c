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
#include "mensajes.h"
#include "respuesta_pendiente.h"

int main(void) {
	//Pantalla inicial
//	cr_saludar();
	puts("¡¡Hello World Team!!!");

	//Inicializo colas
	equipo entrenadores_ready = equipo_create();
	mapa unMapa = mapa_create();
	pendientes mensajesPendientes = pendientes_create();

int haya_mas_mensajes=4;
while(haya_mas_mensajes--){
	//recibo mesaje
	mensaje mensajeRecibido = recibir_mensaje();

	switch(mensajeRecibido.opcode){

		case LOCALIZED_POKEMON_: {
			puts("Se recibio pokemon localizado");

			pokemon* unPokemon = desempaquetar_pokemon(mensajeRecibido.serializado);

			if( es_objetivo_de_alguien(*unPokemon, entrenadores_ready ) ){
				//TODO definir bien esObjDeAlguien

				mapear_objetivo(unMapa, unPokemon);

				//(...) Ver TODO, esto se hace durante planificacion de entrenador

				entrenador* cazador = equipo_mas_cercano(entrenadores_ready, unPokemon->posicion); //(lista, CRITERIO)? ver si vale la pena, con enum
				entrenador_ir_a(cazador, unPokemon->posicion);
				t_id id_mensaje_pendiente = catch(unPokemon->especie);

				agregar_pendiente(mensajesPendientes, id_mensaje_pendiente, cazador, unPokemon);

				entrenador_bloquear(cazador);//TODO
			}

			else{
				puts("pokemon que no es objetivo de nadie, es cartera");
				pokemon_destroy(unPokemon); //"descartar al pokemon"
			}

			break;
		}

		case CAUGHT_POKEMON_:{
			puts("Se recibio pokemon atrapado");
			t_id* idRespuesta = desempaquetar_id(mensajeRecibido.serializado);

			pendiente* mensajePendiente = pendiente_get(mensajesPendientes, *idRespuesta);

			if(!mensajePendiente){
				error_show("Se recibio un id mensaje desconocido");
				exit(1);
			}

			entrenador* cazador = mensajePendiente->cazador;
			pokemon*    victima = mensajePendiente->pokemonCazado;

			mapa_desmapear(unMapa, victima);
			entrenador_capturar(cazador, victima); //TODO

			if(entrenador_puede_cazar_mas_pokemones(*cazador)){
				list_add(entrenadores_ready, cazador);
			}

//			else if(){
//				//TODO Ver diagrama si quedan muchos if
//			}

			pendiente_destroy(mensajePendiente);
			free(idRespuesta); //Se descarta el id

			break;
		}

		case MISSED: {
			puts("Se recibio pokemon perdido. PROXIMAMENTE");
			//TODO LEER enunciado

			//eliminar el pendiente? (tener en cuenta que se repitiria codigo del anterior)

			break;
		}

		default:
			error_show("Se recibio un codigo de operacion desconocido");
			exit(1);
	}
}

	equipo_destroy(entrenadores_ready);
	mapa_destroy(unMapa);
    pendientes_destroy(mensajesPendientes);

	return EXIT_SUCCESS;
}

