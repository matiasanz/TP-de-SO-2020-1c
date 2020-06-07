#include "../hilos_team.h"
#include "../../team.h"

void team_suscriptor_cola_CAUGHT(cr_list* mensajes){

	while(PROCESO_ACTIVO){

		mensaje* mensajeRecibido = cr_list_wait_and_remove(mensajes, 0);
		resultado_captura* resultado = desempaquetar_resultado(mensajeRecibido->serializado);
		captura_pendiente* capturaPendiente = pendientes_get(capturasPendientes, resultado->idCaptura);

		if(capturaPendiente) {
			entrenador* unEntrenador = capturaPendiente->cazador;
			pokemon*pokemonCatcheado = capturaPendiente->pokemonCatcheado;

			pthread_mutex_lock(&Mutex_AndoLoggeando);
			log_info(logger, "CAUGHT %s: %s", pokemonCatcheado->especie, (resultado->tuvoExito? "Exitoso": "Fallido"));
			pthread_mutex_unlock(&Mutex_AndoLoggeando);

			if(resultado->tuvoExito){
				entrenador_pasar_a(unEntrenador, READY, "Se confirmo la captura del pokemon");
				cr_list_add_and_signal(entrenadoresReady, unEntrenador);
				sem_post(&HayTareasPendientes);
			}

			else{
				entrenador_bloquear_hasta_APPEARED(unEntrenador);
				printf("El entrenador N°%u Fallo en capturar al pokemon pero puede seguir cazando mas\n", unEntrenador->id);

				pthread_mutex_lock(&mutexRecursosEnMapa);
				recursos_quitar_instancia_de_recurso(recursosEnMapa, pokemonCatcheado->especie);
				pthread_mutex_unlock(&mutexRecursosEnMapa);
				pokemon_destroy(pokemonCatcheado);
			}

			free(resultado); //Se descarta el id

		}

		else { //La idea es que esto despues no este
			pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
			log_info(event_logger, "Se recibio un resultado con id %u desconocido\n", resultado->idCaptura);
			pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
		}

	}

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Finalizo suscripcion a cola CAUGHT"); //Ver TODO si pokemon localized hace esto. Ver como saltear esta parte.
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

void entrenador_bloquear_hasta_APPEARED(entrenador*unEntrenador){
	pthread_mutex_lock(&mutexEstadoEntrenador[unEntrenador->id]);
	unEntrenador->estado = LOCKED_HASTA_APPEARED;  //Abstraer a funcion
	pthread_mutex_unlock(&mutexEstadoEntrenador[unEntrenador->id]);
	unEntrenador->siguienteTarea = CATCHEAR;
}
