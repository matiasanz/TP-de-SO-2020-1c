#include "hilos_team.h"
#include "../dominio/deadlock/estructuras_deadlock.h"
#include "../team.h"

bool entrenador_validar_objetivos(entrenador*unEntrenador);

void team_hilo_entrenador(entrenador*unEntrenador){
	printf("Entrenador N°%u en espera\n", unEntrenador->id);
	t_id pid = unEntrenador->id;

	bool hiloActivo = true;
	while(hiloActivo){

		sem_wait(&EjecutarEntrenador[pid]);
		switch(unEntrenador->siguienteTarea){
			case CATCHEAR: {
				pokemon*unPokemon = mapa_desmapear(pokemonesRequeridos);
				//Si no tengo pokemones en mapa pero le toca ejecutar por criterio, tengo un overhead hasta que llegue alguno. En teoria siempre van a estar llegando pokemones
				//Duda: que pasa cuando criterio no es fifo TODO

				entrenador_pasar_a(unEntrenador, EXECUTE, "Es su turno de ejecutar y lo va a utilizar para intentar cachear");

				entrenador_desplazarse_hacia(unEntrenador, unPokemon->posicion);

				t_id id_mensaje_pendiente = Catch(unPokemon->especie);

				agregar_pendiente(capturasPendientes, id_mensaje_pendiente, unEntrenador, unPokemon);

				unEntrenador->siguienteTarea = CAPTURAR;

				entrenador_pasar_a(unEntrenador, LOCKED_HASTA_CAUGHT, "Tiene una captura pendiente");

				break;
			}

			case CAPTURAR: {
				entrenador_pasar_a(unEntrenador, EXECUTE, "Es su turno de ejecutar y lo va a utilizar para capturar");
				captura_pendiente* capturaPendiente = pendientes_pendiente_del_entrenador(capturasPendientes, pid);

				if(!capturaPendiente){
					error_show("El id del entrenador N°%u no se corresponde con ningun mensaje pendiente\n", pid);
					exit(1);
				}

				pokemon*pokemonCatcheado = capturaPendiente->pokemonCatcheado;

				entrenador_capturar(unEntrenador, pokemonCatcheado);

				if(entrenador_puede_cazar_mas_pokemones(unEntrenador)){ //Ver si cambios de estado se pueden delegar al planificador
					unEntrenador->siguienteTarea = CATCHEAR;
					entrenador_pasar_a(unEntrenador, LOCKED_HASTA_APPEARED, "Tuvo exito en la captura y todavia puede cazar mas pokemones");
					sem_post(&HayEntrenadoresDisponibles);
				}

				else{
					hiloActivo = !entrenador_validar_objetivos(unEntrenador);
				}

				break;
			}

			case INTERCAMBIAR: {
				entrenador_pasar_a(unEntrenador, EXECUTE, "Es su turno de ejecutar y lo va a utilizar para intercambiar");

				candidato_intercambio*self = list_remove(potencialesDeadlock, 0);
				candidato_intercambio*parejaDeIntercambio = candidatos_pareja_de_intercambio_para(potencialesDeadlock, self);

				candidato_desplazarse_hacia_el_otro(self, parejaDeIntercambio); //Duda

				while(candidato_puede_intercambiar_con(self, parejaDeIntercambio)){
					candidato_intercambiar_con(self, parejaDeIntercambio);
					break;
				}

				hiloActivo = !entrenador_validar_objetivos(unEntrenador);

				entrenador_validar_objetivos(parejaDeIntercambio->unEntrenador);

				sem_post(&finDeIntercambio);

				break;
			}
		}

		sem_post(&EntradaSalida_o_FinDeEjecucion);
	}

	entrenador_destroy(unEntrenador);

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Finalizo un hilo entrenador");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

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
		pthread_create(&hilosEntrenadores[i], NULL, (void*) team_hilo_entrenador, list_get(equipo, i));

		sem_post(&HayEntrenadoresDisponibles);
	}
}

void finalizar_hilos_entrenadores(){
	int i=0;
	for(i=0; i<cantidadDeEntrenadores; i++){
		pthread_join(hilosEntrenadores[i], NULL);
		sem_destroy(&EjecutarEntrenador[i]);
		pthread_mutex_destroy(&mutexEstadoEntrenador[i]);
		pthread_mutex_destroy(&mutexPosicionEntrenador[i]);
	}
}

/*************************************** Funciones Auxiliares ************************************************/

//Cambia de estado al entrenador y lo loggea
void entrenador_pasar_a(entrenador*unEntrenador, t_estado estadoFinal, const char*motivo){
	pthread_mutex_lock(&mutexEstadoEntrenador[unEntrenador->id]);
	t_estado estadoActual = unEntrenador->estado;
	unEntrenador->estado = estadoFinal;
	pthread_mutex_unlock(&mutexEstadoEntrenador[unEntrenador->id]);

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El Entrenador N°%u se paso de la cola de %s a %s, Motivo: %s", unEntrenador->id, estadoFromEnum(estadoActual), estadoFromEnum(estadoFinal), motivo);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

//Incrementa sus recursos y lo contabiliza como inventario global
void entrenador_capturar(entrenador*entrenador, pokemon*victima){

	recursos_agregar_recurso(entrenador->pokemonesCazados, victima->especie);

	objetivos_actualizar_por_captura_de(victima->especie);

	t_posicion posicionDelEvento = entrenador->posicion;

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El Entrenador N°%u ha capturado un %s en la posicion [%u %u]", entrenador->id, victima->especie, posicionDelEvento.pos_x, posicionDelEvento.pos_y);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);

	pokemon_destroy(victima);
}

//Contabiliza la captura de un pokemon en mapa y en inventarios
void objetivos_actualizar_por_captura_de(especie_pokemon unaEspecie){
	pthread_mutex_lock(&mutexInventariosGlobales);
	recursos_agregar_recurso(inventariosGlobales, unaEspecie);
	pthread_mutex_unlock(&mutexInventariosGlobales);

	pthread_mutex_lock(&mutexRecursosEnMapa);
	recursos_quitar_instancia_de_recurso(recursosEnMapa, unaEspecie);
	pthread_mutex_unlock(&mutexRecursosEnMapa);
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
	entrenador_desplazarse_hacia(unCandidato->unEntrenador, haciaQuien->unEntrenador->posicion);
}

void entrenador_desplazarse_hacia(entrenador* unEntrenador, t_posicion posicionFinal){
	t_posicion posicionActual = unEntrenador->posicion;

	pthread_mutex_lock(&mutexPosicionEntrenador[unEntrenador->id]);
	unEntrenador->posicion = posicionFinal;
	pthread_mutex_unlock(&mutexPosicionEntrenador[unEntrenador->id]);

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El Entrenador N°%u se desplazo desde [%u %u] hasta [%u %u]", unEntrenador->id, posicionActual.pos_x, posicionActual.pos_y, unEntrenador->posicion.pos_x, unEntrenador->posicion.pos_y);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

bool entrenador_llego_a(entrenador* unEntrenador, t_posicion posicion){
//	pthread_mutex_lock(&mutexPosicionEntrenador[unEntrenador->id]); //momentaneamente no hace falta ya que se pregunta despues de moverlo
	t_posicion posDelEntrenador = unEntrenador->posicion;
//	pthread_mutex_unlock(&mutexPosicionEntrenador[unEntrenador->id]);

	return posicion_cmp(posDelEntrenador, posicion);
}

//************************************* Salida ********************************************/

// Retorna true si el entrenador debe seguir ejecutando o false si es que ya puede finalizar
bool entrenador_validar_objetivos(entrenador*unEntrenador){

	bool cumplioObjetivos = entrenador_cumplio_sus_objetivos(unEntrenador);

	if(cumplioObjetivos){
		printf("Objetivos: "); recursos_mostrar(unEntrenador->objetivos);
		printf("Inventario: "); recursos_mostrar(unEntrenador->pokemonesCazados); puts("");

		entrenador_pasar_a(unEntrenador, EXIT, "Ya logro cumplir sus objetivos");

		entrenadores_remover_del_equipo_a(equipo, unEntrenador->id);
	}

	else{
		unEntrenador->siguienteTarea = INTERCAMBIAR;
		entrenador_pasar_a(unEntrenador, LOCKED_HASTA_DEADLOCK, "Su inventario esta lleno y no cumplio sus objetivos");

		printf("Objetivos: "); recursos_mostrar(unEntrenador->objetivos);puts("");
		printf("Inventario: "); recursos_mostrar(unEntrenador->pokemonesCazados);puts("");

		candidatos_agregar_entrenador(potencialesDeadlock, unEntrenador);
	}

	sem_post(&EquipoNoPuedaCazarMas);
	return cumplioObjetivos;
}
