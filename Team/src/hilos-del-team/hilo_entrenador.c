#include "../deadlock/candidatos_intercambio.h"
#include "../hilos-del-team/hilos_team.h"
#include "../team.h"

//void entrenador_ir_hacia(entrenador*, t_posicion);
bool entrenador_verificar_objetivos(entrenador*unEntrenador);

void team_hilo_entrenador(entrenador*unEntrenador){
	t_id pid = unEntrenador->id;
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "Se cargo al Entrenador N°%u en estado NEW, en la posicion %s", pid, posicion_to_string(unEntrenador->posicion));
	pthread_mutex_unlock(&Mutex_AndoLoggeando);

	bool hiloActivo = true;
	while(hiloActivo){

		entrenador_esperar_y_consumir_cpu(unEntrenador);
		switch(unEntrenador->siguienteTarea){
			case CATCHEAR: {

				pokemon*unPokemon = unEntrenador->pokemonEntreOjos;

				entrenador_desplazarse_hacia(unEntrenador, unPokemon->posicion);

				Catch(unEntrenador, unPokemon); //TODO Descomentar conexiones

				//Duerme al entrenador hasta que llegue el resultado y consume 1 ciclo de cpu

				break;
			}

			case CAPTURAR: {

				pokemon*pokemonCatcheado = unEntrenador->pokemonEntreOjos;

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

				puts("********* signal(cpuConsumido)");
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
		pthread_create(&hilosEntrenadores[i], NULL, (void*) team_hilo_entrenador, list_get(equipo, i));

		sem_post(&HayEntrenadoresDisponibles);
	}
}

void finalizar_hilos_entrenadores(){
	int i=0;
	for(i=0; i<cantidadDeEntrenadores; i++){
		pthread_join(hilosEntrenadores[i], NULL);
		sem_destroy(&EjecutarEntrenador[i]);
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

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "\n--------------- Cambio de Estado ---------------\n Proceso: Entrenador N°%u\n Estado Inicial: %s\n Estado Final: %s\n Motivo: %s\n", unEntrenador->id, estadoFromEnum(estadoActual),  estadoFromEnum(estadoFinal), motivo);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

//Incrementa sus recursos y lo contabiliza como inventario global
void entrenador_capturar(entrenador*entrenador, pokemon*victima){

	recursos_agregar_recurso(entrenador->pokemonesCazados, victima->especie);

//	objetivos_actualizar_por_captura_de(victima->especie);

	t_posicion posicionDelEvento = entrenador->posicion;

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El Entrenador N°%u ha capturado un %s en la posicion [%u %u]", entrenador->id, victima->especie, posicionDelEvento.pos_x, posicionDelEvento.pos_y);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);

	pokemon_destroy(victima);
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
//	entrenador_ir_hacia(unEntrenador, posicionFinal); //Verdadera implementacion

	//implementacion anterior, para pruebas rapidas
	t_posicion posicionActual = unEntrenador->posicion;

	pthread_mutex_lock(&mutexPosicionEntrenador[unEntrenador->id]);
	unEntrenador->posicion = posicionFinal;
	pthread_mutex_unlock(&mutexPosicionEntrenador[unEntrenador->id]);

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El Entrenador N°%u se desplazo desde [%u %u] hasta [%u %u]", unEntrenador->id, posicionActual.pos_x, posicionActual.pos_y, unEntrenador->posicion.pos_x, unEntrenador->posicion.pos_y);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

bool entrenador_llego_a(entrenador* unEntrenador, t_posicion posicion){
	return posicion_cmp(unEntrenador->posicion, posicion);
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
