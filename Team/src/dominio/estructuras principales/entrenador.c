#include "entrenador.h"

#include <commons/string.h>

#include "pokemon.h"

//Constructor Entrenador
entrenador entrenador_create(t_id id, matriz_recursos pokemonesEnInventario, matriz_recursos objetivos, t_posicion unaPos){
	entrenador nuevo = (entrenador) {objetivos, pokemonesEnInventario, unaPos, NEW, id, CATCHEAR};
	nuevo.siguienteTarea = CATCHEAR; //determinarSiguienteObjetivo(entrenador);
	return nuevo;
}

entrenador*entrenador_ptr_create(t_id id, matriz_recursos pokemonesEnInventario, matriz_recursos objetivos, t_posicion unaPos){
	entrenador*nuevo = malloc(sizeof(entrenador));
	*nuevo = entrenador_create(id, pokemonesEnInventario, objetivos, unaPos);
	return nuevo;
}

//Constructor de entrenador al cual le puedo pasar las matrices como cadenas de caracteres
entrenador*entrenador_ptr_crear(t_id id, char* asignados, char* pedidos, t_posicion unaPos){
	return entrenador_ptr_create(id, recursos_from_string(asignados), recursos_from_string(pedidos), unaPos);
}

matriz_recursos entrenador_objetivos(entrenador*unEntrenador){
	return unEntrenador->objetivos;
}

bool entrenador_cumplio_sus_objetivos(entrenador*unEntrenador){
	return recursos_suficientes_para(unEntrenador->pokemonesCazados, unEntrenador->objetivos);
}

bool entrenador_puede_cazar_mas_pokemones(entrenador* unEntrenador){
	return recursos_contar(unEntrenador->objetivos) > recursos_contar(unEntrenador->pokemonesCazados);
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

void entrenador_destroy(entrenador* destruido){
	recursos_destroy(destruido->objetivos);
	recursos_destroy(destruido->pokemonesCazados);
//	sem_destroy(destruido->SEMAFORO_IDENTIFICADOR);
	free(destruido);
}

//***************************************************************************
//Constructor de equipo
entrenadores entrenadores_create(){
	return list_create();
}

matriz_recursos entrenadores_objetivos_globales(entrenadores unEquipo){

	matriz_recursos objetivosGlobales = recursos_create();

	void agregarObjetivos(entrenador*unEntrenador){ //TODO no considera los que ya fueron capturados
		recursos_sumar_recursos_a(objetivosGlobales, unEntrenador->objetivos);
	}

	list_iterate(unEquipo, (void(*)(void*))&agregarObjetivos);

	return objetivosGlobales;
}

matriz_recursos entrenadores_inventarios_globales(entrenadores unEquipo){ //Ver como abstraer logica repetida
	matriz_recursos inventariosGlobales = recursos_create();

	void agregarInventarios(entrenador*unEntrenador){
		recursos_sumar_recursos_a(inventariosGlobales, unEntrenador->pokemonesCazados);
	}

	list_iterate(unEquipo, (void(*)(void*))&agregarInventarios);

	return inventariosGlobales;

}

//
entrenadores entrenadores_en_estado(entrenadores equipo, t_estado unEstado){
	bool esta_en_estado(void* unEntrenador){
		puts(estadoFromEnum(((entrenador*)unEntrenador)->estado));
		return entrenador_en_estado((entrenador*) unEntrenador, unEstado);
	}

	return list_filter(equipo, &esta_en_estado);
}

void entrenadores_disolver_equipo(entrenadores unEquipo){
	list_destroy(unEquipo);
}

//Destructor de equipo
void entrenadores_destroy(entrenadores equipo){
	list_destroy_and_destroy_elements(equipo, (void(*)(void*)) &entrenador_destroy);
}

//***************************************************************************

void entrenador_agregar_a_cola(entrenador*unEntrenador, cola_entrenadores cola){
	cr_list_add_and_signal(cola, unEntrenador);
}

entrenador*entrenador_esperar_y_desencolar(cola_entrenadores cola){
	return cr_list_wait_and_remove(cola, 0);
}

//Ver si poner CRITERIO, con enum
entrenador* entrenadores_proximo_a_planificar(cola_entrenadores colaDeReady){
//	switch(criterio){
//		case FIFO: {
		entrenador*proximo = cr_list_wait_and_remove(colaDeReady, 0); //FIFO
		return proximo;
//		}
//		default:
//			error_show("Algoritmo de planificacion desconocido");
//			exit(1);
//			return NULL;
}

t_id* entrenadores_id_proximo_a_planificar(cola_entrenadores colaDeReady){
	entrenador*proximo = entrenadores_proximo_a_planificar(colaDeReady);
	if(!proximo){
		return NULL; //quiere decir que no hay entrenadores disponibles. Deadlock?
	}

	return &proximo->id;
}