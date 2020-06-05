#include "team.h"
#include "hilos del team/hilos_team.h"

/*TODO
 * testear y ya pasar al hilo
 * cachear a los entrenadores en una estructura que tenga los recursos que necesita, los que le sobran y el id, para no tener que repetir varias veces las mismas cuentas y sacar algunos destroy
 * cambiar los nombres de las funciones para que sean mas acordes a los fines
 * En teoria los entrenadores que retornan, llegan en orden de ejecucion, con lo cual con que yo remueva dos en orden deberian poder intercambiar
 */

//	podria trabajar con las estructuras cacheadas de recursos TODO Modelo: REntrenador{entrenador*, necesidad, disponibles} mas facil y podria usar funcion map

matriz_recursos entrenador_recursos_sobrantes(entrenador*unEntrenador){
	return recursos_matriz_diferencia_positiva(unEntrenador->pokemonesCazados, unEntrenador->objetivos);
}

//retorna los objetivos no cumplidos del entrenador, o sea, la "necesidad particular"
matriz_recursos entrenador_recursos_pedidos(entrenador*unEntrenador){
	return recursos_matriz_diferencia_positiva(unEntrenador->objetivos, unEntrenador->pokemonesCazados);
}

//****************************************************************************
//implementar para la nueva estructura
//Retorna true si la matriz es suficiente para cumplir sus objetivos, sin considerar los pokemones que ya capturo (:()
bool entrenador_completa_recursos(entrenador*unEntrenador, matriz_recursos recursosDisponibles){

	matriz_recursos disponibles = recursos_matriz_suma(unEntrenador->pokemonesCazados, recursosDisponibles);

	bool completa = recursos_suficientes_para(disponibles, unEntrenador->objetivos);

	recursos_destroy(disponibles);
	return completa;
}

//************************************************************************************
//Controles previos a aplicar algoritmo. La idea por ahora es usar una lista aparte. Ver como aplicar
entrenadores entrenadores_con_recursos_en_exceso(entrenadores unEquipo){
	return entrenadores_en_estado(equipo, LOCKED_HASTA_DEADLOCK);
}

//Ordena a los entrenadores segun la cantidad de recursos que poseen
void entrenadores_ordenar_por_cantidad_de_recursos(entrenadores unEquipo){
	bool entrenador_tiene_menos_asignados_que(void*unEntrenador, void*otro){
			matriz_recursos asignadosDel1ero = ((entrenador*)unEntrenador)->pokemonesCazados;
			matriz_recursos asignadosDel2do = ((entrenador*)otro)->pokemonesCazados;

			bool tieneMenos = recursos_contar(asignadosDel1ero) <= recursos_contar(asignadosDel2do);

			return tieneMenos;
		}


	list_sort(unEquipo, &entrenador_tiene_menos_asignados_que);

}

//*************************************************************************** FIN FUNCIONES AUXILIARES
//NUEVO

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

//*************************************************************************************

bool hay_espera_circular(entrenadores unEquipo, matriz_recursos necesidad,matriz_recursos disponibles){
//	printf("Necesidad:"); recursos_mostrar(necesidad); puts("");
//	printf("Disponibles:"); recursos_mostrar(disponibles);puts("");
	return recursos_suficientes_para(necesidad, disponibles) && list_size(unEquipo)>1;
}

bool hay_deadlock_exceptuando_al_siguiente(entrenadores unEquipo, matriz_recursos necesidad, matriz_recursos disponibles, entrenadores enDeadlock);

bool hay_deadlock_dejando_al_siguiente(entrenadores unEquipo, matriz_recursos necesidad, matriz_recursos disponibles, entrenadores enDeadlock){

	entrenadores procesos = unEquipo;//esAca list_duplicate(unEquipo);
	entrenadores procesosEnDeadlock = list_duplicate(enDeadlock);

	entrenador*unEntrenador = list_remove(procesos, 0);
	list_add(procesosEnDeadlock, unEntrenador);

	if(!unEntrenador){
//		puts("D-NULL");
		return false;
	}

	printf("D%d\n", unEntrenador->id);

	entrenadores aEvaluar = list_duplicate(procesos);
	list_add_all(aEvaluar, procesosEnDeadlock);

	bool deadlock = hay_espera_circular(aEvaluar, necesidad, disponibles)
		|| hay_deadlock_exceptuando_al_siguiente(procesos, necesidad, disponibles, procesosEnDeadlock);

	if(!deadlock){
		printf("********retracto N°%d\n", unEntrenador->id);
		deadlock = deadlock || hay_deadlock_dejando_al_siguiente(procesos, necesidad, disponibles, procesosEnDeadlock);
	}

	//esAca
	list_add_in_index(procesos, 0, unEntrenador);

	return deadlock;
}

bool hay_deadlock_exceptuando_al_siguiente(entrenadores unEquipo, matriz_recursos necesidad, matriz_recursos disponibles, entrenadores enDeadlock){

	entrenadores procesos = unEquipo;
	entrenadores procesosEnDeadlock = list_duplicate(enDeadlock);

	entrenador*unEntrenador = list_remove(procesos, 0);

	entrenadores aEvaluar = list_duplicate(procesos);
	list_add_all(aEvaluar, procesosEnDeadlock);

	if(!unEntrenador){
		puts("L-NULL");
		return false;
	}

	printf("L%d\n", unEntrenador->id);

	matriz_recursos necesidadActual = recursos_matriz_diferencia(necesidad, entrenador_recursos_pedidos(unEntrenador));
	matriz_recursos disponiblesActuales = recursos_matriz_diferencia(disponibles, entrenador_recursos_sobrantes(unEntrenador));

	bool deadlock = hay_espera_circular(aEvaluar, necesidadActual, disponiblesActuales)
		|| hay_deadlock_exceptuando_al_siguiente(procesos, necesidadActual, disponiblesActuales, procesosEnDeadlock)
		|| hay_deadlock_dejando_al_siguiente(procesos, necesidadActual, disponiblesActuales, procesosEnDeadlock);

	list_add_in_index(procesos, 0, unEntrenador);

	recursos_destroy(necesidadActual);
	recursos_destroy(disponiblesActuales);

	return deadlock;
}


bool algoritmo_detectar_deadlock(entrenadores unEquipo, entrenadores enDeadlock){

	matriz_recursos necesidad = entrenadores_pedidos_insatisfechos(unEquipo);
	matriz_recursos disponibles = entrenadores_recursos_sobrantes(unEquipo);

	bool hayDeadlock = hay_espera_circular(unEquipo, necesidad, disponibles)
		|| hay_deadlock_exceptuando_al_siguiente(unEquipo, necesidad, disponibles, enDeadlock)
		|| hay_deadlock_dejando_al_siguiente(unEquipo, necesidad, disponibles, enDeadlock);

	recursos_destroy(necesidad);
	recursos_destroy(disponibles);

	return hayDeadlock;
}

//******************************************************************************************** programa principal de prueba
int main(){

//int mainDePruebaDeadlock(){

//	entrenador*a = entrenador_ptr_crear(0, "a|b", "c|d", posicion_create(1,1));
//	entrenador*b = entrenador_ptr_crear(1, "c|d", "d|e", posicion_create(1,1));
//	entrenador*c = entrenador_ptr_crear(2, "e|f", "f|g", posicion_create(1,1));

	//asignados, pedidos
//		entrenador*d = entrenador_ptr_crear(0, "a|d", "b|e", posicion_create(1,1));
//		entrenador*e = entrenador_ptr_crear(1, "c|e", "a|f", posicion_create(1,1));
//		entrenador*f = entrenador_ptr_crear(2, "b|f", "c|d", posicion_create(1,1));
//		entrenador*g = entrenador_ptr_crear(3, "d|f|c", "a|b|c", posicion_create(1,1));


	entrenador*h        = entrenador_ptr_crear(0, "a|b|c", "b|a|a", posicion_create(1,1));
	entrenador*w    = entrenador_ptr_crear(1, "b|c|c", "c|c|e", posicion_create(1,1));
	entrenador*j   = entrenador_ptr_crear(2, "c|e|e", "c|e|b", posicion_create(1,1));
	entrenador*k = entrenador_ptr_crear(3, "d|f|c", "d|f|t", posicion_create(1,1));

	equipo = entrenadores_create();

//	list_add(equipo, a);
//	list_add(equipo, b);
//	list_add(equipo, c);
	list_add(equipo, h);
	list_add(equipo, w);
	list_add(equipo, j);
	list_add(equipo, k);
//	list_add(equipo, h);


//	list_add(equipo, otroMas);
//	list_add(equipo, unEntrenador);
//	list_add(equipo, otroEntrenador);
//	list_add(equipo, otroOtroMas);

	int i=0, cant=list_size(equipo);
	printf("Pedidos:\n");
	for(; i<cant; i++){
		entrenador*pochi = list_get(equipo, i);
		recursos_mostrar(pochi->objetivos);puts("");
	}

	puts("\n Recursos necesitados (por entrenador):");
	for(i=0; i<cant; i++){
		entrenador*ash = list_get(equipo, i);

		matriz_recursos necesitados = recursos_matriz_diferencia_positiva(ash->objetivos, ash->pokemonesCazados);
		printf("%u: ", i);recursos_mostrar(necesitados); puts("");
		recursos_destroy(necesitados);
	}

	puts("\n Recursos sobrantes:");
	for(i=0; i<cant; i++){
		entrenador*ash = list_get(equipo, i);

		matriz_recursos sobrantes = entrenador_recursos_sobrantes(ash);
		printf("%u: ", i);recursos_mostrar(sobrantes); puts("");
		recursos_destroy(sobrantes);
	}

	entrenadores enDeadlock = entrenadores_create();

	if(algoritmo_detectar_deadlock(equipo, enDeadlock)){
		if(enDeadlock){
			puts("Si hay, llego hasta aca");
			void mostrar_deadlock(entrenador*u){
				printf("\nEntrenador N°%d:\n	Objetivos:   ", u->id); recursos_mostrar(u->objetivos);
								  printf("\n    Disponibles: "); recursos_mostrar(u->pokemonesCazados); puts("");
			}

			list_iterate(enDeadlock, (void(*)(void*)) &mostrar_deadlock);
		}
	}

	else puts("\nNOOO");

	entrenadores_destroy(equipo);
	list_destroy(enDeadlock);

	return 0;
}
