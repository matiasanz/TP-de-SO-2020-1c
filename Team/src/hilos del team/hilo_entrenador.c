#include "hilos_team.h"
#include "../team.h"

/*TODO
 * corregir implementacion de capturar pokemon, considerar que cada llamado a objetivos globales pasa a ser region critica.
 * Puede pasar eventualmente que un entrenador necesite capturar un pokemon y se ejecute a la par que otro. Duda: Puedo pausar un hilo?
 */

void team_hilo_entrenador(entrenador*unEntrenador){
	printf("Entrenador N°%u en espera\n", unEntrenador->id);
	bool hiloActivo = true;
	t_id pid = unEntrenador->id;
	while(hiloActivo){

//		pthread_mutex_lock(&mutexEntrenador[pid]);
		t_estado estadoActual = unEntrenador->estado;
//		pthread_mutex_unlock(&mutexEntrenador[pid]);

		switch(estadoActual){ //Se tiene que ir

			case READY: {
				sem_wait(&sem_Entrenador[pid]);
				entrenador_pasar_a(unEntrenador, EXECUTE, "El planificador ha hablado");

//				pthread_mutex_lock(&mutexMapaPokemones);
				pokemon*unPokemon = mapa_desmapear(pokemonesRequeridos);
				sem_post(&sem_PokemonRemovido);
//				pthread_mutex_unlock(&mutexMapaPokemones);

//				pthread_mutex_lock(&mutexEntrenador[pid]);
				entrenador_ir_a(unEntrenador, unPokemon->posicion);

				if(entrenador_llego_a(*unEntrenador, unPokemon->posicion)){
//					pthread_mutex_unlock(&mutexEntrenador[pid]);
					t_id id_mensaje_pendiente = Catch(unPokemon->especie);
					//Agregar semaforo
//					pthread_mutex_lock(&mutexCapturasPendientes);
					agregar_pendiente(capturasPendientes, id_mensaje_pendiente, unEntrenador, unPokemon);
//					pthread_mutex_unlock(&mutexCapturasPendientes);
					entrenador_pasar_a(unEntrenador, LOCKED_HASTA_CAUGHT, "Tiene una captura pendiente");
				}

				else{
					entrenador_bloquear_hasta_APPEARD(unEntrenador);
//					pthread_mutex_unlock(&mutexEntrenador[pid]);
//					pthread_mutex_lock(&mutexMapaPokemones);
					mapa_mapear_objetivo(pokemonesRequeridos, unPokemon);
//					pthread_mutex_unlock(&mutexMapaPokemones);
				}

				break;
			}

			case LOCKED_HASTA_DEADLOCK: {
				//TODO
				entrenador_pasar_a(unEntrenador, EXIT, "Su inventario esta lleno pero no logro cumplir sus objetivos y aun no existe algoritmo de deadlock");

				break;
			}

			case  EXIT:{
				printf("Se libero al entrenador N°%u\n", pid);
				entrenadores_remover_del_equipo_a(equipo, pid);
				hiloActivo=false;

				break;
			}

			default: ;
				break;

		}
	}

	log_info(event_logger, "Finalizo un hilo entrenador");
}

pthread_t* inicializar_hilos_entrenadores(int*cantidadEntrenadores){

	int i, cantidadDeHilos = list_size(equipo);
	pthread_t* hilosEntrenadores = malloc(sizeof(pthread_t)*cantidadDeHilos);

	sem_Entrenador = malloc(sizeof(sem_t)*cantidadDeHilos);
//	mutexEntrenador = malloc(sizeof(pthread_mutex_t)*cantidadDeHilos);

	for(i=0; i<cantidadDeHilos; i++){
		sem_init(&sem_Entrenador[i], 0, 0);
//		pthread_mutex_init(&mutexEntrenador[i], NULL);
		pthread_create(&hilosEntrenadores[i], NULL, (void*) team_hilo_entrenador, list_get(equipo, i));
	}

	*cantidadEntrenadores = cantidadDeHilos;
	return hilosEntrenadores;
}

/***************************A futuro****/
//				entrenador_pasar_a(unEntrenador, EXECUTE, "Es su turno de ejecutar");
//				switch(unEntrenador->objetivoActual){
//					case CATCHEAR: {
//		//				pthread_mutex_lock(&mutexMapaPokemones);
//						pokemon*unPokemon = mapa_desmapear(pokemonesRequeridos);
//						sem_post(&sem_PokemonRemovido);
//		//				pthread_mutex_unlock(&mutexMapaPokemones);
//
//		//				pthread_mutex_lock(&mutexEntrenador[pid]);
//						entrenador_ir_a(unEntrenador, unPokemon->posicion);
//
//						if(entrenador_llego_a(*unEntrenador, unPokemon->posicion)){
//		//					pthread_mutex_unlock(&mutexEntrenador[pid]);
//							t_id id_mensaje_pendiente = Catch(unPokemon->especie);
//							//Agregar semaforo
//		//					pthread_mutex_lock(&mutexCapturasPendientes);
//							agregar_pendiente(capturasPendientes, id_mensaje_pendiente, unEntrenador, unPokemon);
//		//					pthread_mutex_unlock(&mutexCapturasPendientes);
//							entrenador_pasar_a(unEntrenador, LOCKED_HASTA_CAUGHT, "Tiene una captura pendiente");
//						}
//
//						else{
//							entrenador_bloquear_hasta_APPEARD(unEntrenador);
//		//					pthread_mutex_unlock(&mutexEntrenador[pid]);
//		//					pthread_mutex_lock(&mutexMapaPokemones);
//							mapa_mapear_objetivo(pokemonesRequeridos, unPokemon);
//		//					pthread_mutex_unlock(&mutexMapaPokemones);
//						}
//
//						break;
//					}
//
//					/*no default*/
//
////					case CAPTURAR: {
////						wait(puedeCapturar[id]);
//			//			pthread_mutex_lock(&mutexCapturasPendientes);
//// VER como hacer						pendiente* capturaPendiente = pendiente_get(capturasPendientes, resultado->idCaptura);
//			//			pthread_mutex_unlock(&mutexCapturasPendientes);
//
////						if(!capturaPendiente){
////							error_show("Se recibio un resultado con id %u desconocido\n", resultado->idCaptura);
////							break; //exit(1);
////						}
////
////						entrenador* unEntrenador = capturaPendiente->cazador;
////						pokemon*pokemonCatcheado = capturaPendiente->pokemonCatcheado;
////
////						log_info(logger, "CAUGHT %s: %s", pokemonCatcheado->especie, (resultado->tuvoExito? "Exitoso": "Fallido"));
////
////						if(resultado->tuvoExito){
////
////							entrenadores_bloquear_por_captura(equipo);//TODO solamente cambia un atributo. Eventualmente podrian ejecutarse simultaneamente 2 entrenadores. Tendria que guardarme al ultimo en execute y despues devolverlo
////							entrenador_pasar_a(unEntrenador, EXECUTE, "Se confirmo la captura del pokemon");
////
////							entrenador_capturar(unEntrenador, pokemonCatcheado); //TODO
////
////							//VER TODO muchos if anidados
////
////			//				pthread_mutex_lock(&mutexEntrenador[unEntrenador->id]);
////							if(entrenador_puede_cazar_mas_pokemones(*unEntrenador)){
////								entrenador_pasar_a(unEntrenador, LOCKED_HASTA_APPEARD, "Tuvo exito en la captura y todavia puede cazar mas pokemones");
////							}
////
////							else if(entrenador_objetivos_cumplidos(unEntrenador)){ //abstraer a mensaje objetivos cumplidos
////									entrenador_pasar_a(unEntrenador, EXIT, "Ya logro cumplir sus objetivos");
////							}
////
////							else{
////								entrenador_pasar_a(unEntrenador, LOCKED_HASTA_DEADLOCK, "Su inventario esta lleno y no cumplio sus objetivos");
////							}
////
////			//				pthread_mutex_unlock(&mutexEntrenador[unEntrenador->id]);
////						}
////
////						else{
////							pokemon_destroy(pokemonCatcheado);
////							entrenador_pasar_a(unEntrenador, LOCKED_HASTA_APPEARD, "Fallo en capturar al pokemon pero puede seguir cazando mas");
////						}
////
////						free(resultado); //Se descarta el id
////						pendiente_destroy(capturaPendiente);
////						break;
////					}
////
////					case DEADLOCK: {
////						puts("falta algoritmo");
////					}
////
//					default:
//						break;
//				}

