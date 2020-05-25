#include "../hilos_team.h"
#include "../../team.h"

void team_suscriptor_cola_CAUGHT(cr_list* mensajes){

	while(!FinDelProceso){

		mensaje* mensajeRecibido = cr_list_wait_and_remove(mensajes, 0);

		puts("---------------------------------------------------------------------entra caught");

		resultado_captura* resultado = desempaquetar_resultado(mensajeRecibido->serializado);

		puts("---------------------------------------------------------------------rdo desempaquetado");

		pendiente* capturaPendiente = pendientes_get(capturasPendientes, resultado->idCaptura);

		if(!capturaPendiente){
			pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
			log_info(event_logger, "Se recibio un resultado con id %u desconocido\n", resultado->idCaptura);
			pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
		}

		else {
			puts("---------------------------------------------------------------hay algo de verdad");
			entrenador* unEntrenador = capturaPendiente->cazador;
			pokemon*pokemonCatcheado = capturaPendiente->pokemonCatcheado;

			puts("---------------------------------------------------Efectivamente era un pendiente");

			pthread_mutex_lock(&Mutex_AndoLoggeando);
			log_info(logger, "CAUGHT %s: %s", pokemonCatcheado->especie, (resultado->tuvoExito? "Exitoso": "Fallido"));
			pthread_mutex_unlock(&Mutex_AndoLoggeando);

			if(resultado->tuvoExito){
				entrenador_pasar_a(unEntrenador, READY, "Se confirmo la captura del pokemon");
			}

			else{
				pokemon_destroy(pokemonCatcheado);
				entrenador_pasar_a(unEntrenador, LOCKED_HASTA_APPEARD, "Fallo en capturar al pokemon pero puede seguir cazando mas");
			}

			free(resultado); //Se descarta el id

			sem_post(&HayTareasPendientes);
		}
	}
}
