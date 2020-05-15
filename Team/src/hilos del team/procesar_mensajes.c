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

void procesar_APPEARD(pokemon*);

bool entrenadores_dan_seniales_de_vida(entrenadores unEquipo){
	return !list_is_empty(unEquipo);
}

void team_procesar_mensajes(t_list* mensajesRecibidos) {

while(entrenadores_dan_seniales_de_vida(equipo)){
	puts("Casilla: espera mensajes");
	sem_wait(&sem_HayMensajesRecibidos);
	//recibo mesaje
	mensaje* mensajeRecibido = list_remove(mensajesRecibidos, 0);//recibir_mensaje();

	if(!mensajeRecibido){
		error_show("Se intento leer un mensaje inexistente");
		exit(1);
	}

	switch(mensajeRecibido->opcode){

		case LOCALIZED_POKEMON_: ; //VER TODO por que no le gusta
			pokemon* unPokemon = desempaquetar_pokemon(mensajeRecibido->serializado);

			log_info(logger, "LOCALIZED %s", unPokemon->especie); //Agregar lista infinita de posiciones

			if(especie_recibida_con_anterioridad(unPokemon->especie, historialDePokemones)){
				printf("%s: figurita repetida se descarta\n", unPokemon->especie);
				pokemon_destroy(unPokemon);
				break;
			}

			mensajeRecibido->opcode = APPEARD_POKEMON_;
			list_add_in_index(mensajesRecibidos, 0, mensajeRecibido);

			/*no break*/

		case APPEARD_POKEMON_: {
			pokemon* unPokemon = desempaquetar_pokemon(mensajeRecibido->serializado);

			log_info(logger, "APPEARD %s [%u] [%u]", unPokemon->especie, unPokemon->posicion.x, unPokemon->posicion.y); //Ver TODO si pokemon localized hace esto. Ver como saltear esta parte.

			procesar_APPEARD(unPokemon); //Ver si pasar a localized

			break;
		}

		case CAUGHT_POKEMON_:{
			log_info(logger, "Se recibio mensaje CAUGHT");
			resultado_captura* resultado = desempaquetar_resultado(mensajeRecibido->serializado);

			pendiente* mensajePendiente = pendiente_get(mensajesPendientes, resultado->idCaptura);

			if(!mensajePendiente){
				error_show("Se recibio un resultado con id %u desconocido\n", resultado->idCaptura);
				break; //exit(1);
			}

			entrenador* unEntrenador = mensajePendiente->cazador;
			pokemon*pokemonCatcheado = mensajePendiente->pokemonCatcheado;

			if(resultado->tuvoExito){
				entrenador_capturar(unEntrenador, pokemonCatcheado); //TODO

				//VER TODO muchos if anidados

				if(entrenador_puede_cazar_mas_pokemones(*unEntrenador)){
					entrenador_pasar_a(unEntrenador, LOCKED_HASTA_APPEARD, "Tuvo exito en la captura y todavia puede cazar mas pokemones");
				}

				else if(list_is_empty(unEntrenador->objetivos)){ //abstraer a mensaje objetivos cumplidos
						entrenador_pasar_a(unEntrenador, EXIT, "Ya logro cumplir sus objetivos");
				}

				else{
					entrenador_pasar_a(unEntrenador, LOCKED_HASTA_DEADLOCK, "Su inventario esta lleno y no cumplio sus objetivos");
				}
			}

			else{
				pokemon_destroy(pokemonCatcheado);
				entrenador_pasar_a(unEntrenador, LOCKED_HASTA_APPEARD, "Fallo en capturar al pokemon pero puede seguir cazando mas");
			}

			free(resultado); //Se descarta el id
			pendiente_destroy(mensajePendiente);
			break;
		}

		default:
			error_show("Se recibio un codigo de operacion desconocido: %d", mensajeRecibido->opcode);
			exit(1);
	}

	free(mensajeRecibido);
}

	log_info(event_logger, "Finalizo el procesamiento de mensajes");
	estoyLeyendo=false; //Provisorio

}

//**********************************************************************************************

void procesar_APPEARD(pokemon*unPokemon){

	especies_agregar(historialDePokemones, unPokemon->especie);

	if( es_objetivo_de_alguien(*unPokemon, equipo ) && !mapa_especie_mapeada(pokemonesRequeridos, unPokemon->especie)){
		//devolver 2da condicion a mapear objetivo, cambiar nombre por validado y agregarle el semaforo.
		mapa_mapear_objetivo(pokemonesRequeridos, unPokemon);
		sem_post(&sem_HayMasPokemonesEnMapa);
		puts(">>>Signal(hay mas pokemones)");

//				puts("A continuacion se muestra es un modelo de un hilo entrenador ejecutado inmediatamente despues de recibir al pokemon");
//				MODELO_PLANIFICAR();
	}

	else{
		printf("Se recibio un %s y pokemon que no es objetivo de nadie o que ya tengo en mapa, es cartera\n", unPokemon->especie);
		pokemon_destroy(unPokemon); //"descartar al pokemon"
	}
}
