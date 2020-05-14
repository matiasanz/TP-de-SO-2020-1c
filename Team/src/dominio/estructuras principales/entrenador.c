#include <commons/string.h>
#include "entrenador.h"
#include "../estructuras auxiliares/mensajes.h" //si en header rompe


//Constructor Entrenador
entrenador entrenador_create(t_list* pokemonesEnInventario, t_list*objetivos, t_posicion unaPos){
	entrenador nuevo = (entrenador) {objetivos, pokemonesEnInventario, unaPos, NEW};
	nuevo.SEMAFORO_IDENTIFICADOR = malloc(sizeof(sem_t));
	sem_init(nuevo.SEMAFORO_IDENTIFICADOR, 0, 0);
	return nuevo;
}

entrenador*entrenador_ptr_create(t_list* pokemonesEnInventario, t_list*objetivos, t_posicion unaPos){
	entrenador*nuevo = malloc(sizeof(entrenador));
	*nuevo = entrenador_create(pokemonesEnInventario, objetivos, unaPos);
	return nuevo;
}

t_list*entrenador_objetivos(entrenador*unEntrenador){
	return unEntrenador->objetivos;
}

void entrenador_ir_a(entrenador* unEntrenador, t_posicion posicionFinal){

//	int i=QUANTUM; if(criterio == ROUND_ROBBIN && distancia>QUANTUM){}

//	numero distancia = posicion_distancia(unEntrenador->posicion, posicionFinal);

//	sleep(distancia*tiempoPorDistancia);// VER alternativas para sleep

	unEntrenador->posicion = posicionFinal;
//	log_info(logger, "El entrenador llego a la posicion (%u, %u)", unEntrenador->posicion.x, unEntrenador->posicion.y);
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

void entrenador_pasar_a(entrenador*unEntrenador, t_estado estado, const char*motivo){

	unEntrenador->estado = estado;

//	char*estadoFromEnum(){
//		switch(estado)
//		case NEW:       {return "NEW";}
//		case READY:	    {return "Ready";}
//		case EXIT:	    {return "Exit";}
//		default:		{return "LOCKED";}
//	}
//
//	char*unEstado = estado_from_enum(estado);
//	log_info(logger, "Se paso el entrenador (id) a estado %s. Motivo: %s", estado, motivo);
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
	sem_destroy(destruido->SEMAFORO_IDENTIFICADOR);
	free(destruido);
}

//***************************************************************************
//Constructor de equipo
entrenadores entrenadores_create(){
	return list_create();
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

sem_t* entrenadores_id_proximo_a_planificar(entrenadores equipo){
	entrenador*proximo = entrenadores_proximo_a_planificar(equipo);
	if(!proximo){
		printf("no encontre mas\n");
		return NULL; //quiere decir que no hay entrenadores disponibles. Deadlock?
	}

	return proximo->SEMAFORO_IDENTIFICADOR;
}

//
entrenadores entrenadores_en_estado(entrenadores equipo, t_estado unEstado){
	bool esta_en_estado(void* unEntrenador){
		return entrenador_en_estado((entrenador*) unEntrenador, unEstado);
	}

	return list_filter(equipo, &esta_en_estado);
}


//Destructor de equipo
void entrenadores_destroy(entrenadores equipo){
	list_destroy_and_destroy_elements(equipo, (void(*)(void*)) &entrenador_destroy);
}

