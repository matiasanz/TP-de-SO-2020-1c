#include "hilos_team.h"
#include "../team.h"

/*TODO
 * corregir implementacion de capturar pokemon, considerar que cada llamado a objetivos globales pasa a ser region critica.
 * Puede pasar eventualmente que un entrenador necesite capturar un pokemon y se ejecute a la par que otro. Duda: Puedo pausar un hilo?
 */

/************************************ nueva implementacion ****************************************/

void team_hilo_entrenador(entrenador*unEntrenador){
	printf("Entrenador N°%u en espera\n", unEntrenador->id);
	t_id pid = unEntrenador->id;

	bool hiloActivo = true;
	while(hiloActivo){

		sem_wait(&EjecutarEntrenador[pid]);
		entrenador_pasar_a(unEntrenador, EXECUTE, "Es su turno de ejecutar");

		printf("Objetivo: %s\n", (unEntrenador->siguienteTarea == CATCHEAR? "catchear": unEntrenador->siguienteTarea == CAPTURAR? "capturar": "deadlock"));

		switch(unEntrenador->siguienteTarea){
			case CATCHEAR: {
				pokemon*unPokemon = mapa_desmapear(pokemonesRequeridos);

		//				pthread_mutex_lock(&mutexEntrenador[pid]);
				entrenador_ir_a(unEntrenador, unPokemon->posicion);

				if(entrenador_llego_a(*unEntrenador, unPokemon->posicion)){
		//					pthread_mutex_unlock(&mutexEntrenador[pid]);
					t_id id_mensaje_pendiente = Catch(unPokemon->especie);
					//Agregar semaforo
					agregar_pendiente(capturasPendientes, id_mensaje_pendiente, unEntrenador, unPokemon);

					unEntrenador->siguienteTarea = CAPTURAR;

					entrenador_pasar_a(unEntrenador, LOCKED_HASTA_CAUGHT, "Tiene una captura pendiente");
				}

				else{
					entrenador_pasar_a(unEntrenador, LOCKED_HASTA_APPEARD, "No llego a la posicion del pokemon");
					mapa_mapear_objetivo(pokemonesRequeridos, unPokemon);
				}

				break;
			}

			case CAPTURAR: {
				puts("Proximamente capturar");
				pendiente* capturaPendiente = pendientes_pendiente_del_entrenador(capturasPendientes, pid);

				if(!capturaPendiente){
					error_show("El id del entrenador N°%u no se corresponde con ningun mensaje pendiente\n", pid);
					exit(1);
				}

				pokemon*pokemonCatcheado = capturaPendiente->pokemonCatcheado;

				entrenador_capturar(unEntrenador, pokemonCatcheado); //TODO

					//VER TODO muchos if anidados

//				pthread_mutex_lock(&mutexEntrenador[unEntrenador->id]);
				if(entrenador_puede_cazar_mas_pokemones(*unEntrenador)){ //Ver si cambios de estado se pueden delegar al planificador
					unEntrenador->siguienteTarea = CATCHEAR;
					entrenador_pasar_a(unEntrenador, LOCKED_HASTA_APPEARD, "Tuvo exito en la captura y todavia puede cazar mas pokemones");
				}

				else if(entrenador_objetivos_cumplidos(unEntrenador)){ //abstraer a mensaje objetivos cumplidos
//					unEntrenador->siguienteTarea = FINALIZAR;
					entrenador_pasar_a(unEntrenador, EXIT, "Ya logro cumplir sus objetivos");
					entrenadores_remover_del_equipo_a(equipo, pid);
					hiloActivo = false;
				}

				else{
					unEntrenador->siguienteTarea = DEADLOCK;
					entrenador_pasar_a(unEntrenador, LOCKED_HASTA_DEADLOCK, "Su inventario esta lleno y no cumplio sus objetivos");

					//TODO: Ver en que momento estos entrenadores vuelven a READY
					entrenador_pasar_a(unEntrenador, EXIT, "Aun no esta implementado el algoritmo de deadlock");
					entrenadores_remover_del_equipo_a(equipo, pid);
					hiloActivo = false;
				}

				break;
			}

			case DEADLOCK: {
				puts("Proximamente deadlock");
				break;
			}
		}

//				pthread_mutex_unlock(&mutexEntrenador[unEntrenador->id]);
		sem_post(&EntradaSalida_o_FinDeEjecucion);
	}

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Finalizo un hilo entrenador");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

}

pthread_t* inicializar_hilos_entrenadores(int*cantidadEntrenadores){

	int i, cantidadDeHilos = list_size(equipo);
	pthread_t* hilosEntrenadores = malloc(sizeof(pthread_t)*cantidadDeHilos);

	EjecutarEntrenador = malloc(sizeof(sem_t)*cantidadDeHilos);
//	mutexEntrenador = malloc(sizeof(pthread_mutex_t)*cantidadDeHilos);

	for(i=0; i<cantidadDeHilos; i++){
		sem_init(&EjecutarEntrenador[i], 0, 0);
//		pthread_mutex_init(&mutexEntrenador[i], NULL);
		pthread_create(&hilosEntrenadores[i], NULL, (void*) team_hilo_entrenador, list_get(equipo, i));
	}

	*cantidadEntrenadores = cantidadDeHilos;
	return hilosEntrenadores;
}
