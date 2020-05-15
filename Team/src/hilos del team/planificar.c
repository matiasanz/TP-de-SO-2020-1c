#include "../team.h"
#include "hilos_team.h"
#include<semaphore.h>

// Pasa a estado ready a los entrenadores que se encuentran en NEW o en LOCKED hasta APPEARD
	void entrenadores_despertar_APPEARD(entrenadores, pokemon*);

//*******************************************************************

/*TODO
 * No frena nunca
 */

void team_planificar(){

//	sem_init(&sem_HayMasPokemonesEnMapa, 0, 0); //por ahora esta en procesar mensajes

	puts("**********************************************\nSe va a planificar");
	int muchasVeces=1;
	while(muchasVeces){
		//A futuro, al capturar un pokemon se lo eliminara de los objs del entrenador, con lo cual pasaria a region critica
		especies_pokemones objetivosGlobales = entrenadores_objetivos_globales(equipo);
		Get_pokemones(objetivosGlobales);

		puts(">>>Planificar: wait(hay mas pokemones)");
		sem_wait(&sem_HayMasPokemonesEnMapa);

		puts(">>>Planificar: wait(pokemon removido)");
		sem_wait(&sem_PokemonRemovido);
		pokemon*unPokemon = mapa_first(pokemonesRequeridos);

		if(!unPokemon){
			error_show("Se intento leer un pokemon inexistente\n");
			exit(1);
		}

		entrenadores_despertar_APPEARD(equipo, unPokemon);
		t_id* idProximoEntrenador = entrenadores_id_proximo_a_planificar(equipo); //TODO

		if(idProximoEntrenador){
			printf(">>>Planificar: signal(Entrenador N°%u)\n", *idProximoEntrenador);
			sem_post(&sem_Entrenador[*idProximoEntrenador]); //signal(id);
		}

		else if(FinDelProceso){
			puts("Todos los entrenadores en EXIT");
			break;
		}

	}

	log_info(event_logger, "Se finalizo hilo de planificacion");
}


void entrenadores_despertar_APPEARD(entrenadores equipo, pokemon* unPokemon){

	void entrenador_despertar_appeard(entrenador*unEntrenador){
		if(entrenador_en_estado(unEntrenador, NEW) || entrenador_en_estado(unEntrenador, LOCKED_HASTA_APPEARD)){
			entrenador_pasar_a(unEntrenador, READY, "Acaba de llegar un pokemon que puede cazar");
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
	t_id id_mensaje_pendiente = Catch(unPokemon->especie);

	agregar_pendiente(mensajesPendientes, id_mensaje_pendiente, unEntrenador, unPokemon);

	entrenador_bloquear_hasta_APPEARD(unEntrenador);// pasa a LOCKED
}
