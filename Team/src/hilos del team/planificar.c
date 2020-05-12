#include "../team.h"
#include "hilos_team.h"
#include<semaphore.h>

// Pasa a estado ready a los entrenadores que se encuentran en NEW o en LOCKED hasta APPEARD
	void entrenadores_despertar_APPEARD(entrenadores, pokemon*);

//*******************************************************************

void team_planificar(){

	int i, muchasVeces=10;
	for(i=0; muchasVeces--; i = (i+1)%list_size(equipo)){

		especies_pokemones objetivosGlobales = entrenadores_objetivos_globales(equipo);
		list_iterate(objetivosGlobales, get);
		list_destroy(objetivosGlobales);

		// wait(hayMasPokemonesEnMapa);

		pokemon*unPokemon = mapa_first(pokemonesRequeridos);
		entrenadores_despertar_APPEARD(equipo, unPokemon);
		sem_t* idProximoEntrenador = entrenadores_id_proximo_a_planificar(equipo); //TODO

		if(!idProximoEntrenador){
			error_show("no hay entrenadores en ready");
			abort();
		}

//		sem_post(idProximoEntrenador); //signal(id);
	}
}


void entrenadores_despertar_APPEARD(entrenadores equipo, pokemon* unPokemon){

	void entrenador_despertar_appeard(entrenador*unEntrenador){
		if(entrenador_en_estado(unEntrenador, NEW) || entrenador_en_estado(unEntrenador, LOCKED_HASTA_APPEARD)){
			entrenador_desbloquear(unEntrenador);
		}
	}

	list_iterate(equipo, (void(*)(void*)) &entrenador_despertar_appeard);

	bool porCercania(entrenador*unEntrenador, entrenador* otroEntrenador){
		numero distanciaDelPrimero = posicion_distancia(unEntrenador->posicion, unPokemon->posicion);
		numero distanciaDelSegundo = posicion_distancia(otroEntrenador->posicion, unPokemon->posicion);
		return  distanciaDelPrimero - distanciaDelSegundo;
	}

	list_sort(equipo, (bool(*)(void*, void*)) &porCercania);
}

//***************************************************************************
// version de muestra
void MODELO_PLANIFICAR(){
	pokemon* unPokemon = mapa_desmapear(pokemonesRequeridos);

		if(!unPokemon){
			puts("<<no hay mas pokemones en mapa>>");
			abort();
		}

		entrenadores_despertar_APPEARD(equipo, unPokemon);

	entrenador* unEntrenador = entrenadores_proximo_a_planificar(equipo);

		if(!unEntrenador){
			puts("<<no hay entrenadores disponibles>>");
			mapa_mapear_objetivo(pokemonesRequeridos, unPokemon);
			abort();
		}

	entrenador_ir_a(unEntrenador, unPokemon->posicion);
	t_id id_mensaje_pendiente = catch(unPokemon->especie);

	agregar_pendiente(mensajesPendientes, id_mensaje_pendiente, unEntrenador, unPokemon);

	entrenador_bloquear_hasta_APPEARD(unEntrenador);// pasa a LOCKED
}
