#include "team.h"
#include "hilos del team/hilos_team.h"
#include "hilos del team/detector de deadlock.h"

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
