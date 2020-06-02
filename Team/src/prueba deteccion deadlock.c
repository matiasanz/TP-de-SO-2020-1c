#include "team.h"
#include "hilos del team/hilos_team.h"

/*TODO
 * cachear a los entrenadores en una estructura que tenga los recursos que necesita, los que le sobran y el id, para no tener que repetir varias veces las mismas cuentas y sacar algunos destroy
 * retornar de alguna manera (por ej por parametro) a los entrenadores que se encuentren en espera circular
 * Optimizar. Si ya sabe que A y B no funcan, por mas que agregue un tercero, no volver a analizarlos.
 * cambiar los nombres de las funciones para que sean mas acordes a los fines
 */

//Constructor de entrenador al cual le puedo pasar las matrices como cadenas de caracteres. Usar en adelante
entrenador*entrenador_ptr_crear(t_id id, char* asignados, char* pedidos, t_posicion unaPos){
	return entrenador_ptr_create(id, recursos_from_string(asignados), recursos_from_string(pedidos), unaPos);
}

//IMPORTANTE retorna los pokemones que el entrenador capturo pero no le sirven
matriz_recursos entrenador_recursos_sobrantes(entrenador*unEntrenador){
	return recursos_matriz_diferencia_positiva(unEntrenador->pokemonesCazados, unEntrenador->objetivos);
}

//IMPORTANTE retorna los objetivos del entrenador, o sea, la "necesidad particular"
matriz_recursos entrenador_recursos_pedidos(entrenador*unEntrenador){
	return recursos_duplicar(unEntrenador->objetivos);
//	return recursos_matriz_diferencia_positiva(unEntrenador->objetivos, unEntrenador->pokemonesCazados);
}

//IMPORTANTE retorna los pokemones que capturo el entrenador
matriz_recursos entrenador_recursos_asignados(entrenador*unEntrenador){
	return recursos_duplicar(unEntrenador->pokemonesCazados);
}



// retorna una nueva matriz con la suma de ambas
matriz_recursos recursos_matriz_suma(matriz_recursos unaMatriz, matriz_recursos otra){
	matriz_recursos suma = recursos_duplicar(unaMatriz);
	recursos_sumar_recursos_a(suma, otra);

	return suma;
}

// hace exactamente lo mismo que recursos_suficientes_para(...)
bool objetivos_cumplidos(matriz_recursos objetivos, matriz_recursos disponibles){
	matriz_recursos diferencia = recursos_matriz_diferencia(objetivos, disponibles); //NO PUEDE CAZAR MAS DE LOS QUE NECESITA .'. si o si tiene que ser nula
	bool seCumplieron = recursos_matriz_nula(diferencia);
	recursos_destroy(diferencia);
	return seCumplieron;
}

// Exactamente igual a inventarios_globales(), retorna los pokemones capturados por todos los entrenadores
matriz_recursos entrenadores_recursos_asignados(entrenadores unEquipo){
	matriz_recursos totalAsignados = recursos_create();

	void entrenador_sumar_recursos_asignados(void*unEntrenador){
		matriz_recursos asignados = entrenador_recursos_asignados(unEntrenador);
		recursos_sumar_recursos_a(totalAsignados, asignados);
		recursos_destroy(asignados);
	}

	list_iterate(unEquipo, &entrenador_sumar_recursos_asignados);

	return totalAsignados;
}


//Retorna true si la matriz es suficiente para cumplir sus objetivos, sin considerar los pokemones que ya capturo (:()
bool entrenador_completa_recursos(entrenador*unEntrenador, matriz_recursos recursosDisponibles){

	matriz_recursos faltantes = entrenador_recursos_pedidos(unEntrenador);

	bool completa = recursos_suficientes_para(recursosDisponibles, faltantes);

	recursos_destroy(faltantes);
	return completa;
}

//Retorna al proximo que cumpla la condicion de completa_recursos
entrenador* entrenadores_proximo_que_complete_objetivos_con(matriz_recursos recursos, entrenadores unEquipo){
	bool completaRecursos(void* unEntrenador){
		return entrenador_completa_recursos((entrenador*)unEntrenador, recursos);
	}

	return list_remove_by_condition(unEquipo, &completaRecursos);
}

//Retorna solamente a los entrenadores que hayan capturado pokemones (todos)
entrenadores entrenadores_con_recursos_asignados(entrenadores unEquipo){
	bool noEsFilaDeCeros(void*unEntrenador){
			bool esFilaDeCeros = recursos_matriz_nula(((entrenador*)unEntrenador)->pokemonesCazados);

			if(esFilaDeCeros){
				printf("\n>> Se descarto de la revision al entrenador N°%u dado que no requiere mas recursos", ((entrenador*)unEntrenador)->id);
			}

			return !esFilaDeCeros;
		}

	return list_filter(unEquipo, &noEsFilaDeCeros);
}

//IMPORTANTE Ordena a los entrenadores segun la cantidad de recursos que poseen
void entrenadores_ordenar_por_cantidad_de_recursos(entrenadores unEquipo){
	bool entrenador_tiene_menos_asignados_que(void*unEntrenador, void*otro){
			matriz_recursos asignadosDel1ero = entrenador_recursos_asignados(unEntrenador);
			matriz_recursos asignadosDel2do = entrenador_recursos_asignados(otro);

			bool tieneMenos = recursos_contar(asignadosDel1ero) <= recursos_contar(asignadosDel2do);

			recursos_destroy(asignadosDel1ero);
			recursos_destroy(asignadosDel2do);

			return tieneMenos;
		}


	list_sort(unEquipo, &entrenador_tiene_menos_asignados_que);

}

//*************************************************************************** FIN FUNCIONES AUXILIARES (no guiarse por los nombres)

bool hay_deadlock_si_libero_al_proximo(entrenadores entrenadoresEnRevision, entrenadores entrenadoresEjecutados, matriz_recursos necesidad, matriz_recursos recursosDisponibles);

//IMPORTANTISIMA
bool hay_deadlock_si_ejecuto_al_proximo(entrenadores unEquipo, entrenadores entrenadoresEjecutados, matriz_recursos pedidos, matriz_recursos disponibles){

	entrenadores sinEjecutar = list_duplicate(unEquipo);
	entrenadores yaEjecutados = list_duplicate(entrenadoresEjecutados);

	matriz_recursos necesidadActual = recursos_duplicar(pedidos);
	matriz_recursos disponiblesActuales = recursos_duplicar(disponibles);

	entrenador*unEntrenador = entrenadores_proximo_que_complete_objetivos_con(disponiblesActuales, sinEjecutar);

	if(!unEntrenador){
		puts("No se pueden ejecutar mas entrenadores");

		recursos_destroy(necesidadActual);
		recursos_destroy(disponiblesActuales);
		list_destroy(sinEjecutar);
		list_destroy(yaEjecutados);

		return recursos_suficientes_para(disponibles, pedidos) && !list_is_empty(yaEjecutados); //DUDA, reemplazaria al caso base?
	}

	matriz_recursos necesidadDelEntrenador = entrenador_recursos_pedidos(unEntrenador);
	matriz_recursos asignadosAlEntrenador  = entrenador_recursos_asignados(unEntrenador);
	matriz_recursos recursosSobrantes      = entrenador_recursos_sobrantes(unEntrenador);
	//a futuro cacheados


	printf("Analizo al entrenador N°%u | ", unEntrenador->id); //YO no libero los mismos que ocupo
	printf("necesita:"); recursos_mostrar(necesidadDelEntrenador);
	printf(" y le sobran:"); recursos_mostrar(recursosSobrantes); puts("");


	recursos_restar_recursos_a(necesidadActual, necesidadDelEntrenador);
	recursos_restar_recursos_a(disponiblesActuales, necesidadDelEntrenador);
	recursos_sumar_recursos_a(disponiblesActuales, recursosSobrantes);

	printf("Ejecuto al entrenador N°%u | ", unEntrenador->id); //YO no libero los mismos que ocupo
	list_add(yaEjecutados, unEntrenador);

	//d=deuda
	printf("Necesidad Actual + d :"); recursos_mostrar(necesidadActual); puts("");
	printf("                           | Disponibles Actuales :"); recursos_mostrar(disponiblesActuales); puts("");

	bool retorno = (recursos_suficientes_para(disponiblesActuales, necesidadActual) && !list_is_empty(yaEjecutados))
				|| hay_deadlock_si_ejecuto_al_proximo(sinEjecutar, yaEjecutados, necesidadActual, disponiblesActuales)
				|| hay_deadlock_si_libero_al_proximo(sinEjecutar, yaEjecutados, necesidadActual, disponiblesActuales);

	recursos_destroy(necesidadDelEntrenador);
	recursos_destroy(asignadosAlEntrenador);
	recursos_destroy(necesidadActual);
	recursos_destroy(disponiblesActuales);
	recursos_destroy(recursosSobrantes);
	list_destroy(sinEjecutar);
	list_destroy(yaEjecutados);

	return retorno;
}

bool hay_deadlock_si_libero_al_proximo(entrenadores entrenadoresEnRevision, entrenadores entrenadoresEjecutados, matriz_recursos necesidad, matriz_recursos recursosDisponibles){

	entrenadores sinEjecutar = list_duplicate(entrenadoresEnRevision);
	entrenadores yaEjecutados = list_duplicate(entrenadoresEjecutados);

	int i, cantidad = list_size(sinEjecutar);
	bool hayDeadlock = false;

	for(i=0; i<cantidad && !hayDeadlock; i++){
		entrenador*unEntrenador = list_remove(sinEjecutar, 0);

		printf("\n---------------------------------- INICIO SI LIBERO %u\n", unEntrenador->id);

		printf("Analizo al entrenador N°%u | ", unEntrenador->id); //YO no libero los mismos que ocupo
		matriz_recursos pedidosDelEntrenador = entrenador_recursos_pedidos(unEntrenador);
		printf("necesita:"); recursos_mostrar(pedidosDelEntrenador);
		matriz_recursos recursosSobrantesDelEntrenador = entrenador_recursos_sobrantes(unEntrenador);
		printf(" y le sobran:"); recursos_mostrar(recursosSobrantesDelEntrenador); puts("");


		printf("Estado actual                 | ");

		printf("Necesidad Actual + d :"); recursos_mostrar(necesidad); puts("\n");
		printf("                              | Disponibles:"); recursos_mostrar(recursosDisponibles); puts("");

		printf("Si libero al entrenador N°%u  | ", unEntrenador->id);

		matriz_recursos necesidadSiLibero = recursos_matriz_diferencia(necesidad, pedidosDelEntrenador);

		printf("Necesidad Actual + d :"); recursos_mostrar(necesidadSiLibero); puts("");
		printf("                             | Disponibles:"); recursos_mostrar(recursosDisponibles); puts("");

		hayDeadlock = (recursos_suficientes_para(recursosDisponibles, necesidadSiLibero) && !list_is_empty(yaEjecutados))
					|| hay_deadlock_si_ejecuto_al_proximo(sinEjecutar, yaEjecutados, necesidadSiLibero, recursosDisponibles)
					|| hay_deadlock_si_libero_al_proximo(sinEjecutar, yaEjecutados, necesidadSiLibero, recursosDisponibles);
																//necesidad y asignados??

		//bloqueados y pokemon en exceso: lista no vacia
		//espera circular: espera circular

		list_add(sinEjecutar, unEntrenador);//TODO ver si lo puedo sacar sin perder casos
		recursos_destroy(pedidosDelEntrenador);
		recursos_destroy(recursosSobrantesDelEntrenador);
		recursos_destroy(necesidadSiLibero);
		printf("\n---------------------------------- FIN SI LIBERO %d\n", unEntrenador->id);
	}

	list_destroy(sinEjecutar);
	list_destroy(yaEjecutados);

	return hayDeadlock;
}

bool algoritmo_detectar_deadlock(entrenadores unEquipo){ //agregar parametro entrenadores en deadlock y que me lo retorne para que desp lo pueda trabajar

//	entrenadores bloqueadosConExceso = entrenadores_en_estado(unEquipo, LOCKED_HASTA_DEADLOCK); cambiar por entrenadoresEnRevision
	entrenadores entrenadoresEnRevision = entrenadores_con_recursos_asignados(unEquipo); //En realidad es un caso absurdo por dos motivos: Del config no se pueden leer entrenadores sin recursos y por otro lado, la idea es partir de aquellos que no puedan cazar mas pokemones, pero loo dice la teoria.
	entrenadores_ordenar_por_cantidad_de_recursos(entrenadoresEnRevision);

	//**************************************** hasta aca fue preparacion de los entrenadores pre-algoritmo

	matriz_recursos pedidosTotales    = entrenadores_objetivos_globales(entrenadoresEnRevision);
	matriz_recursos recursosAsignados = entrenadores_recursos_asignados(entrenadoresEnRevision);

	int i, cantidad=list_size(entrenadoresEnRevision);
	bool hayDeadlock = false;


	for(i=0; i<cantidad && !hayDeadlock; i++){
		entrenador*unEntrenador = list_get(entrenadoresEnRevision, i);
		printf("\n**************************************** INICIO ALGORITMO con Vd = Vn<%u>\n", unEntrenador->id);

		matriz_recursos recursosDisponibles = entrenador_recursos_pedidos(unEntrenador);
		matriz_recursos pedidosMasDeuda     = recursos_matriz_suma(pedidosTotales, recursosDisponibles);
		entrenadores entrenadoresRevisados = list_create();

		printf("Necesidad Actual + d :"); recursos_mostrar(pedidosMasDeuda); puts("");
		printf("Disponibles:"); recursos_mostrar(recursosDisponibles); puts("");

		hayDeadlock = hay_deadlock_si_ejecuto_al_proximo(entrenadoresEnRevision, entrenadoresRevisados, pedidosMasDeuda, recursosDisponibles);

		puts("\n********************* intento fallido de ejecutar, se probara liberar");

		printf("Necesidad Actual + d :"); recursos_mostrar(pedidosMasDeuda); puts("");
		printf("Disponibles:"); recursos_mostrar(recursosDisponibles); puts("");


		hayDeadlock = hayDeadlock || hay_deadlock_si_libero_al_proximo(entrenadoresEnRevision, entrenadoresRevisados, pedidosMasDeuda, recursosDisponibles);

		recursos_destroy(recursosDisponibles);
		recursos_destroy(pedidosMasDeuda);
		list_destroy(entrenadoresRevisados);

		printf("\n**************************************** FIN ALGORITMO %u\n", unEntrenador->id);
	}

	recursos_destroy(pedidosTotales);
	recursos_destroy(recursosAsignados);
	list_destroy(entrenadoresEnRevision);

	return hayDeadlock;
}

//******************************************************************************************** programa principal de prueba
//int main(){

int mainDePruebaDeadlock(){

	//asignados, pedidos
	entrenador*otroMas        = entrenador_ptr_crear(0, "a", "b", posicion_create(1,1));
	entrenador*otroOtroMas    = entrenador_ptr_crear(1, "c", "a", posicion_create(1,1));
	entrenador*unEntrenador   = entrenador_ptr_crear(2, "x", "x", posicion_create(1,1));
	entrenador*otroEntrenador = entrenador_ptr_crear(3, "x", "x", posicion_create(1,1));

	equipo = entrenadores_create();

	list_add(equipo, otroMas);
	list_add(equipo, otroOtroMas);
	list_add(equipo, unEntrenador);
	list_add(equipo, otroEntrenador);

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

	if(algoritmo_detectar_deadlock(equipo)) puts("\nSIII");
	else puts("\nNOOO");

	entrenadores_destroy(equipo);

	return 0;
}
