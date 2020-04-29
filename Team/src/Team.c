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
	entrenadores equipo = entrenadores_create();
	mapa unMapa = mapa_create();
	pendientes mensajesPendientes = pendientes_create();

int haya_mas_mensajes=4;
while(haya_mas_mensajes--){
	//recibo mesaje
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
				mapear_objetivo(unMapa, unPokemon); //ver por que se mapea mas de una vez

				entrenador* cazador = entrenadores_mas_cercano(equipo, unPokemon->posicion);
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

		case CAUGHT_POKEMON:{ //VER ERROR: no encuentra mensaje con id
			puts("Se recibio pokemon atrapado");
			t_id* idRespuesta = desempaquetar_id(mensajeRecibido.serializado);
			pendiente* mensajePendiente;

			mensajePendiente = pendiente_get(mensajesPendientes, *idRespuesta);

			if(mensajePendiente){
				entrenador* cazador = mensajePendiente->cazador;
				pokemon*    victima = mensajePendiente->pokemonCazado;

				mapa_desmapear(unMapa, victima);
				entrenador_capturar(cazador, victima); //TODO

//					if(entrenador_puede_cazar_mas_pokemones(*cazador)){
//						//TODO Ver si quedan muchos if anidados
//					}

				pendiente_destroy(mensajePendiente);
			}

			else{
				free(idRespuesta); //Se descarta el id
			}

			break;
		}

		default:
			error_show("Codigo de operacion desconocido");
			exit(1);
	}
}

	entrenadores_destroy(equipo);
	mapa_destroy(unMapa);
    pendientes_destroy(mensajesPendientes);

	return EXIT_SUCCESS;
}

