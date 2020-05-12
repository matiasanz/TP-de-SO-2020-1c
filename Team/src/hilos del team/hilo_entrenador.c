#include "hilos_team.h"
#include "../team.h"

void team_hilo_entrenador(entrenador*unEntrenador){
	puts("en espera");
	int muchasVeces = 10;
	while(muchasVeces--){
		sem_wait(unEntrenador->SEMAFORO_IDENTIFICADOR);
		pokemon*unPokemon = mapa_desmapear(pokemonesRequeridos);

		entrenador_ir_a(unEntrenador, unPokemon->posicion);

		if(entrenador_llego_a(*unEntrenador, unPokemon->posicion)){
			t_id id_mensaje_pendiente = catch(unPokemon->especie);
			agregar_pendiente(mensajesPendientes, id_mensaje_pendiente, unEntrenador, unPokemon);
			entrenador_bloquear_hasta_APPEARD(unEntrenador);// pasa a LOCKED
		}

		else{
			entrenador_bloquear_hasta_APPEARD(unEntrenador);
			mapa_mapear_objetivo(pokemonesRequeridos, unPokemon);
		}
	}
}

pthread_t* inicializar_hilos_entrenadores(int*cantidadDeHilos){
	int i, cantidadEntrenadores = list_size(equipo);
	pthread_t* hilosEntrenadores = malloc(sizeof(pthread_t)*cantidadEntrenadores);

	for(i=0; i<cantidadEntrenadores; i++){
		pthread_create(&hilosEntrenadores[i], NULL, (void*) team_hilo_entrenador, list_get(equipo, i));
	}

	*cantidadDeHilos = cantidadEntrenadores;
	return hilosEntrenadores;
}
