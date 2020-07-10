#include "../hilos_team.h"
#include "../../team.h"
#include "../../utils/mensajes.h"

void team_procesador_cola_CAUGHT(cr_list* mensajes){

	while(PROCESO_ACTIVO){

		t_mensaje_caught_pokemon* mensajeRecibido = leer_mensaje_cuando_este_disponible(mensajes);

		t_id idCaptura = mensaje_caught_pokemon_get_id_correlativo(mensajeRecibido);

		captura_pendiente* capturaPendiente = pendientes_remove_by_id(capturasPendientes, idCaptura);

		if(capturaPendiente) {

			log_enunciado_llegada_de_mensaje_caught(mensajeRecibido);

			validar_captura(capturaPendiente);

			bool resultado = mensaje_caught_pokemon_get_resultado(mensajeRecibido);

			procesar_resultado_de_captura(capturaPendiente, resultado);
		}

		else {
			log_event_captura_desconocida(mensajeRecibido);
		}

		mensaje_caught_pokemon_destruir(mensajeRecibido);
	}
}


