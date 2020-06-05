//#include "team.h"
//#include "hilos del team/hilos_team.h"
//
///*TODO
// * testear y ya pasar al hilo
// * cachear a los entrenadores en una estructura que tenga los recursos que necesita, los que le sobran y el id, para no tener que repetir varias veces las mismas cuentas y sacar algunos destroy
// * cambiar los nombres de las funciones para que sean mas acordes a los fines
// * En teoria los entrenadores que retornan, llegan en orden de ejecucion, con lo cual con que yo remueva dos en orden deberian poder intercambiar
// */
//
////	podria trabajar con las estructuras cacheadas de recursos TODO Modelo: REntrenador{entrenador*, necesidad, disponibles} mas facil y podria usar funcion map
//
//matriz_recursos entrenador_recursos_sobrantes(entrenador*unEntrenador){
//	return recursos_matriz_diferencia_positiva(unEntrenador->pokemonesCazados, unEntrenador->objetivos);
//}
//
////retorna los objetivos no cumplidos del entrenador, o sea, la "necesidad particular"
//matriz_recursos entrenador_recursos_pedidos(entrenador*unEntrenador){
//	return recursos_matriz_diferencia_positiva(unEntrenador->objetivos, unEntrenador->pokemonesCazados);
//}
//
////***************************************************************************
////A futuro TODO
//
//typedef t_list* cache_recursos;
//
//typedef struct{
//	entrenador*unEntrenador;
//	matriz_recursos necesidad;
//	matriz_recursos sobrantes;
//}recursos_cacheados;
//
//recursos_cacheados* cache_recursos_create(entrenador*unEntrenador){
//	recursos_cacheados* elemento = malloc(sizeof(recursos_cacheados));
//				  *elemento = (recursos_cacheados) {unEntrenador, entrenador_recursos_pedidos(unEntrenador), entrenador_recursos_sobrantes(unEntrenador)};
//	return elemento;
//}
//
//void cache_recursos_destroy(recursos_cacheados* elemento){
//	recursos_destroy(elemento->necesidad);
//	recursos_destroy(elemento->sobrantes);
//	free(elemento);
//}
//
////****************************************************************************
////Pasar a recursos.c
//
//// retorna una nueva matriz con la suma de ambas
//matriz_recursos recursos_matriz_suma(matriz_recursos unaMatriz, matriz_recursos otra){
//	matriz_recursos suma = recursos_duplicar(unaMatriz);
//	recursos_sumar_recursos_a(suma, otra);
//
//	return suma;
//}
//
////****************************************************************************
////implementar para la nueva estructura
////Retorna true si la matriz es suficiente para cumplir sus objetivos, sin considerar los pokemones que ya capturo (:()
//bool entrenador_completa_recursos(entrenador*unEntrenador, matriz_recursos recursosDisponibles){
//
//	matriz_recursos disponibles = recursos_matriz_suma(unEntrenador->pokemonesCazados, recursosDisponibles);
//
//	bool completa = recursos_suficientes_para(disponibles, unEntrenador->objetivos);
//
//	recursos_destroy(disponibles);
//	return completa;
//}
//
////Retorna al proximo que cumpla la condicion de completa_recursos
//entrenador* entrenadores_proximo_que_complete_objetivos_con(matriz_recursos recursos, entrenadores unEquipo){
//	bool completaRecursos(void* unEntrenador){
//		printf("Analizo al entrenador N°%u | ", ((entrenador*)unEntrenador)->id); //YO no libero los mismos que ocupo
//
//		return entrenador_completa_recursos((entrenador*)unEntrenador, recursos);
//	}
//
//	return list_remove_by_condition(unEquipo, &completaRecursos);
//}
//
////************************************************************************************
////Controles previos a aplicar algoritmo. La idea por ahora es usar una lista aparte. Ver como aplicar
//entrenadores entrenadores_con_recursos_en_exceso(entrenadores unEquipo){
//	return entrenadores_en_estado(equipo, LOCKED_HASTA_DEADLOCK);
//}
//
////Ordena a los entrenadores segun la cantidad de recursos que poseen
//void entrenadores_ordenar_por_cantidad_de_recursos(entrenadores unEquipo){
//	bool entrenador_tiene_menos_asignados_que(void*unEntrenador, void*otro){
//			matriz_recursos asignadosDel1ero = ((entrenador*)unEntrenador)->pokemonesCazados;
//			matriz_recursos asignadosDel2do = ((entrenador*)otro)->pokemonesCazados;
//
//			bool tieneMenos = recursos_contar(asignadosDel1ero) <= recursos_contar(asignadosDel2do);
//
//			return tieneMenos;
//		}
//
//
//	list_sort(unEquipo, &entrenador_tiene_menos_asignados_que);
//
//}
//
////*************************************************************************** FIN FUNCIONES AUXILIARES
////Cosas importantes:
//
//bool hay_deadlock_si_ejecuto_al_proximo(entrenadores enRevision, entrenadores ejecutadosAnteriores, matriz_recursos pedidosAnteriores, matriz_recursos disponiblesAnteriores, entrenadores enEsperaCircular);
//
//bool hay_deadlock_si_lo_salteo_y_ejecuto_al_siguiente(entrenadores sinEjecutar, entrenadores yaEjecutados, matriz_recursos necesidadActual, matriz_recursos disponiblesActuales, entrenadores enEsperaCircular){
//	puts("--------------------------------me arrepenti, lo salteo");
//	return hay_deadlock_si_ejecuto_al_proximo(sinEjecutar, yaEjecutados, necesidadActual, disponiblesActuales, enEsperaCircular);
//}
//
//bool hay_deadlock_si_ejecuto_al_proximo(entrenadores enRevision, entrenadores ejecutadosAnteriores, matriz_recursos pedidosAnteriores, matriz_recursos disponiblesAnteriores, entrenadores enEsperaCircular){
//
//	entrenadores sinEjecutar = list_duplicate(enRevision);
//	entrenadores yaEjecutados = list_duplicate(ejecutadosAnteriores);
//
//	matriz_recursos necesidadActual = recursos_duplicar(pedidosAnteriores);
//	matriz_recursos disponiblesActuales = recursos_duplicar(disponiblesAnteriores);
//
//	entrenador*unEntrenador = entrenadores_proximo_que_complete_objetivos_con(disponiblesActuales, sinEjecutar);
//
//		if(!unEntrenador){
//			puts("No se pueden ejecutar mas entrenadores");
//
//			recursos_destroy(necesidadActual);
//			recursos_destroy(disponiblesActuales);
//			list_destroy(sinEjecutar);
//			list_destroy(yaEjecutados);
//
//			enEsperaCircular = list_duplicate(ejecutadosAnteriores);
//
//			return recursos_suficientes_para(disponiblesAnteriores, pedidosAnteriores) && !list_is_empty(ejecutadosAnteriores);
//		}
//
//		matriz_recursos necesidadDelEntrenador = entrenador_recursos_pedidos(unEntrenador);
//		matriz_recursos recursosSobrantes      = entrenador_recursos_sobrantes(unEntrenador);
//		//a futuro cacheados
//
//		printf("necesita:"); recursos_mostrar(necesidadDelEntrenador);
//		printf(" y le sobran:"); recursos_mostrar(recursosSobrantes); puts("");
//
//		recursos_restar_recursos_a(disponiblesActuales, necesidadDelEntrenador);
//		recursos_sumar_recursos_a(disponiblesActuales, recursosSobrantes);
//
//		printf("Ejecuto al entrenador N°%u | ", unEntrenador->id);
//		list_add(yaEjecutados, unEntrenador);
//
//	//d=deuda
//	printf("Necesidad Actual + d :"); recursos_mostrar(necesidadActual); puts("");
//	printf("                          | Disponibles Actuales :"); recursos_mostrar(disponiblesActuales); puts("");
//
//
//	bool seCerroLaEsperaCircular = (recursos_suficientes_para(disponiblesActuales, necesidadActual) && !list_is_empty(yaEjecutados));
//
//	bool hayDeadlock = seCerroLaEsperaCircular
//				|| hay_deadlock_si_ejecuto_al_proximo(sinEjecutar, yaEjecutados, necesidadActual, disponiblesActuales, enEsperaCircular)
//				|| hay_deadlock_si_lo_salteo_y_ejecuto_al_siguiente(sinEjecutar, ejecutadosAnteriores, pedidosAnteriores, disponiblesAnteriores, enEsperaCircular);
//
//	recursos_destroy(necesidadDelEntrenador);
//	recursos_destroy(necesidadActual);
//	recursos_destroy(disponiblesActuales);
//	recursos_destroy(recursosSobrantes);
//	list_destroy(sinEjecutar);
//
//	if(seCerroLaEsperaCircular){
//		list_add_all(enEsperaCircular, yaEjecutados);
//	} else{
//		list_destroy(yaEjecutados);
//	}
//
//
//	return hayDeadlock;
//}
//
//
//bool algoritmo_detectar_deadlock(entrenadores unEquipo, entrenadores enDeadlock){
//	entrenadores entrenadoresEnRevision = list_duplicate(unEquipo);//entrenadores_con_recursos_en_exceso(unEquipo);
////	entrenadores_ordenar_por_cantidad_de_recursos(entrenadoresEnRevision);
//
//
////	entrenadoresEnRevision = list_map(entrenadoresEnRevision, (void(*)(void*)) &cache_deadlock_create); TODO
//
//	//**************************************** hasta aca fue preparacion de los entrenadores pre-algoritmo
//
//	int i, cantidad=list_size(entrenadoresEnRevision);
//	bool hayDeadlock = false;
//	entrenadores entrenadoresRevisados = entrenadores_create();
//
//	for(i=0; i<cantidad && !hayDeadlock; i++){
//		entrenador*unEntrenador = list_get(entrenadoresEnRevision, i);
//		printf("\n**************************************** INICIO ALGORITMO con Vd = Vn<%u>\n", unEntrenador->id);
//
//		matriz_recursos recursosDisponibles = entrenador_recursos_pedidos(unEntrenador);
//		matriz_recursos recursosEndeudados  = recursos_duplicar(recursosDisponibles);
//
//		printf("Deuda Actual:"); recursos_mostrar(recursosEndeudados); puts("");
//		printf("Disponibles:");  recursos_mostrar(recursosDisponibles); puts("");
//
//		hayDeadlock = hay_deadlock_si_ejecuto_al_proximo(entrenadoresEnRevision, entrenadoresRevisados, recursosEndeudados, recursosDisponibles, enDeadlock);
//
//		recursos_destroy(recursosDisponibles);
//		recursos_destroy(recursosEndeudados);
//
//		printf("\n**************************************** FIN ALGORITMO %u\n", unEntrenador->id);
//	}
//
//	list_destroy(entrenadoresEnRevision);
//	list_destroy(entrenadoresRevisados);
//
//	return hayDeadlock;
//}
//
////******************************************************************************************** programa principal de prueba
//int main(){
//
////int mainDePruebaDeadlock(){
//
//												//asignados, pedidos
//		entrenador*a = entrenador_ptr_crear(4, "a|d", "b|e", posicion_create(1,1));
//		entrenador*b = entrenador_ptr_crear(5, "c|e", "a|f", posicion_create(1,1));
//		entrenador*c = entrenador_ptr_crear(6, "b|f", "c|d", posicion_create(1,1));
////		entrenador*d = entrenador_ptr_crear(7, "d|f|c", "a|b|c", posicion_create(1,1));
//
//
////	entrenador*otroMas        = entrenador_ptr_crear(0, "a|b|c", "b|a|a", posicion_create(1,1));
////	entrenador*otroOtroMas    = entrenador_ptr_crear(1, "b|c|c", "c|c|e", posicion_create(1,1));
////	entrenador*unEntrenador   = entrenador_ptr_crear(2, "c|e|e", "c|e|b", posicion_create(1,1));
////	entrenador*otroEntrenador = entrenador_ptr_crear(3, "d|f|c", "d|f|t", posicion_create(1,1));
//
//	equipo = entrenadores_create();
//
//	list_add(equipo, a);
//	list_add(equipo, b);
//	list_add(equipo, c);
////	list_add(equipo, d);
//
////	list_add(equipo, otroMas);
////	list_add(equipo, unEntrenador);
////	list_add(equipo, otroEntrenador);
////	list_add(equipo, otroOtroMas);
//
//	int i=0, cant=list_size(equipo);
//	printf("Pedidos:\n");
//	for(; i<cant; i++){
//		entrenador*pochi = list_get(equipo, i);
//		recursos_mostrar(pochi->objetivos);puts("");
//	}
//
//	puts("\n Recursos necesitados (por entrenador):");
//	for(i=0; i<cant; i++){
//		entrenador*ash = list_get(equipo, i);
//
//		matriz_recursos necesitados = recursos_matriz_diferencia_positiva(ash->objetivos, ash->pokemonesCazados);
//		printf("%u: ", i);recursos_mostrar(necesitados); puts("");
//		recursos_destroy(necesitados);
//	}
//
//	puts("\n Recursos sobrantes:");
//	for(i=0; i<cant; i++){
//		entrenador*ash = list_get(equipo, i);
//
//		matriz_recursos sobrantes = entrenador_recursos_sobrantes(ash);
//		printf("%u: ", i);recursos_mostrar(sobrantes); puts("");
//		recursos_destroy(sobrantes);
//	}
//
//	entrenadores enDeadlock = entrenadores_create();
//
//	if(algoritmo_detectar_deadlock(equipo, enDeadlock)){
//		if(enDeadlock){
//			puts("llego hasta aca");
//			void mostrar_deadlock(entrenador*u){
//				printf("\nEntrenador N°%d:\n	Objetivos:   ", u->id); recursos_mostrar(u->objetivos);
//								  printf("\n    Disponibles: "); recursos_mostrar(u->pokemonesCazados); puts("");
//			}
//
//			list_iterate(enDeadlock, (void(*)(void*)) &mostrar_deadlock);
//		}
//	}
//
//	else puts("\nNOOO");
//
//	entrenadores_destroy(equipo);
//	list_destroy(enDeadlock);
//
//	return 0;
//}
