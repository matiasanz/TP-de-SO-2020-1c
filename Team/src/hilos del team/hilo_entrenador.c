#include "hilos_team.h"
#include "../team.h"

void team_hilo_entrenador(entrenador*unEntrenador){
	printf("Entrenador N°%u en espera\n", unEntrenador->id);
	t_id pid = unEntrenador->id;

	bool hiloActivo = true;
	while(hiloActivo){

		sem_wait(&EjecutarEntrenador[pid]);
		entrenador_pasar_a(unEntrenador, EXECUTE, "Es su turno de ejecutar");

		switch(unEntrenador->siguienteTarea){
			case CATCHEAR: {
				pokemon*unPokemon = mapa_desmapear(pokemonesRequeridos);

		//				pthread_mutex_lock(&mutexEntrenador[pid]);
				entrenador_ir_a(unEntrenador, unPokemon->posicion);

				if(entrenador_llego_a(*unEntrenador, unPokemon->posicion)){
		//					pthread_mutex_unlock(&mutexEntrenador[pid]);
					t_id id_mensaje_pendiente = Catch(unPokemon->especie);

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
				pendiente* capturaPendiente = pendientes_pendiente_del_entrenador(capturasPendientes, pid);

				if(!capturaPendiente){
					error_show("El id del entrenador N°%u no se corresponde con ningun mensaje pendiente\n", pid);
					exit(1);
				}

				pokemon*pokemonCatcheado = capturaPendiente->pokemonCatcheado;

				entrenador_capturar(unEntrenador, pokemonCatcheado);

//				pthread_mutex_lock(&mutexEntrenador[unEntrenador->id]);
				if(entrenador_puede_cazar_mas_pokemones(unEntrenador)){ //Ver si cambios de estado se pueden delegar al planificador
					unEntrenador->siguienteTarea = CATCHEAR;
					entrenador_pasar_a(unEntrenador, LOCKED_HASTA_APPEARD, "Tuvo exito en la captura y todavia puede cazar mas pokemones");
				}

				else if(entrenador_cumplio_sus_objetivos(unEntrenador)){
//					unEntrenador->siguienteTarea = FINALIZAR;

					printf("Objetivos: "); recursos_mostrar(unEntrenador->objetivos);
					printf("Inventario: "); recursos_mostrar(unEntrenador->pokemonesCazados);

					entrenador_pasar_a(unEntrenador, EXIT, "Ya logro cumplir sus objetivos");

					entrenadores_remover_del_equipo_a(equipo, pid);
					hiloActivo = false;
				}

				else{
					unEntrenador->siguienteTarea = DEADLOCK;
					entrenador_pasar_a(unEntrenador, LOCKED_HASTA_DEADLOCK, "Su inventario esta lleno y no cumplio sus objetivos");

					printf("Objetivos: "); recursos_mostrar(unEntrenador->objetivos);puts("");
					printf("Inventario: "); recursos_mostrar(unEntrenador->pokemonesCazados);puts("");


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

	entrenador_destroy(unEntrenador);

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Finalizo un hilo entrenador");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

}

numero cantidadDeEntrenadores;

void inicializar_hilos_entrenadores(){
	cantidadDeEntrenadores = list_size(equipo);

	hilosEntrenadores = malloc(sizeof(pthread_t)*cantidadDeEntrenadores);

	EjecutarEntrenador = malloc(sizeof(sem_t)*cantidadDeEntrenadores);
	mutexEstadoEntrenador = malloc(sizeof(pthread_mutex_t)*cantidadDeEntrenadores);

	int i;
	for(i=0; i<cantidadDeEntrenadores; i++){
		sem_init(&EjecutarEntrenador[i], 0, 0);
		pthread_mutex_init(&mutexEstadoEntrenador[i], NULL);
		pthread_create(&hilosEntrenadores[i], NULL, (void*) team_hilo_entrenador, list_get(equipo, i));
	}
}

void finalizar_hilos_entrenadores(){
	int i=0;
	for(i=0; i<cantidadDeEntrenadores; i++){
		pthread_join(hilosEntrenadores[i], NULL);
		pthread_mutex_destroy(&mutexEstadoEntrenador[i]);
	}
}



void entrenador_pasar_a(entrenador*unEntrenador, t_estado estadoFinal, const char*motivo){
	pthread_mutex_lock(&mutexEstadoEntrenador[unEntrenador->id]);
	t_estado estadoActual = unEntrenador->estado;
	unEntrenador->estado = estadoFinal;
	pthread_mutex_unlock(&mutexEstadoEntrenador[unEntrenador->id]);

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El Entrenador N°%u se paso de la cola de %s a %s, Motivo: %s", unEntrenador->id, estadoFromEnum(estadoActual), estadoFromEnum(estadoFinal), motivo);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

void entrenador_capturar(entrenador*entrenador, pokemon*victima){

	recursos_agregar_recurso(entrenador->pokemonesCazados, victima->especie);
//	recursos_agregar_recurso(inventariosGlobales, victima->especie);

	t_posicion posicionDelEvento = entrenador->posicion;

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El Entrenador N°%u ha capturado un %s en la posicion [%u %u]", entrenador->id, victima->especie, posicionDelEvento.pos_x, posicionDelEvento.pos_y);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);

	pokemon_destroy(victima);
}

entrenador* entrenadores_remover_del_equipo_a(entrenadores unEquipo, t_id id){
	bool entrenador_cmpId(entrenador*unEntrenador){
		return id == unEntrenador->id;
	}

	pthread_mutex_lock(&mutexEntrenadores);
	entrenador*removido = list_remove_by_condition(unEquipo, (bool(*)(void*)) &entrenador_cmpId);
	pthread_mutex_unlock(&mutexEntrenadores);

	return removido;
}

