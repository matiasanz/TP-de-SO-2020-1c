#include "hilos_team.h"
#include "../team.h"

/*TODO
 * corregir implementacion de capturar pokemon, considerar que cada llamado a objetivos globales pasa a ser region critica.
 * Puede pasar eventualmente que un entrenador necesite capturar un pokemon y se ejecute a la par que otro. Duda: Puedo pausar un hilo?
 */

void team_hilo_entrenador(entrenador*unEntrenador){
	printf("Entranador N°%u en espera\n", unEntrenador->id);
	bool hiloActivo = true;
	while(hiloActivo){
		switch(unEntrenador->estado){

			case READY: {
				sem_wait(&sem_Entrenador[unEntrenador->id]); //sem_wait(unEntrenador->SEMAFORO_IDENTIFICADOR);
				entrenador_pasar_a(unEntrenador, EXECUTE, "El planificador ha hablado");
				pokemon*unPokemon = mapa_desmapear(pokemonesRequeridos);
				sem_post(&sem_PokemonRemovido);

				entrenador_ir_a(unEntrenador, unPokemon->posicion);

				if(entrenador_llego_a(*unEntrenador, unPokemon->posicion)){
					t_id id_mensaje_pendiente = Catch(unPokemon->especie);
					//Agregar semaforo
					agregar_pendiente(mensajesPendientes, id_mensaje_pendiente, unEntrenador, unPokemon);
					printf("\n******************** >> Se agrego pendiente id %u\n\n", id_mensaje_pendiente);
					entrenador_pasar_a(unEntrenador, LOCKED_HASTA_CAUGHT, "Tiene una captura pendiente");
		//			exit(0);
				}

				else{
					entrenador_bloquear_hasta_APPEARD(unEntrenador);
					mapa_mapear_objetivo(pokemonesRequeridos, unPokemon);
				}

				break;
			}

			case LOCKED_HASTA_DEADLOCK: {
				//TODO
				entrenador_pasar_a(unEntrenador, EXIT, "Su inventario esta lleno pero no logro cumplir sus objetivos y aun no existe algoritmo de deadlock");

				break;
			}

			case  EXIT:{
				printf("Se libero al entrenador N°%u\n", unEntrenador->id);
				entrenadores_remover_del_equipo_a(equipo, unEntrenador->id);
				hiloActivo=false;

				break;
			}

			default: ;
				break;

		}
	}

	log_info(event_logger, "Finalizo un hilo entrenador");
	FinDelProceso = list_is_empty(equipo);
}

pthread_t* inicializar_hilos_entrenadores(int*cantidadEntrenadores){

	int i, cantidadDeHilos = list_size(equipo);
	pthread_t* hilosEntrenadores = malloc(sizeof(pthread_t)*cantidadDeHilos);

	sem_Entrenador = malloc(sizeof(sem_t)*cantidadDeHilos);

	for(i=0; i<cantidadDeHilos; i++){
		sem_init(&sem_Entrenador[i], 0, 0);
		pthread_create(&hilosEntrenadores[i], NULL, (void*) team_hilo_entrenador, list_get(equipo, i));
	}

	*cantidadEntrenadores = cantidadDeHilos;
	return hilosEntrenadores;
}
