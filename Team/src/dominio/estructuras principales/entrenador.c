#include <commons/string.h>
#include "entrenador.h"
#include "pokemon.h"
#include "../estructuras auxiliares/mensajes.h" //si en header rompe


//Constructor Entrenador
entrenador entrenador_create(t_id id, t_list* pokemonesEnInventario, t_list*objetivos, t_posicion unaPos){
	entrenador nuevo = (entrenador) {objetivos, pokemonesEnInventario, unaPos, NEW, id, CATCHEAR};
	nuevo.siguienteTarea = CATCHEAR; //determinarSiguienteObjetivo(entrenador);
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
	t_posicion posicionActual = unEntrenador->posicion;
//	int i=QUANTUM; if(criterio == ROUND_ROBBIN && distancia>QUANTUM){}

//	numero distancia = posicion_distancia(unEntrenador->posicion, posicionFinal);

//	pthread_mutex_t bloqueado;
//	pthread_mutex_init(&bloqueado, ...); //Duda, vale la pena?
//	sleep(distancia*tiempoPorDistancia);// VER alternativas para sleep... o un mutex?

	unEntrenador->posicion = posicionFinal;
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El Entrenador N°%u se desplazo desde [%u %u] hasta [%u %u]", unEntrenador->id, posicionActual.pos_x, posicionActual.pos_y, unEntrenador->posicion.pos_x, unEntrenador->posicion.pos_y);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

bool entrenador_llego_a(entrenador unEntrenador, t_posicion posicion){
	return posicion_cmp(unEntrenador.posicion, posicion);
}

bool entrenador_en_estado(entrenador* unEntrenador, t_estado ESTADO){

//	pthread_mutex_lock(&Mutex_Entrenador[unEntrenador->id]);
	bool estaEnEstado = unEntrenador->estado == ESTADO;
//	pthread_mutex_lock(&Mutex_Entrenador[unEntrenador->id]);

	return estaEnEstado;
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

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El Entrenador N°%u se paso de la cola de %s a %s, Motivo: %s", unEntrenador->id, estadoActual, estadoFromEnum(estadoFinal), motivo);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);

	if(estadoFinal==EXIT) entrenador_destroy(unEntrenador);
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
	bool entrenador_cmpId(entrenador*unEntrenador){
		return id == unEntrenador->id;
	}
	return list_remove_by_condition(unEquipo, (bool(*)(void*)) &entrenador_cmpId);
}

especies_pokemones entrenadores_objetivos_globales(entrenadores unEquipo){

	especies_pokemones objetivosGlobales = list_create(); //Implementar con dictionary

	//TODO

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

