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

void registrar_pokemon(pokemon*);

void team_procesar_mensajes(t_list* mensajesRecibidos) {

//	pthread_mutex_lock(&mutexFinDeProceso);
while(!FinDelProceso){
//	pthread_mutex_unlock(&mutexFinDeProceso);
	puts("Casilla: espera mensajes");

	//wait(mutex_noSeEstaEscribiendo);

	sem_wait(&sem_HayMensajesRecibidos);
	//recibo mesaje
pthread_mutex_lock(&mutexMensaje);
	mensaje* mensajeRecibido = list_remove(mensajesRecibidos, 0);//recibir_mensaje();
pthread_mutex_unlock(&mutexMensaje);

	if(!mensajeRecibido){
		error_show("Se intento leer un mensaje inexistente");
		exit(1);
	}

	switch(mensajeRecibido->opcode){

//		case LOCALIZED_POKEMON_: ; //TODO funcion tal que yo pueda iterar lista de posiciones creando pokemones y repitiendo procedimiento APPEARD
//			pokemon* unPokemon = desempaquetar_pokemon(mensajeRecibido->serializado);
//
//			log_info(logger, "LOCALIZED %s", unPokemon->especie); //Agregar lista infinita de posiciones
//
//			if(especie_recibida_con_anterioridad(unPokemon->especie, historialDePokemones)){
//				printf("%s: figurita repetida se descarta\n", unPokemon->especie);
//				pokemon_destroy(unPokemon);
//			}

//			else{
//
//
//				mensajeRecibido->opcode = APPEARD_POKEMON_;
//				list_add_in_index(mensajesRecibidos, 0, mensajeRecibido);
//			}
//
//			break;

		case APPEARD_POKEMON_: {
			pokemon* unPokemon = desempaquetar_pokemon(mensajeRecibido->serializado);

			log_info(logger, "APPEARD %s [%u] [%u]", unPokemon->especie, unPokemon->posicion.pos_x, unPokemon->posicion.pos_y); //Ver TODO si pokemon localized hace esto. Ver como saltear esta parte.

			registrar_pokemon(unPokemon); //Ver si pasar a localized

			break;
		}

		case CAUGHT_POKEMON_:{
			resultado_captura* resultado = desempaquetar_resultado(mensajeRecibido->serializado);

//			pthread_mutex_lock(&mutexCapturasPendientes);
			pendiente* capturaPendiente = pendiente_get(capturasPendientes, resultado->idCaptura);
//			pthread_mutex_unlock(&mutexCapturasPendientes);

			if(!capturaPendiente){
				error_show("Se recibio un resultado con id %u desconocido\n", resultado->idCaptura);
				break; //exit(1);
			}

			entrenador* unEntrenador = capturaPendiente->cazador;
			pokemon*pokemonCatcheado = capturaPendiente->pokemonCatcheado;

			log_info(logger, "CAUGHT %s: %s", pokemonCatcheado->especie, (resultado->tuvoExito? "Exitoso": "Fallido"));

			if(resultado->tuvoExito){

				entrenadores_bloquear_por_captura(equipo);//TODO solamente cambia un atributo. Eventualmente podrian ejecutarse simultaneamente 2 entrenadores. Tendria que guardarme al ultimo en execute y despues devolverlo
				entrenador_pasar_a(unEntrenador, EXECUTE, "Se confirmo la captura del pokemon");

				entrenador_capturar(unEntrenador, pokemonCatcheado); //TODO

				//VER TODO muchos if anidados

//				pthread_mutex_lock(&mutexEntrenador[unEntrenador->id]);
				if(entrenador_puede_cazar_mas_pokemones(*unEntrenador)){
					entrenador_pasar_a(unEntrenador, LOCKED_HASTA_APPEARD, "Tuvo exito en la captura y todavia puede cazar mas pokemones");
				}

				else if(entrenador_objetivos_cumplidos(unEntrenador)){ //abstraer a mensaje objetivos cumplidos
						entrenador_pasar_a(unEntrenador, EXIT, "Ya logro cumplir sus objetivos");
				}

				else{
					entrenador_pasar_a(unEntrenador, LOCKED_HASTA_DEADLOCK, "Su inventario esta lleno y no cumplio sus objetivos");
				}

//				pthread_mutex_unlock(&mutexEntrenador[unEntrenador->id]);
			}

			else{
				pokemon_destroy(pokemonCatcheado);
				entrenador_pasar_a(unEntrenador, LOCKED_HASTA_APPEARD, "Fallo en capturar al pokemon pero puede seguir cazando mas");
			}

			free(resultado); //Se descarta el id
			pendiente_destroy(capturaPendiente);
			break;
		}

		default:
			error_show("Se recibio un codigo de operacion desconocido: %d", mensajeRecibido->opcode);
			exit(1);
	}

//	pthread_mutex_lock(&mutexFinDeProceso);
	free(mensajeRecibido);
}

	log_info(event_logger, "Finalizo el procesamiento de mensajes");
}

//**********************************************************************************************

void registrar_pokemon(pokemon*unPokemon){

	especies_agregar(historialDePokemones, unPokemon->especie);

	if( es_objetivo_de_alguien(*unPokemon, equipo ) && !mapa_especie_mapeada(pokemonesRequeridos, unPokemon->especie)){
		//devolver 2da condicion a mapear objetivo, cambiar nombre por validado y agregarle el semaforo.
//		pthread_mutex_lock(&mutexMapaPokemones);
		mapa_mapear_objetivo(pokemonesRequeridos, unPokemon);
//		pthread_mutex_unlock(&mutexMapaPokemones);
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
