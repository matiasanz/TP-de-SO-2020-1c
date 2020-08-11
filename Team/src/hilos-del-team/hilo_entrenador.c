#include "../dominio/estructuras-auxiliares/candidatos_intercambio.h"
#include "hilos_team.h"
#include "../team.h"

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

				fin_de_ciclo_cpu();

				break;
			}

			case CAPTURAR: {

				pokemon*pokemonCatcheado = entrenador_get_proxima_presa(unEntrenador);

				validar_pokemon(pokemonCatcheado, pid);

				entrenador_capturar(unEntrenador, pokemonCatcheado);

				if(entrenador_puede_cazar_mas_pokemones(unEntrenador)){
					entrenador_dormir_hasta_que_aparezcan(unEntrenador);
				}

				else hiloActivo = !entrenador_verificar_objetivos(unEntrenador);

				fin_de_ciclo_cpu();

				break;
			}

			case INTERCAMBIAR: {

				candidato_intercambio*self = list_remove(potencialesDeadlock, 0);
				candidato_intercambio*parejaDeIntercambio = candidatos_pareja_de_intercambio_para(potencialesDeadlock, self);

				candidato_desplazarse_hacia_el_otro(self, parejaDeIntercambio);

				candidato_concretar_intercambio(self, parejaDeIntercambio);

				hiloActivo = !entrenador_verificar_objetivos(unEntrenador);
				entrenador_verificar_objetivos(parejaDeIntercambio->interesado);

				fin_de_ciclo_cpu(); //Consumi el ultimo ciclo

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
		pthread_mutex_destroy(&mutexPosicionEntrenador[i]);
		pthread_mutex_destroy(&mutexEstadoEntrenador[i]);
	}
}

/*************************************** Funciones Auxiliares ************************************************/

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

//Desplaza al entrenador de a un paso, consumiendo un ciclo de cpu por cada uno que da
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

//	Desplaza un entrenador de una posicion a otra de forma inmediata; Funcion hardcodeada para acelerar pruebas
void entrenador_teletransportarse_a(entrenador*unEntrenador, t_posicion posicionFinal){
//	//implementacion anterior, para pruebas rapidas
	t_posicion posicionActual = unEntrenador->posicion;

	pthread_mutex_lock(&mutexPosicionEntrenador[unEntrenador->id]);
	unEntrenador->posicion = posicionFinal;
	pthread_mutex_unlock(&mutexPosicionEntrenador[unEntrenador->id]);

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El Entrenador N°%u se desplazo desde [%u %u] hasta [%u %u]", unEntrenador->id, posicionActual.pos_x, posicionActual.pos_y, unEntrenador->posicion.pos_x, unEntrenador->posicion.pos_y);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

//Incrementa sus recursos y lo contabiliza como inventario global
void entrenador_capturar(entrenador*entrenador, pokemon*victima){

	recursos_agregar_recurso(entrenador->pokemonesCazados, victima->especie);

	t_posicion posicionDelEvento = entrenador->posicion;

	log_enunciado_operacion_de_atrapar(entrenador, victima, posicionDelEvento);

	pokemon_destroy_hard(victima);
}

void entrenador_dormir_hasta_que_aparezcan(entrenador* unEntrenador){
	unEntrenador->siguienteTarea = CATCHEAR;
	entrenador_pasar_a(unEntrenador, LOCKED_HASTA_APPEARED, "Tuvo exito en la captura y todavia puede cazar mas pokemones");
	sem_post(&HayEntrenadoresDisponibles);
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

//************************************* Salida ********************************************/

// Retorna true si el entrenador debe seguir ejecutando o false si es que ya puede finalizar
bool entrenador_verificar_objetivos(entrenador*unEntrenador){

	bool cumplioObjetivos = entrenador_cumplio_sus_objetivos(unEntrenador);

	log_event_situacion_del_entrenador(unEntrenador);

	if(cumplioObjetivos){
		entrenador_finalizar(unEntrenador);
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

void entrenador_finalizar(entrenador*unEntrenador){
	entrenador_pasar_a(unEntrenador, EXIT, "Ya logro cumplir sus objetivos");
	entrenador_liberar_recursos(unEntrenador);
}
