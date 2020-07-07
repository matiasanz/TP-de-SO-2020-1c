#ifndef _TEAM_LOGGERS_
#define _TEAM_LOGGERS_

#include <commons/log.h>
#include <commons/config.h>

#include "../hilos-del-team/deadlock/candidatos_intercambio.h"
#include "../hilos-del-team/deadlock/detector_de_deadlock.h"
#include "estructuras-auxiliares/captura_pendiente.h"
#include "estructuras-auxiliares/lector_config.h"
#include "estructuras-auxiliares/mensajes.h"
#include "estructuras-principales/pokemon.h"
#include "planificacion/planificacion.h"
#include "header_global_team.h"

	pthread_mutex_t Mutex_AndoLoggeando;
	pthread_mutex_t Mutex_AndoLoggeandoEventos;

//Logs del enunciado
	void log_enunciado_entrenador_creado(entrenador*);
	void log_enunciado_cambio_de_estado(entrenador*, t_estado actual, t_estado final, const char* motivo);
	void log_enunciado_captura_de_pokemon(entrenador*, pokemon*, t_posicion);
	void log_enunciado_desplazamiento(entrenador*);
	void log_enunciado_mensaje_appeared(t_mensaje_appeared_pokemon*);
	void log_enunciado_mensaje_localized(t_mensaje_localized_pokemon*);

//Eventos Random
	void log_event_localized_repetido(especie_pokemon);
	void log_event_localized_descartado_por_id(t_mensaje_localized_pokemon* localized_pokemon);
	void log_event_loggear_situacion_actual(matriz_recursos objetivos, matriz_recursos inventarios);
	void log_event_pokemon_mapeado(especie_pokemon);
	void log_event_loggear_pokemon_descartado(especie_pokemon);
	void log_event_de_donde_partio(entrenador*, t_posicion);
	void log_event_cpu_otorgado();
	void log_event_cpu_consumido();
	void log_event_pokemon_por_pedir(especie_pokemon);
	void log_event_mensaje_get_enviado(t_mensaje_get_pokemon*, t_id);

#endif
