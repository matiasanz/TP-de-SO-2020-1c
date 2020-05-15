#include <commons/string.h>
#include "entrenador.h"
#include "pokemon.h"
#include "../estructuras auxiliares/mensajes.h" //si en header rompe


//Constructor Entrenador
entrenador entrenador_create(t_id id, t_list* pokemonesEnInventario, t_list*objetivos, t_posicion unaPos){
	entrenador nuevo = (entrenador) {objetivos, pokemonesEnInventario, unaPos, NEW, id};
//	nuevo.SEMAFORO_IDENTIFICADOR = malloc(sizeof(sem_t));
//	sem_init(nuevo.SEMAFORO_IDENTIFICADOR, 0, 0);
	return nuevo;
}

entrenador*entrenador_ptr_create(t_id id, t_list* pokemonesEnInventario, t_list*objetivos, t_posicion unaPos){
	entrenador*nuevo = malloc(sizeof(entrenador));
	*nuevo = entrenador_create(id, pokemonesEnInventario, objetivos, unaPos);
	return nuevo;
}

t_list*entrenador_objetivos(entrenador*unEntrenador){
	return unEntrenador->objetivos;
}

bool entrenador_objetivos_cumplidos(entrenador*unEntrenador){

	bool seCumplio(void* unaEspecie){

		bool mismaEspecie(void* deLista){
			return especie_cmp((especie_pokemon)unaEspecie, (especie_pokemon)deLista);
		}

		return list_any_satisfy(unEntrenador->pokemonesCazados, &mismaEspecie);
	}

	return list_all_satisfy(unEntrenador->objetivos, &seCumplio);
}

void entrenador_ir_a(entrenador* unEntrenador, t_posicion posicionFinal){

//	int i=QUANTUM; if(criterio == ROUND_ROBBIN && distancia>QUANTUM){}

//	numero distancia = posicion_distancia(unEntrenador->posicion, posicionFinal);

//	pthread_mutex_t bloqueado;
//	pthread_mutex_init(&bloqueado, ...); //Duda, vale la pena?
//	sleep(distancia*tiempoPorDistancia);// VER alternativas para sleep... o un mutex?

	unEntrenador->posicion = posicionFinal;
	log_info(logger, "El Entrenador N°%u se desplazo hasta [%u] [%u]", unEntrenador->id, unEntrenador->posicion.x, unEntrenador->posicion.y);
}

bool entrenador_llego_a(entrenador unEntrenador, t_posicion posicion){
	return posicion_cmp(unEntrenador.posicion, posicion);
}

bool entrenador_en_estado(entrenador* unEntrenador, t_estado ESTADO){
	return unEntrenador->estado == ESTADO;
}

bool entrenador_puede_cazar_mas_pokemones(entrenador unEntrenador){
	return list_size(unEntrenador.objetivos) > list_size(unEntrenador.pokemonesCazados);
}

char*estadoFromEnum(t_estado unEstado){
	switch(unEstado){
		case NEW:       {return "NEW";}
		case READY:	    {return "READY";}
		case EXIT:	    {return "EXIT";}
		case EXECUTE:	{return "EXECUTE";}
		default:		{return "LOCKED";}
	}
}


void entrenador_pasar_a(entrenador*unEntrenador, t_estado estadoFinal, const char*motivo){
	char*estadoActual = estadoFromEnum(unEntrenador->estado);
	unEntrenador->estado = estadoFinal;
	log_info(logger, "El Entrenador N°%u se paso de la cola de %s a %s, Motivo: %s", unEntrenador->id, estadoActual, estadoFromEnum(estadoFinal), motivo);

	if(estadoFinal==EXIT) entrenador_destroy(unEntrenador);
}

void entrenador_bloquear_hasta_APPEARD(entrenador* unEntrenador){
	unEntrenador->estado = LOCKED_HASTA_APPEARD;
	puts("<<Se ha bloqueado al entrenador hasta appeard>>");
}

void entrenador_bloquear_hasta_CAUGHT(entrenador*unEntrenador){
	unEntrenador->estado = LOCKED_HASTA_APPEARD;
	puts("<<Se ha bloqueado al entrenador hasta caught>>");

}

void entrenador_bloquear_hasta_DEADLOCK(entrenador*unEntrenador){
	unEntrenador->estado = LOCKED_HASTA_DEADLOCK;
	puts("<<Se ha bloqueado al entrenador hasta deadlock>>");
}

void entrenador_desbloquear(entrenador*unEntrenador){
	unEntrenador->estado = READY;
	puts("entrenador en ready");
}

void entrenador_destroy(entrenador* destruido){
	list_destroy(destruido->objetivos);
	list_destroy(destruido->pokemonesCazados);
//	sem_destroy(destruido->SEMAFORO_IDENTIFICADOR);
	free(destruido);
}

//***************************************************************************
//Constructor de equipo
entrenadores entrenadores_create(){
	return list_create();
}

entrenador* entrenadores_remover_del_equipo_a(entrenadores unEquipo, t_id id){
	bool entrenador_cmpId(void*unEntrenador){
		return id == ((entrenador*)unEntrenador)->id;
	}
	return list_remove_by_condition(unEquipo, &entrenador_cmpId);
}

especies_pokemones entrenadores_objetivos_globales(entrenadores unEquipo){

	especies_pokemones objetivosGlobales = list_create();

//	void agregarObjetivosSinRepetidos(especies_pokemones especies){
//
//		void agregarObjetivoSinRepetidos(especie_pokemon unaEspecie){
//
//			bool especieRepetida(especie_pokemon otraEspecie){
//				return especie_cmp(unaEspecie,  otraEspecie);
//			}
//
//			if(!list_find(objetivosGlobales, (bool*) especieRepetida))
//				especies_agregar(objetivosGlobales, unaEspecie);
//		}
//
//		list_iterate(especies, (void*) agregarObjetivoSinRepetidos);
//	}

	void agregarObjetivos(entrenador*unEntrenador){
		list_add_all(objetivosGlobales, entrenador_objetivos(unEntrenador));
	}

	list_iterate(unEquipo, (void(*)(void*))&agregarObjetivos);

	return objetivosGlobales;
}

entrenador* entrenadores_mas_cercano(entrenadores unEquipo, t_posicion unaPosicion){
	entrenador*entrenador_mas_cercano(entrenador*uno, entrenador*otro){
		if(!uno){
			return otro;
		}

		return posicion_distancia(uno->posicion, unaPosicion)>=posicion_distancia(otro->posicion, unaPosicion)? uno : otro;
	}

	return list_fold(unEquipo, NULL, (void*(*)(void*, void*))&entrenador_mas_cercano);
}

//Ver si poner CRITERIO, con enum
entrenador* entrenadores_proximo_a_planificar(entrenadores equipo){ //, pokemon){
//	switch(criterio){
//		case FIFO: {
		entrenadores entrenadores_ready = entrenadores_en_estado(equipo, READY);
		entrenador*proximo = list_remove(entrenadores_ready, 0); //FIFO
		list_destroy(entrenadores_ready);
		return proximo;
//		}
//		default:
//			return NULL;
}

t_id* entrenadores_id_proximo_a_planificar(entrenadores equipo){
	entrenador*proximo = entrenadores_proximo_a_planificar(equipo);
	if(!proximo){
		printf("no encontre mas\n");
		return NULL; //quiere decir que no hay entrenadores disponibles. Deadlock?
	}

	return &proximo->id;
//	return proximo->SEMAFORO_IDENTIFICADOR;
}

//
entrenadores entrenadores_en_estado(entrenadores equipo, t_estado unEstado){
	bool esta_en_estado(void* unEntrenador){
		return entrenador_en_estado((entrenador*) unEntrenador, unEstado);
	}

	return list_filter(equipo, &esta_en_estado);
}

void entrenadores_bloquear_por_captura(entrenadores unEquipo){

	void sacar_de_execute(entrenador*unEntrenador){
		if(entrenador_en_estado(unEntrenador, EXECUTE)) entrenador_pasar_a(unEntrenador, LOCKED_HASTA_APPEARD, "Se ejecutara a otro entrenador");
	}

	list_iterate(unEquipo, (void(*)(void*)) &sacar_de_execute);
}

//Destructor de equipo
void entrenadores_destroy(entrenadores equipo){
	list_destroy_and_destroy_elements(equipo, (void(*)(void*)) &entrenador_destroy);
}

