#include "../estructuras-principales/entrenador.h"

#include <commons/string.h>

#include "../estructuras-principales/pokemon.h"

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

matriz_recursos entrenador_recursos_sobrantes(entrenador*unEntrenador){
	return recursos_matriz_diferencia_positiva(unEntrenador->pokemonesCazados, unEntrenador->objetivos);
}

//retorna los objetivos no cumplidos del entrenador, o sea, la "necesidad particular"
matriz_recursos entrenador_recursos_pedidos(entrenador*unEntrenador){
	return recursos_matriz_diferencia_positiva(unEntrenador->objetivos, unEntrenador->pokemonesCazados);
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
	free(destruido);
}

//***************************************************************************
//Constructor de equipo
entrenadores entrenadores_create(){
	return list_create();
}

matriz_recursos entrenadores_objetivos_globales(entrenadores unEquipo){

	matriz_recursos objetivosGlobales = recursos_create();

	void agregarObjetivos(entrenador*unEntrenador){
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

matriz_recursos entrenadores_pedidos_insatisfechos(entrenadores unEquipo){
	matriz_recursos pedidos = recursos_create();

	void sumarInsatisfechos(entrenador*unEntrenador){
		matriz_recursos necesidad = entrenador_recursos_pedidos(unEntrenador); //Se va a ir con nueva estructura
		recursos_sumar_recursos_a(pedidos, necesidad);
		recursos_destroy(necesidad);
	}

	list_iterate(unEquipo, (void(*)(void*))&sumarInsatisfechos);

	return pedidos;
}

matriz_recursos entrenadores_recursos_sobrantes(entrenadores unEquipo){
	matriz_recursos sobrantes = recursos_create();

	void sumarSobrantes(entrenador*unEntrenador){
		matriz_recursos disponibles = entrenador_recursos_sobrantes(unEntrenador); //Se va a ir con nueva estructura
		recursos_sumar_recursos_a(sobrantes, disponibles);
		recursos_destroy(disponibles);
	}

	list_iterate(unEquipo, (void(*)(void*))&sumarSobrantes);

	return sobrantes;

}

//
entrenadores entrenadores_en_estado(entrenadores equipo, t_estado unEstado){
	bool esta_en_estado(void* unEntrenador){
		puts(estadoFromEnum(((entrenador*)unEntrenador)->estado));
		return entrenador_en_estado((entrenador*) unEntrenador, unEstado);
	}

	return list_filter(equipo, &esta_en_estado);
}

//retorna el entrenador mas cercano a una posicion
	entrenador* entrenadores_mas_cercano(entrenadores unEquipo, t_posicion unaPosicion){

		entrenador*entrenador_mas_cercano(entrenador*uno, entrenador*otro){
			if(!uno){
				return otro;
			}

			return posicion_distancia(uno->posicion, unaPosicion)<=posicion_distancia(otro->posicion, unaPosicion)? uno : otro;
		}

		return list_fold(unEquipo, NULL, (void*(*)(void*, void*))&entrenador_mas_cercano);

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
