#include "../hilos_del_team/hilos_team.h"
#include "../team.h"

resultado_captura desempaquetar_resultado(t_mensaje_caught_pokemon*mensaje){
	return (resultado_captura) {mensaje->mensaje_header.id_correlativo,  (bool) mensaje->atrapado};
}

void team_suscriptor_cola_CAUGHT(cr_list* mensajes){

	while(PROCESO_ACTIVO){

		t_mensaje_caught_pokemon* mensajeRecibido = cr_list_wait_and_remove(mensajes, 0);
		resultado_captura resultado = desempaquetar_resultado(mensajeRecibido);
		captura_pendiente* capturaPendiente = pendientes_get(capturasPendientes, resultado.idCaptura);

		if(capturaPendiente) {
			entrenador* unEntrenador = capturaPendiente->cazador;
			pokemon*pokemonCatcheado = capturaPendiente->pokemonCatcheado;

			log_info(logger, "CAUGHT %s: %s", pokemonCatcheado->especie, (resultado.tuvoExito? "Exitoso": "Fallido"));

			if(resultado.tuvoExito){
				entrenador_pasar_a(unEntrenador, READY, "Se confirmo la captura del pokemon");
				cr_list_add_and_signal(entrenadoresReady, unEntrenador);
			}

			else{
				entrenador_bloquear_hasta_APPEARED(unEntrenador);
				printf("El entrenador N°%u Fallo en capturar al pokemon pero puede seguir cazando mas\n", unEntrenador->id);

				pthread_mutex_lock(&mutexRecursosEnMapa);
				recursos_quitar_instancia_de_recurso(recursosEnMapa, pokemonCatcheado->especie);
				pthread_mutex_unlock(&mutexRecursosEnMapa);
				pokemon_destroy(pokemonCatcheado);
			}

			sem_post(&HayTareasPendientes);
		}

		else { //La idea es que esto despues no este
			log_info(event_logger, "Se recibio un resultado con id %u desconocido\n", resultado.idCaptura);
		}

		mensaje_caught_pokemon_destruir(mensajeRecibido);
	}

	log_info(event_logger, "Finalizo suscripcion a cola CAUGHT"); //Ver TODO si pokemon localized hace esto. Ver como saltear esta parte.
}

void entrenador_bloquear_hasta_APPEARED(entrenador*unEntrenador){
	pthread_mutex_lock(&mutexEstadoEntrenador[unEntrenador->id]);
	unEntrenador->estado = LOCKED_HASTA_APPEARED;  //Abstraer a funcion
	pthread_mutex_unlock(&mutexEstadoEntrenador[unEntrenador->id]);
	unEntrenador->siguienteTarea = CATCHEAR;
}
