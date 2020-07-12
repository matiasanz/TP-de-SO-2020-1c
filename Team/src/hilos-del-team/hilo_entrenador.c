#include "../dominio/estructuras-auxiliares/candidatos_intercambio.h"
#include "hilos_team.h"
#include "../team.h"

pokemon* entrenador_get_proxima_presa(entrenador*unEntrenador){
	return unEntrenador->proximaPresa;
}

void team_hilo_entrenador(entrenador*unEntrenador){
	t_id pid = unEntrenador->id;

	log_enunciado_entrenador_creado(unEntrenador);

	bool hiloActivo = true;
	while(hiloActivo){

		entrenador_esperar_y_consumir_cpu(unEntrenador);
		switch(unEntrenador->siguienteTarea){
			case CATCHEAR: {

				pokemon*unPokemon = entrenador_get_proxima_presa(unEntrenador);

				entrenador_desplazarse_hacia(unEntrenador, unPokemon->posicion);

				Catch(unEntrenador, unPokemon);

				//Al hacer catch, el entrenador consume 1 ciclo de CPU y queda dormido hasta que llegue el resultado

				break;
			}

			case CAPTURAR: {

				pokemon*pokemonCatcheado = entrenador_get_proxima_presa(unEntrenador);

				if(!pokemonCatcheado){
					error_show("El entrenador N°%u intento capturar un pokemon nulo\n", pid);
					exit(1);
				}

				entrenador_capturar(unEntrenador, pokemonCatcheado);

				if(entrenador_puede_cazar_mas_pokemones(unEntrenador)){
					unEntrenador->siguienteTarea = CATCHEAR;
					entrenador_pasar_a(unEntrenador, LOCKED_HASTA_APPEARED, "Tuvo exito en la captura y todavia puede cazar mas pokemones");
					sem_post(&HayEntrenadoresDisponibles);
				}

				else hiloActivo = !entrenador_verificar_objetivos(unEntrenador);

				sem_post(&FinDeCiclo_CPU);

				break;
			}

			case INTERCAMBIAR: {

				candidato_intercambio*self = list_remove(potencialesDeadlock, 0);
				candidato_intercambio*parejaDeIntercambio = candidatos_pareja_de_intercambio_para(potencialesDeadlock, self);

				candidato_desplazarse_hacia_el_otro(self, parejaDeIntercambio);

				int cpuConsumidosPorIntercambio=1;
				do{
					sem_post(&FinDeCiclo_CPU);
					entrenador_consumir_N_cpu(unEntrenador, 5-cpuConsumidosPorIntercambio);
					candidato_intercambiar_con(self, parejaDeIntercambio);
					cpuConsumidosPorIntercambio=0;
				} while(candidato_puede_intercambiar_con(self, parejaDeIntercambio));

				hiloActivo = !entrenador_verificar_objetivos(unEntrenador);
				entrenador_verificar_objetivos(parejaDeIntercambio->interesado);

				sem_post(&FinDeCiclo_CPU); //Consumi el ultimo ciclo

				sem_post(&finDeIntercambio);
				candidato_destroy(self);
				candidato_destroy(parejaDeIntercambio);

				break;
			}
		}
	}
}

/********************************** Funciones de Inicio y Finalizacion ******************************************/

void inicializar_hilos_entrenadores(){
	cantidadDeEntrenadores = list_size(equipo);

	hilosEntrenadores = malloc(sizeof(pthread_t)*cantidadDeEntrenadores);

	EjecutarEntrenador      = malloc(sizeof(sem_t)          *cantidadDeEntrenadores);
	mutexEstadoEntrenador   = malloc(sizeof(pthread_mutex_t)*cantidadDeEntrenadores);
	mutexPosicionEntrenador = malloc(sizeof(pthread_mutex_t)*cantidadDeEntrenadores);

	int i;
	for(i=0; i<cantidadDeEntrenadores; i++){
		sem_init(&EjecutarEntrenador[i], 0, 0);
		pthread_mutex_init(&mutexEstadoEntrenador[i], NULL);
		pthread_mutex_init(&mutexPosicionEntrenador[i], NULL);

		pthread_t hiloEntrenador;
		pthread_create(&hiloEntrenador, NULL, (void*) team_hilo_entrenador, list_get(equipo, i));
		pthread_detach(hiloEntrenador);

		sem_post(&HayEntrenadoresDisponibles);
	}
}

void finalizar_hilos_entrenadores(){
	int i=0;
	for(i=0; i<cantidadDeEntrenadores; i++){
//		pthread_join(hilosEntrenadores[i], NULL);
//		sem_destroy(&EjecutarEntrenador[i]);
		pthread_mutex_destroy(&mutexPosicionEntrenador[i]);
		pthread_mutex_destroy(&mutexEstadoEntrenador[i]);
	}
}

/*************************************** Funciones Auxiliares ************************************************/

bool cambio_de_contexto(t_estado inicial, t_estado final){
	return final==EXECUTE;
}

//Cambia de estado al entrenador y lo loggea
void entrenador_pasar_a(entrenador*unEntrenador, t_estado estadoFinal, const char*motivo){
	pthread_mutex_lock(&mutexEstadoEntrenador[unEntrenador->id]);
	t_estado estadoActual = unEntrenador->estado;
	unEntrenador->estado = estadoFinal;
	pthread_mutex_unlock(&mutexEstadoEntrenador[unEntrenador->id]);

	log_enunciado_cambio_de_cola(unEntrenador, estadoActual, estadoFinal, motivo);

	if(cambio_de_contexto(estadoActual, estadoFinal)){
		Estadisticas.cambiosDeContexto++;
	}
}

//Incrementa sus recursos y lo contabiliza como inventario global
void entrenador_capturar(entrenador*entrenador, pokemon*victima){

	recursos_agregar_recurso(entrenador->pokemonesCazados, victima->especie);

	t_posicion posicionDelEvento = entrenador->posicion;

	log_enunciado_operacion_de_atrapar(entrenador, victima, posicionDelEvento);

	pokemon_destroy_hard(victima);
}

//Remueve un entrenador de una lista
entrenador* entrenadores_remover_del_equipo_a(entrenadores unEquipo, t_id id){
	bool entrenador_cmpId(entrenador*unEntrenador){
		return id == unEntrenador->id;
	}

	pthread_mutex_lock(&mutexEntrenadores);
	entrenador*removido = list_remove_by_condition(unEquipo, (bool(*)(void*)) &entrenador_cmpId);
	pthread_mutex_unlock(&mutexEntrenadores);

	return removido;
}

void candidato_desplazarse_hacia_el_otro(candidato_intercambio*unCandidato, candidato_intercambio*haciaQuien){
	entrenador_desplazarse_hacia(unCandidato->interesado, haciaQuien->interesado->posicion);
}

void entrenador_desplazarse_hacia(entrenador* unEntrenador, t_posicion posicionFinal){
	log_event_de_donde_partio(unEntrenador);

	bool llegoALaPosicion = entrenador_llego_a(unEntrenador, posicionFinal);

	while(!llegoALaPosicion){

		pthread_mutex_lock(&mutexPosicionEntrenador[unEntrenador->id]);
		entrenador_dar_un_paso_hacia(unEntrenador, posicionFinal);
		llegoALaPosicion = entrenador_llego_a(unEntrenador, posicionFinal);
		pthread_mutex_unlock(&mutexPosicionEntrenador[unEntrenador->id]);

		sem_post(&FinDeCiclo_CPU);
		entrenador_esperar_y_consumir_cpu(unEntrenador);
	}
}

void desplazar_unidimensional(coordenada* posicionInicial, coordenada posicionFinal){
	int desplazamiento = (posicionFinal > *posicionInicial) - (posicionFinal < *posicionInicial);
	*posicionInicial += desplazamiento;
}

void entrenador_dar_un_paso_hacia(entrenador*unEntrenador, t_posicion posicionFinal){

	t_posicion* posicionActual = &unEntrenador->posicion;

	if(posicionActual->pos_x != posicionFinal.pos_x){
		desplazar_unidimensional(&posicionActual->pos_x, posicionFinal.pos_x);
	}

	else{
		desplazar_unidimensional(&posicionActual->pos_y, posicionFinal.pos_y);
	}

	log_enunciado_movimiento_de_un_entrenador(unEntrenador);

}

bool entrenador_llego_a(entrenador* unEntrenador, t_posicion posicion){
	return posicion_cmp(unEntrenador->posicion, posicion);
}

//Funcion hardcodeada para acelerar pruebas
void entrenador_teletransportarte_a(entrenador*unEntrenador, t_posicion posicionFinal){
//	//implementacion anterior, para pruebas rapidas
	t_posicion posicionActual = unEntrenador->posicion;

	pthread_mutex_lock(&mutexPosicionEntrenador[unEntrenador->id]);
	unEntrenador->posicion = posicionFinal;
	pthread_mutex_unlock(&mutexPosicionEntrenador[unEntrenador->id]);

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El Entrenador N°%u se desplazo desde [%u %u] hasta [%u %u]", unEntrenador->id, posicionActual.pos_x, posicionActual.pos_y, unEntrenador->posicion.pos_x, unEntrenador->posicion.pos_y);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

//************************************* Salida ********************************************/

// Retorna true si el entrenador debe seguir ejecutando o false si es que ya puede finalizar
bool entrenador_verificar_objetivos(entrenador*unEntrenador){

	bool cumplioObjetivos = entrenador_cumplio_sus_objetivos(unEntrenador);

	printf("Objetivos: "); recursos_mostrar(unEntrenador->objetivos);puts("");
	printf("Inventario: "); recursos_mostrar(unEntrenador->pokemonesCazados);puts("");

	if(cumplioObjetivos){

		entrenador_pasar_a(unEntrenador, EXIT, "Ya logro cumplir sus objetivos");

		PROCESOS_SIN_FINALIZAR--;

	}

	else{
		unEntrenador->siguienteTarea = INTERCAMBIAR;
		if(!entrenador_en_estado(unEntrenador, LOCKED_HASTA_DEADLOCK)) entrenador_pasar_a(unEntrenador, LOCKED_HASTA_DEADLOCK, "Su inventario esta lleno y no cumplio sus objetivos");
		candidatos_agregar_entrenador(potencialesDeadlock, unEntrenador);
	}

	sem_post(&EquipoNoPuedaCazarMas);
	return cumplioObjetivos;
}
