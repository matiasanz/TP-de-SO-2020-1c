#include "team.h"
#include "hilos del team/hilos_team.h"

matriz_recursos entrenador_recursos_sobrantes(entrenador*unEntrenador){
	return recursos_matriz_diferencia_positiva(unEntrenador->pokemonesCazados, unEntrenador->objetivos);
}

matriz_recursos entrenador_recursos_faltantes(entrenador*unEntrenador){
	return recursos_matriz_diferencia_positiva(unEntrenador->objetivos, unEntrenador->pokemonesCazados);
}

matriz_recursos recursos_matriz_suma(matriz_recursos unaMatriz, matriz_recursos otra){
	matriz_recursos suma = recursos_duplicar(unaMatriz);
	recursos_sumar_recursos_a(suma, otra);

	return suma;
}

//***************************************************************************

bool objetivos_cumplidos(matriz_recursos objetivos, matriz_recursos disponibles){
	matriz_recursos diferencia = recursos_matriz_diferencia(objetivos, disponibles); //NO PUEDE CAZAR MAS DE LOS QUE NECESITA .'. si o si tiene que ser nula
	bool seCumplieron = recursos_matriz_nula(diferencia);
	recursos_destroy(diferencia);
	return seCumplieron;
}

matriz_recursos entrenador_recursos_asignados(entrenador*unEntrenador){
	return recursos_matriz_diferencia(((entrenador*)unEntrenador)->pokemonesCazados, entrenador_recursos_sobrantes(unEntrenador));
}

matriz_recursos entrenadores_recursos_asignados(entrenadores unEquipo){
	matriz_recursos totalAsignados = recursos_create();

	void entrenador_sumar_recursos_asignados(void*unEntrenador){
		matriz_recursos asignados = recursos_matriz_diferencia(((entrenador*)unEntrenador)->pokemonesCazados, entrenador_recursos_sobrantes(unEntrenador));
		recursos_sumar_recursos_a(totalAsignados, asignados);
		recursos_destroy(asignados);
	}

	list_iterate(unEquipo, &entrenador_sumar_recursos_asignados);

	return totalAsignados;
}

bool entrenador_completa_recursos(entrenador*unEntrenador, matriz_recursos recursosDisponibles){

	matriz_recursos faltantes = entrenador_recursos_faltantes(unEntrenador);

	bool completa = recursos_suficientes_para(faltantes, recursosDisponibles);

	recursos_destroy(faltantes);
	return completa;
}

//***************************************************************************************

bool hay_deadlock_si_ejecuto_al_proximo(matriz_recursos objetivos, matriz_recursos disponibles, entrenadores unEquipo){
	entrenadores duplicada = list_duplicate(unEquipo);

	int cantidad = list_size(unEquipo);

	if(!cantidad){ //caso base
		return false;
	}

	else{
		matriz_recursos necesidadActual = recursos_duplicar(objetivos);
		matriz_recursos disponiblesActuales = recursos_duplicar(disponibles);

		bool completaRecursos(void* unEntrenador){
			return entrenador_completa_recursos((entrenador*)unEntrenador, disponiblesActuales);
		}

		entrenador*unEntrenador = list_remove_by_condition(duplicada, &completaRecursos);

		if(!unEntrenador){
			puts("No se pueden ejecutar mas entrenadores");
			return false; //list_is_empty(duplicada); //DUDA, reemplazaria al caso base?
		}

		matriz_recursos necesidadDelEntrenador = entrenador_recursos_faltantes(unEntrenador);
		matriz_recursos recursosSobrantes      = entrenador_recursos_sobrantes(unEntrenador);

		printf("Ejecuto al entrenador N°%u             | ", unEntrenador->id); //YO no libero los mismos que ocupo

		printf("necesita:"); recursos_mostrar(necesidadDelEntrenador);
		printf(" y le sobran:"); recursos_mostrar(recursosSobrantes); puts("");


		recursos_restar_recursos_a(necesidadActual, necesidadDelEntrenador);
		recursos_sumar_recursos_a(disponiblesActuales, recursosSobrantes);

		printf("Si ejecuto y libero al entrenador N°%u | ", unEntrenador->id); //YO no libero los mismos que ocupo

		printf("Necesidad   Actual   :"); recursos_mostrar(necesidadActual);
		printf("Disponibles Actuales :"); recursos_mostrar(disponiblesActuales); puts("");


		bool retorno = recursos_matriz_nula(necesidadActual);
			 retorno = retorno || hay_deadlock_si_ejecuto_al_proximo(necesidadActual, disponiblesActuales, duplicada);

	    recursos_destroy(necesidadDelEntrenador);
		recursos_destroy(necesidadActual);
		recursos_destroy(disponiblesActuales);
		recursos_destroy(recursosSobrantes);
		list_destroy(duplicada);

		return retorno;
 	}
}

bool hay_deadlock_si_libero_al_proximo(entrenadores entrenadoresEnRevision, matriz_recursos pedidosTotales, matriz_recursos recursosAsignados, matriz_recursos recursosDisponibles){

	entrenadores duplicados = list_duplicate(entrenadoresEnRevision);
	matriz_recursos necesidad         = recursos_matriz_diferencia_positiva(pedidosTotales, recursosAsignados);

	numero i=0, cantidad = list_size(duplicados);
	bool hayDeadlock = false;

	for(i=0; i<cantidad && !hayDeadlock; i++){
		puts("\n********************************** INICIO");

		printf("Pedidos Totales      :"); recursos_mostrar(pedidosTotales);puts("");
		printf("Recursos Asignados   :"); recursos_mostrar(recursosAsignados); puts("");
		printf("Necesidad            :"); recursos_mostrar(necesidad); puts("\n");

		entrenador*unEntrenador = list_remove(duplicados, 0);
		printf("Si libero al entrenador N°%u           | ", unEntrenador->id);

		matriz_recursos pedidosDelEntrenador = entrenador_recursos_faltantes(unEntrenador);
		matriz_recursos recursosSobrantesDelEntrenador = entrenador_recursos_sobrantes(unEntrenador);
		matriz_recursos recursosAsignadosAlEntrenador = entrenador_recursos_asignados(unEntrenador);

		matriz_recursos necesidadCondicional = recursos_matriz_diferencia(necesidad, pedidosDelEntrenador);
		matriz_recursos asignadosCondicionales = recursos_matriz_diferencia(recursosAsignados, recursosAsignadosAlEntrenador);
		matriz_recursos recursosDisponiblesCondicionales = recursos_matriz_suma(recursosDisponibles, recursosSobrantesDelEntrenador);

		printf("Necesidad   Actual   :"); recursos_mostrar(necesidadCondicional);
		printf("Disponibles Actuales :"); recursos_mostrar(recursosDisponibles); puts("");

		hayDeadlock = !list_is_empty(duplicados) &&
								  (hay_deadlock_si_ejecuto_al_proximo(necesidadCondicional, recursosDisponiblesCondicionales, duplicados)
					|| hay_deadlock_si_libero_al_proximo(duplicados, necesidadCondicional, asignadosCondicionales, recursosDisponiblesCondicionales));

		//bloqueados y pokemon en exceso: lista no vacia
		//espera circular: espera circular

		list_add(duplicados, unEntrenador);
		recursos_destroy(pedidosDelEntrenador);
		recursos_destroy(recursosAsignadosAlEntrenador);
		recursos_destroy(recursosSobrantesDelEntrenador);
		recursos_destroy(necesidadCondicional);
		recursos_destroy(asignadosCondicionales);
		recursos_destroy(recursosDisponiblesCondicionales);
		puts("\n********************************** FIN");
	}


	recursos_destroy(necesidad);
	list_destroy(duplicados);

	return hayDeadlock;
}

bool algoritmo_detectar_deadlock(entrenadores unEquipo){ //agregar parametro entrenadores en deadlock y que me lo retorne para que desp lo pueda trabajar

//	entrenadores bloqueadosConExceso = entrenadores_en_estado(unEquipo, LOCKED_HASTA_DEADLOCK); cambiar por entrenadoresEnRevision

	bool noEsFilaDeCeros(void*unEntrenador){
		matriz_recursos necesidad = entrenador_recursos_faltantes(unEntrenador);
		bool esFilaDeCeros = recursos_matriz_nula(necesidad);
		recursos_destroy(necesidad);

		if(esFilaDeCeros){
			printf("\n>> Se descarto de la revision al entrenador N°%u dado que no requiere mas recursos", ((entrenador*)unEntrenador)->id);
		}

		return !esFilaDeCeros;
	}

	entrenadores entrenadoresEnRevision = list_filter(unEquipo, &noEsFilaDeCeros);

	bool entrenador_tiene_menos_asignados_que(void*unEntrenador, void*otro){
		matriz_recursos asignadosDel1ero = entrenador_recursos_asignados(unEntrenador);
		matriz_recursos asignadosDel2do = entrenador_recursos_asignados(otro);

		bool tieneMenos = recursos_contar(asignadosDel1ero) <= recursos_contar(asignadosDel2do);

		recursos_destroy(asignadosDel1ero);
		recursos_destroy(asignadosDel2do);

		return tieneMenos;
	}

	list_sort(entrenadoresEnRevision, &entrenador_tiene_menos_asignados_que);

	matriz_recursos pedidosTotales    = entrenadores_objetivos_globales(entrenadoresEnRevision);
	matriz_recursos recursosAsignados = entrenadores_recursos_asignados(entrenadoresEnRevision);
	matriz_recursos recursosDisponibles = recursos_create();

	bool hayDeadlock = hay_deadlock_si_libero_al_proximo(entrenadoresEnRevision, pedidosTotales, recursosAsignados, recursosDisponibles);

	recursos_destroy(pedidosTotales);
	recursos_destroy(recursosAsignados);

	return hayDeadlock;
}

int main(){

	matriz_recursos mr1 = recursos_from_string("a|b");
	matriz_recursos mr2 = recursos_from_string("c|d");
	matriz_recursos mr3 = recursos_from_string("e|f");
	matriz_recursos mr4 = recursos_from_string("g|h");
													//asignados, pedidos
	entrenador*unEntrenador   = entrenador_ptr_create(0, mr1, mr2, posicion_create(1,1));
	entrenador*otroEntrenador = entrenador_ptr_create(1, mr1, mr2, posicion_create(1,1));
	entrenador*otroMas        = entrenador_ptr_create(2, mr1, mr2, posicion_create(1,1));
	entrenador*otroOtroMas    = entrenador_ptr_create(3, mr3, mr4, posicion_create(1,1));

	equipo = entrenadores_create();

	recursos_agregar_N_instancias_de(mr1, "a", 0);
	list_add(equipo, unEntrenador);
	list_add(equipo, otroEntrenador);
	list_add(equipo, otroMas);
	list_add(equipo, otroOtroMas);

	int i=0, cant=list_size(equipo);
	printf("Pedidos:\n");
	for(; i<cant; i++){
		entrenador*pochi = list_get(equipo, i);
		recursos_mostrar(pochi->objetivos);puts("");
	}

	puts("\n Recursos asignados:");
	for(i=0; i<cant; i++){
		entrenador*ash = list_get(equipo, i);

		printf("%u: ", i); matriz_recursos asignados = entrenador_recursos_asignados(ash);
		recursos_mostrar(asignados); puts("");
		recursos_destroy(asignados);
	}

	puts("\n Recursos necesitados:");
	for(i=0; i<cant; i++){
		entrenador*ash = list_get(equipo, i);

		matriz_recursos necesitados = entrenador_recursos_faltantes(ash);
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

	if(algoritmo_detectar_deadlock(equipo)) puts("SIII");
	else puts("NOOO");

	recursos_destroy(mr1);
	recursos_destroy(mr2);

	return 0;
}
