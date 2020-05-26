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

matriz_recursos entrenador_objetivos(entrenador*unEntrenador){
	return unEntrenador->objetivos;
}

bool entrenador_cumplio_sus_objetivos(entrenador*unEntrenador){
	return recursos_suficientes_para(unEntrenador->pokemonesCazados, unEntrenador->objetivos);
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


void entrenador_pasar_a(entrenador*unEntrenador, t_estado estadoFinal, const char*motivo){
	char*estadoActual = estadoFromEnum(unEntrenador->estado);
	unEntrenador->estado = estadoFinal;

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El Entrenador N°%u se paso de la cola de %s a %s, Motivo: %s", unEntrenador->id, estadoActual, estadoFromEnum(estadoFinal), motivo);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);

	if(estadoFinal==EXIT) entrenador_destroy(unEntrenador);
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

entrenador* entrenadores_remover_del_equipo_a(entrenadores unEquipo, t_id id){
	bool entrenador_cmpId(entrenador*unEntrenador){
		return id == unEntrenador->id;
	}
	return list_remove_by_condition(unEquipo, (bool(*)(void*)) &entrenador_cmpId);
}

matriz_recursos entrenadores_objetivos_globales(entrenadores unEquipo){

	matriz_recursos objetivosGlobales = recursos_create();

	void agregarObjetivos(entrenador*unEntrenador){ //TODO no considera los que ya fueron capturados
		recursos_sumar_recursos_a(objetivosGlobales, unEntrenador->objetivos);
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

//***************************************************************************

matriz_recursos recursos_create(){
	return dictionary_create();
}

matriz_recursos recursos_from_string(char*cadena){

	if(!cadena){
		error_show("E de Error de lectura (y de estupido)");
	}

	matriz_recursos recursosLeidos = dictionary_create();

		void _a_la_matriz(especie_pokemon unaEspecie) {
			recursos_agregar_validado(recursosLeidos, unaEspecie);
		}

    especie_pokemon*tokens = string_split(cadena, "|");

    string_iterate_lines(tokens, _a_la_matriz);

//    void mostrar(t_posicion pos){
//    	printf("pos: (%u, %u)\n", pos.x, pos.y);
//    }

    free(tokens);
//    string_array_destroy(tokens); TODO ver antes rompia

    return recursosLeidos;
}

numero recursos_cantidad_de_instancias_de(matriz_recursos recursos, recurso unaEspecie){
	numero*cantidadDeInstancias = dictionary_get(recursos, unaEspecie);
	return cantidadDeInstancias? *cantidadDeInstancias : 0;
}

void recursos_agregar_recurso(matriz_recursos recursos, recurso unaEspecie){
	recursos_agregar_N_instancias_de(recursos, unaEspecie, 1);
}

void recursos_agregar_validado(matriz_recursos recursos, recurso unaEspecie){
	if(!unaEspecie){
		error_show("E de Error de lectura (y de estupido)");
		exit(1);
	}

	recursos_agregar_recurso(recursos, unaEspecie);
}

void recursos_agregar_N_instancias_de(matriz_recursos recursos, recurso unaEspecie, numero cantidad){
	numero* cantidadDeInstancias = dictionary_get(recursos, unaEspecie);

	if(cantidadDeInstancias){
		(*cantidadDeInstancias)+=cantidad;
	}
	else{
		cantidadDeInstancias = malloc(sizeof(numero));
		*cantidadDeInstancias = cantidad;
		dictionary_put(recursos, unaEspecie, cantidadDeInstancias);
	}
}

void recursos_destroy(matriz_recursos recursos){
	dictionary_destroy_and_destroy_elements(recursos, free);
}

void recursos_mostrar(matriz_recursos recursos){

	void mostrar(especie_pokemon unaEspecie, void*cantidadDeInstancias){
		printf(" (%s, %u)", unaEspecie, *((numero*)cantidadDeInstancias));
	}

	dictionary_iterator(recursos, &mostrar);
}

bool recursos_matriz_nula(matriz_recursos recursos){
	bool esNula = true;

	void esoNoEsCierto(char*recurso, void*cantidad){
		if(*((numero*)cantidad)){
			esNula = false;
		}
	}

	dictionary_iterator(recursos, esoNoEsCierto);

	return esNula;
}

numero recursos_contar(matriz_recursos recursos){
	numero cantidad = 0;

	void contar(recurso unRecurso, void*suCantidad){
		cantidad+= *((numero*)suCantidad);
	}

	dictionary_iterator(recursos, contar);

	return cantidad;
}

//Operaciones entre matrices
void recursos_sumar_recursos_a(matriz_recursos unaMatriz, matriz_recursos otraMatriz){
	void sumar_recursos(especie_pokemon unaEspecie, void*cantidadDeInstancias){
		recursos_agregar_N_instancias_de(unaMatriz, unaEspecie, *((numero*) cantidadDeInstancias));
	}

	dictionary_iterator(otraMatriz, sumar_recursos); //itero la otra matriz, dado que no necesariamente va a tener recursos de igual tipo

}

void recursos_restar_recursos_a(matriz_recursos unaMatriz, matriz_recursos otraMatriz){
	void restar_semejantes(especie_pokemon unaEspecie, void*cantidadDeInstancias){
		*((numero*) cantidadDeInstancias) -= recursos_cantidad_de_instancias_de(otraMatriz, unaEspecie);
	}

	dictionary_iterator(unaMatriz, restar_semejantes);
}

matriz_recursos recursos_duplicar(matriz_recursos recursos){
	matriz_recursos duplicada = recursos_create();

	void duplicar(char* recurso, void*cantidad){
		numero* cantidadDuplicada = malloc(sizeof(numero));
		      * cantidadDuplicada = *((numero*)cantidad);

		dictionary_put(duplicada, recurso, cantidadDuplicada);
	}

	dictionary_iterator(recursos, duplicar);

	return duplicada;
}

matriz_recursos recursos_matriz_diferencia(matriz_recursos unaMatriz, matriz_recursos otraMatriz){
	matriz_recursos matrizDiferencia = recursos_duplicar(unaMatriz);
	recursos_restar_recursos_a(matrizDiferencia, otraMatriz);
	return matrizDiferencia;
}

bool recursos_suficientes_para(matriz_recursos proveedora, matriz_recursos receptora){
	matriz_recursos diferencia = recursos_matriz_diferencia(proveedora, receptora);
	bool SiONo = recursos_matriz_nula(diferencia);
	recursos_destroy(diferencia);

	return SiONo;
}
