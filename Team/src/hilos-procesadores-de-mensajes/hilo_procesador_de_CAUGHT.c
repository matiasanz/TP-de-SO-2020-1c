#include "../hilos-del-team/hilos_team.h"
#include "../team.h"
#include "../estructuras-auxiliares/mensajes.h"

resultado_captura desempaquetar_resultado(t_mensaje_caught_pokemon*mensaje){
	return (resultado_captura) {mensaje->mensaje_header.id_correlativo,  (bool) mensaje->atrapado};
}

void team_procesador_cola_CAUGHT(cr_list* mensajes){

	while(PROCESO_ACTIVO){

		t_mensaje_caught_pokemon* mensajeRecibido = leer_mensaje_cuando_este_disponible(mensajes);
		resultado_captura resultado = desempaquetar_resultado(mensajeRecibido);
		captura_pendiente* capturaPendiente = pendientes_get(capturasPendientes, resultado.idCaptura);

		if(capturaPendiente) {
			entrenador* unEntrenador = capturaPendiente->cazador;
			pokemon*pokemonCatcheado = capturaPendiente->pokemonCatcheado;

			pthread_mutex_lock(&Mutex_AndoLoggeando);
			log_info(logger, "CAUGHT %s: %s", pokemonCatcheado->especie, (resultado.tuvoExito? "Exitoso": "Fallido"));
			pthread_mutex_unlock(&Mutex_AndoLoggeando);

			pthread_mutex_lock(&mutexRecursosEnMapa);
			if(resultado.tuvoExito && captura_sigue_siendo_requerida(capturaPendiente)){
				recursos_agregar_recurso(recursosEnMapa, capturaPendiente->pokemonCatcheado->especie);
				pthread_mutex_unlock(&mutexRecursosEnMapa);

				entrenador_pasar_a(unEntrenador, READY, "Se confirmo la captura del pokemon");
				cr_list_add_and_signal(entrenadoresReady, unEntrenador);
				sem_post(&HayTareasPendientes);
				sem_post(&HayEntrenadoresDisponibles);
			}

			else{
				pthread_mutex_unlock(&mutexRecursosEnMapa);
				entrenador_bloquear_hasta_APPEARED(unEntrenador);
				log_info(event_logger, ">> Se descarto a %s porque %s\n", pokemonCatcheado->especie, (resultado.idCaptura? "ya no es requerido": "fallo la captura"));
				pokemon_destroy(pokemonCatcheado);
				cr_list_wait_and_remove(capturasPendientes, 0);
				pendiente_destroy(capturaPendiente);
			}
		}

		else {
			pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
			log_warning(event_logger, "Se recibio el resultado de una captura id %u desconocida\n", resultado.idCaptura);
			pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
		}

		mensaje_caught_pokemon_destruir(mensajeRecibido);
	}

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Finalizo suscripcion a cola CAUGHT"); //Ver TODO si pokemon localized hace esto. Ver como saltear esta parte.
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

void entrenador_bloquear_hasta_APPEARED(entrenador*unEntrenador){
	unEntrenador->siguienteTarea = CATCHEAR;
	pthread_mutex_lock(&mutexEstadoEntrenador[unEntrenador->id]);
	unEntrenador->estado = LOCKED_HASTA_APPEARED;  //Abstraer a funcion
	pthread_mutex_unlock(&mutexEstadoEntrenador[unEntrenador->id]);
	sem_post(&HayEntrenadoresDisponibles);
}

bool captura_sigue_siendo_requerida(captura_pendiente*unaCaptura){
	especie_pokemon especie = unaCaptura->pokemonCatcheado->especie;
	numero cantidadBruta = objetivos_cantidad_bruta_requerida_de(especie);
	numero instanciasEnMapa = recursos_cantidad_de_instancias_de(recursosEnMapa, especie);

	return (cantidadBruta - instanciasEnMapa);
}
