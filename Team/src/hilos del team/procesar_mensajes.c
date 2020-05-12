/*
 ============================================================================
 Name        : Team.c
 Author      : C-renito Casero
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "hilos_team.h"
#include "../team.h"

void team_procesar_mensajes() {
	//Pantalla inicial
	puts("¡¡Hello World Team!!!");

int haya_mas_mensajes=10;
while(haya_mas_mensajes--){
	//recibo mesaje
	mensaje mensajeRecibido = recibir_mensaje();

	switch(mensajeRecibido.opcode){

		case LOCALIZED_POKEMON_: ;
			pokemon* unPokemon = desempaquetar_pokemon(mensajeRecibido.serializado);

			if(especie_recibida_con_anterioridad(unPokemon->especie, historialDePokemones)){
				puts("figurita repetida se descarta");
				pokemon_destroy(unPokemon);
				break;
			}

			//mensaje.contenido = empaquetar_pokemon(unPokemon); //volver a empaquetar pokemon para que appeard

			/* no break */

		case APPEARD_POKEMON_: {
//			log_info(logger, "Se recibio pokemon localizado");

			pokemon* unPokemon = desempaquetar_pokemon(mensajeRecibido.serializado);
			especies_agregar(historialDePokemones, unPokemon->especie);

			if( es_objetivo_de_alguien(*unPokemon, equipo ) ){

				mapa_mapear_objetivo(pokemonesRequeridos, unPokemon);

				puts("A continuacion se muestra es un modelo de planificar() ejecutado inmediatamente despues de recibir al pokemon");
				MODELO_PLANIFICAR();
//
//					unPokemon = mapa_desmapear(mapaPokemones);
//					entrenador* cazador = entrenadores_mas_cercano(equipo, unPokemon->posicion);
//					entrenador_ir_a(cazador, unPokemon->posicion);
//					t_id id_mensaje_pendiente = catch(unPokemon->especie);
//
//					agregar_pendiente(mensajesPendientes, id_mensaje_pendiente, cazador, unPokemon);
//
//					entrenador_bloquear(cazador);// pasa a LOCKED
			}

			else{
				puts("pokemon que no es objetivo de nadie, es cartera");
				pokemon_destroy(unPokemon); //"descartar al pokemon"
			}

			break;
		}

		case CAUGHT_POKEMON_:{
//			log_info(logger, "Se recibio pokemon atrapado");
			resultado_captura* resultado = desempaquetar_resultado(mensajeRecibido.serializado);

			pendiente* mensajePendiente = pendiente_get(mensajesPendientes, resultado->idCaptura);

			if(!mensajePendiente){
				error_show("Se recibio un resultado con id desconocido");
				exit(1);
			}

			entrenador* unEntrenador = mensajePendiente->cazador;
			pokemon*pokemonCatcheado = mensajePendiente->pokemonCatcheado;

			if(resultado->tuvoExito){
				entrenador_capturar(unEntrenador, pokemonCatcheado); //TODO

				//VER TODO muchos if anidados

				if(entrenador_puede_cazar_mas_pokemones(*unEntrenador)){
					entrenador_bloquear_hasta_APPEARD(unEntrenador);
				}

				else if(list_is_empty(unEntrenador->objetivos)){ //abstraer a mensaje objetivos cumplidos
						entrenador_destroy(unEntrenador);//	entrenador_pasar_a_estado(EXIT);
				}

				else{
					entrenador_bloquear_hasta_DEADLOCK(unEntrenador);
				}
			}

			else{
				pokemon_destroy(pokemonCatcheado);
				entrenador_bloquear_hasta_APPEARD(unEntrenador);
			}

			free(resultado); //Se descarta el id
			pendiente_destroy(mensajePendiente);
			break;
		}

		default:
			error_show("Se recibio un codigo de operacion desconocido");
			team_exit();
	}
}

}
