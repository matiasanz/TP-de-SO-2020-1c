#include "../hilos_team.h"
#include "../../team.h"
#include "../../dominio/estructuras-auxiliares/mensajes.h"

void pokemon_localizar_en_mapa(pokemon*pokemonCatcheado);

void team_procesador_cola_CAUGHT(cr_list* mensajes){

	while(PROCESO_ACTIVO){

		t_mensaje_caught_pokemon* mensajeRecibido = leer_mensaje_cuando_este_disponible(mensajes);

		t_id idCaptura = mensaje_caught_pokemon_get_id_correlativo(mensajeRecibido);

		captura_pendiente* capturaPendiente = pendientes_remove_by_id(capturasPendientes, idCaptura);

		if(capturaPendiente) {

			pthread_mutex_lock(&Mutex_AndoLoggeando);
			mensaje_caught_pokemon_log(logger, mensajeRecibido);
			pthread_mutex_unlock(&Mutex_AndoLoggeando);

			validar_captura(capturaPendiente);

			bool resultado = mensaje_caught_pokemon_get_resultado(mensajeRecibido);

			procesar_resultado_de_captura(capturaPendiente, resultado);
		}

		else {
			pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
			log_info(event_logger, "Se recibio el resultado de una captura id %u desconocida\n", idCaptura);
			pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
		}

		mensaje_caught_pokemon_destruir(mensajeRecibido);
	}
}

/*************************************** Funciones Auxiliares ************************************************/

void entrenador_dormir_hasta_llegada_de_pokemon(entrenador*unEntrenador){
	unEntrenador->siguienteTarea = CATCHEAR;
	unEntrenador->proximaPresa = NULL;
	pthread_mutex_lock(&mutexEstadoEntrenador[unEntrenador->id]);
	unEntrenador->estado = LOCKED_HASTA_APPEARED;
	pthread_mutex_unlock(&mutexEstadoEntrenador[unEntrenador->id]);
	sem_post(&HayEntrenadoresDisponibles);
}

void validar_captura(captura_pendiente*unaCaptura){
	especie_pokemon especie = unaCaptura->pokemonCatcheado->especie;

	pthread_mutex_lock(&mutexRecursosDisponibles);
	numero cantidadRequerida = objetivos_cantidad_bruta_requerida_de(especie);
	pthread_mutex_unlock(&mutexRecursosDisponibles);

	if( ((int)cantidadRequerida) <= 0){
		log_error(event_logger, "Se capturo un %s que no se necesitaba", especie);

		printf("Objetivos: "); recursos_mostrar(objetivosGlobales);
		printf("\nInventarios: "); recursos_mostrar(inventariosGlobales);

		exit(1);
	}
}

//Contabiliza la captura de un pokemon en mapa y en inventarios
void objetivos_actualizar_por_captura_de(especie_pokemon unaEspecie){

	pthread_mutex_lock(&mutexRecursosDisponibles);

//	pthread_mutex_lock(&mutexRecursosDisponibles);
	recursos_agregar_recurso(inventariosGlobales, unaEspecie);
//	pthread_mutex_unlock(&mutexRecursosDisponibles);

	recursos_quitar_instancia_de_recurso(recursosEnMapa, unaEspecie);
	pthread_mutex_unlock(&mutexRecursosDisponibles);
}

bool mensaje_caught_pokemon_get_resultado(t_mensaje_caught_pokemon* mensaje){
	return mensaje->atrapado;
}

bool pokemon_necesito_mas_instancias(pokemon*unPokemon){
	pthread_mutex_lock(&mutexRecursosDisponibles);
	bool siTraeMas = pokemon_es_requerido(*unPokemon);
	pthread_mutex_unlock(&mutexRecursosDisponibles);

	return siTraeMas;
}

void procesar_resultado_de_captura(captura_pendiente*capturaPendiente, bool fueExitosa){

	entrenador* unEntrenador     = capturaPendiente->cazador;
	pokemon*    pokemonCatcheado = capturaPendiente->pokemonCatcheado;

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "CAUGHT %s: %s", pokemonCatcheado->especie, (fueExitosa? "OK": "FAIL"));
	pthread_mutex_unlock(&Mutex_AndoLoggeando);

	if(fueExitosa){

		objetivos_actualizar_por_captura_de(pokemonCatcheado->especie);

		entrenador_despertar(unEntrenador, "Se confirmo la captura del pokemon");
		pokemon_localizar_en_mapa(pokemonCatcheado);
	}

	else{
		captura_procesar_fallo(capturaPendiente);
	}

	pendiente_destroy(capturaPendiente);
}

void captura_procesar_fallo(captura_pendiente*capturaFallida){
	entrenador* cazador = capturaFallida->cazador;
	pokemon*pokemonCatcheado = capturaFallida->pokemonCatcheado;

	pthread_mutex_lock(&mutexRecursosDisponibles);
	recursos_quitar_instancia_de_recurso(recursosEnMapa, pokemonCatcheado->especie);
	pthread_mutex_unlock(&mutexRecursosDisponibles);

	entrenador_dormir_hasta_llegada_de_pokemon(cazador);
	pokemon_localizar_en_mapa(pokemonCatcheado);
	pokemon_destroy(pokemonCatcheado); //Destroy hard?
}

void pokemon_localizar_en_mapa(pokemon*pokemonCatcheado){

	if(pokemon_necesito_mas_instancias(pokemonCatcheado)){
		pthread_mutex_lock(&mutexRepuestos);
		pokemon*repuesto = list_remove_by_comparation(pokemonesDeRepuesto, pokemonCatcheado, (bool(*)(void*, void*))&pokemon_misma_especie_que);
		pthread_mutex_unlock(&mutexRepuestos);

		if(repuesto){
			cr_list_add_and_signal(pokemonesRecibidos, repuesto);
		}
	}

	else{

		bool esOtraInstanciaDe(void* unPokemon){
			puts("\n\n XXX PAREN TODO XXX");

			return pokemon_misma_especie_que(unPokemon, pokemonCatcheado);
		}

		pthread_mutex_lock(&mutexRepuestos);
		list_remove_and_destroy_by_condition(pokemonesDeRepuesto, &esOtraInstanciaDe, (void*)pokemon_destroy_hard);
		pthread_mutex_unlock(&mutexRepuestos);
	}
}
