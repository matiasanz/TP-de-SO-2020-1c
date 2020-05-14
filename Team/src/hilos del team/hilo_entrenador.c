#include "hilos_team.h"
#include "../team.h"

void team_hilo_entrenador(entrenador*unEntrenador){
	puts("en espera");
//	int muchasVeces = 10;
	while(1){
		sem_wait(&sem_Entrenador[unEntrenador->id]); //sem_wait(unEntrenador->SEMAFORO_IDENTIFICADOR);
		unEntrenador->estado = EXECUTE;
		pokemon*unPokemon = mapa_desmapear(pokemonesRequeridos);
		sem_post(&sem_PokemonRemovido);

		entrenador_ir_a(unEntrenador, unPokemon->posicion);

		if(entrenador_llego_a(*unEntrenador, unPokemon->posicion)){
			t_id id_mensaje_pendiente = catch(unPokemon->especie);
			agregar_pendiente(mensajesPendientes, id_mensaje_pendiente, unEntrenador, unPokemon);
			entrenador_pasar_a(unEntrenador, LOCKED_HASTA_CAUGHT, "Tiene una captura pendiente");
//			exit(0);
		}

		else{
			entrenador_bloquear_hasta_APPEARD(unEntrenador);
			mapa_mapear_objetivo(pokemonesRequeridos, unPokemon);
		}
	}
}

pthread_t* inicializar_hilos_entrenadores(int*cantidadEntrenadores){

	int i, cantidadDeHilos = list_size(equipo);
	pthread_t* hilosEntrenadores = malloc(sizeof(pthread_t)*cantidadDeHilos);

	sem_Entrenador = malloc(sizeof(sem_t)*cantidadDeHilos);

	for(i=0; i<cantidadDeHilos; i++){
		entrenador*unEntrenador = list_get(equipo, i);
		unEntrenador->id = i;
		sem_init(&sem_Entrenador[i], 0, 0);
		pthread_create(&hilosEntrenadores[i], NULL, (void*) team_hilo_entrenador, unEntrenador);
	}

	*cantidadEntrenadores = cantidadDeHilos;
	return hilosEntrenadores;
}
