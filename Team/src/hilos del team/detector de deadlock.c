#include "../team.h"
#include "hilos_team.h"
#include "detector de deadlock.h"
#include "../dominio/estructuras auxiliares/deadlock.h"

#define TIEMPO_ENTRE_DETECCIONES 15 //segundos

candidatos_intercambio entrenadoresConRecursosEnExceso;
pares_intercambio intercambiosPendientes;

//Es un gran TODO
void team_ejecutar_algoritmo_de_deteccion_de_deadlock(){


	while(PROCESO_ACTIVO){
		sleep(TIEMPO_ENTRE_DETECCIONES);

		entrenadores entrenadoresEnDeadlock = entrenadores_create();

		bool hayDeadlock = algoritmo_detectar_deadlock(entrenadoresConRecursosEnExceso, entrenadoresEnDeadlock);

//		algoritmo_procesar_deadlock(entrenadoresEnDeadlock);

		entrenadores_disolver_equipo(entrenadoresEnDeadlock);

		pthread_mutex_lock(&Mutex_AndoLoggeando);
		log_info(logger, "Se ejecuto el detector de deadlock, resultado: %s", ( hayDeadlock? "Positivo": "Negativo") );
		pthread_mutex_unlock(&Mutex_AndoLoggeando);
	}
}

///*************************************** Detectar Deadlock ************************************************/
/*Descripcion:
 *  Se establecen los objetivos que le faltan a cada entrenador y el total de los sobrantes. Luego, se van evaluando
 *  los distintos subconjuntos de la lista con recursos en exceso, pensando en cada instancia como un arbol de la forma
 *    v             (ó)
 *  L   D ; (libero)   (dejo)
 *
 */
bool algoritmo_detectar_deadlock(entrenadores unEquipo, entrenadores enDeadlock){

	matriz_recursos necesidad = entrenadores_pedidos_insatisfechos(unEquipo);
	matriz_recursos disponibles = entrenadores_recursos_sobrantes(unEquipo);

	entrenadores omitidos = entrenadores_create();

	bool hayDeadlock = hay_espera_circular(unEquipo, necesidad, disponibles)
		|| hay_deadlock_exceptuando_al_siguiente(unEquipo, necesidad, disponibles, omitidos, enDeadlock)
		|| hay_deadlock_dejando_al_siguiente(unEquipo, necesidad, disponibles, omitidos, enDeadlock);

	recursos_destroy(necesidad);
	recursos_destroy(disponibles);
	list_destroy(omitidos);

	return hayDeadlock;
}

bool hay_deadlock_exceptuando_al_siguiente(entrenadores unEquipo, matriz_recursos necesidad, matriz_recursos disponibles, entrenadores omitidos, entrenadores enDeadlock){

	entrenadores procesos = unEquipo;
	entrenadores procesosOmitidos = list_duplicate(omitidos);

	entrenador*unEntrenador = list_remove(procesos, 0);

	entrenadores aEvaluar = list_duplicate(procesos);
	list_add_all(aEvaluar, procesosOmitidos);

	if(!unEntrenador){
		puts("L-NULL");
		return false;
	}

	printf("L%d\n", unEntrenador->id);

	matriz_recursos necesidadActual = recursos_matriz_diferencia(necesidad, entrenador_recursos_pedidos(unEntrenador));
	matriz_recursos disponiblesActuales = recursos_matriz_diferencia(disponibles, entrenador_recursos_sobrantes(unEntrenador));


	bool esperaCircular = hay_espera_circular(aEvaluar, necesidadActual, disponiblesActuales);

	if(esperaCircular){
		list_add_all(enDeadlock, aEvaluar);
	}

	bool deadlock = esperaCircular
				 || hay_deadlock_exceptuando_al_siguiente(procesos, necesidadActual, disponiblesActuales, procesosOmitidos, enDeadlock)
			     || hay_deadlock_dejando_al_siguiente(procesos, necesidadActual, disponiblesActuales, procesosOmitidos, enDeadlock);

	list_add_in_index(procesos, 0, unEntrenador);

	recursos_destroy(necesidadActual);
	recursos_destroy(disponiblesActuales);
	list_destroy(aEvaluar);

	return deadlock;
}

bool hay_deadlock_dejando_al_siguiente(entrenadores unEquipo, matriz_recursos necesidad, matriz_recursos disponibles, entrenadores omitidos, entrenadores enDeadlock){

	entrenadores procesos = unEquipo;//esAca list_duplicate(unEquipo);
	entrenadores procesosOmitidos = list_duplicate(omitidos);

	entrenador*unEntrenador = list_remove(procesos, 0);
	list_add(procesosOmitidos, unEntrenador);

	if(!unEntrenador){
//		puts("D-NULL");
		return false;
	}

	printf("D%d\n", unEntrenador->id);

	entrenadores aEvaluar = list_duplicate(procesos);
	list_add_all(aEvaluar, procesosOmitidos);

	bool esperaCircular = hay_espera_circular(aEvaluar, necesidad, disponibles);

	if(esperaCircular){
		list_add_all(enDeadlock, aEvaluar);
	}

	bool deadlock = esperaCircular
		|| hay_deadlock_exceptuando_al_siguiente(procesos, necesidad, disponibles, procesosOmitidos, enDeadlock)
		|| hay_deadlock_dejando_al_siguiente(procesos, necesidad, disponibles, procesosOmitidos, enDeadlock);


	//esAca
	list_add_in_index(procesos, 0, unEntrenador);

	return deadlock;
}

bool hay_espera_circular(entrenadores unEquipo, matriz_recursos necesidad,matriz_recursos disponibles){
//	printf("Necesidad:"); recursos_mostrar(necesidad); puts("");
//	printf("Disponibles:"); recursos_mostrar(disponibles);puts("");
	return recursos_suficientes_para(necesidad, disponibles) && list_size(unEquipo)>1;
}

////*************************************** Intercambios ************************************************//
//void algoritmo_procesar_deadlock(entrenadores unEquipo){
//	//TODO
//	puts("Te odio");
//}



//bool entrenador_puede_intercambiar_con(entrenador*unEntrenador, entrenador*otro){
//	//TODO
//
////	bool recursos_tienen_en_comun(matriz_recursos una, matriz_recursos otra){
////
////
////
////		return
////	}
//
//
//	return true;
//}
//
//entrenador* entrenadores_pareja_de_intercambio_para(entrenadores unEquipo, entrenador*unEntrenador){
//	bool esParejaDeIntercambio(void*otro){
//		return entrenador_puede_intercambiar_con(unEntrenador, otro);
//	}
//	return list_remove_by_condition(unEquipo, esParejaDeIntercambio);
//}
//
//void algoritmo_procesar_deadlock(entrenadores entrenadoresEnDeadlock){
//	while(!list_is_empty(entrenadoresEnDeadlock)){ //Limitar condicion en caso de que cantidad no sea par y al final vaya a quedar uno solo
//		entrenador*unEntrenador = list_remove(entrenadoresEnDeadlock, 0);
//
//		entrenador*otroEntrenador = entrenadores_pareja_de_intercambio_para(entrenadoresEnDeadlock, unEntrenador);
//
//		par_intercambio* parejaDeIntercambio = par_intercambio_create(unEntrenador, otroEntrenador);
//
//		entrenador_pasar_a(unEntrenador, READY, "Puede intercambiar figus con otro entrenador");
//		//Ver como agregar el id del otro, si vale la pena crear funcion especifica
//
//		list_add(intercambiosPendientes, parejaDeIntercambio);
//	}
//}


////*************************************** Auxiliares *********************************************
matriz_recursos entrenador_recursos_sobrantes(entrenador*unEntrenador){
	return recursos_matriz_diferencia_positiva(unEntrenador->pokemonesCazados, unEntrenador->objetivos);
}

//retorna los objetivos no cumplidos del entrenador, o sea, la "necesidad particular"
matriz_recursos entrenador_recursos_pedidos(entrenador*unEntrenador){
	return recursos_matriz_diferencia_positiva(unEntrenador->objetivos, unEntrenador->pokemonesCazados);
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

	void sumarInsatisfechos(entrenador*unEntrenador){
		matriz_recursos disponibles = entrenador_recursos_sobrantes(unEntrenador); //Se va a ir con nueva estructura
		recursos_sumar_recursos_a(sobrantes, disponibles);
		recursos_destroy(disponibles);
	}

	list_iterate(unEquipo, (void(*)(void*))&sumarInsatisfechos);

	return sobrantes;

}
