#ifndef _TEAM_LOGGERS_
#define _TEAM_LOGGERS_

#include <commons/log.h>
#include <commons/config.h>

#include "../utils/mensajes.h"
#include "../hilos-del-team/detector_de_deadlock.h"
#include "estructuras-auxiliares/candidatos_intercambio.h"
#include "estructuras-auxiliares/captura_pendiente.h"
#include "estructuras-auxiliares/lector_config.h"
#include "estructuras-principales/pokemon.h"
#include "planificacion/planificacion.h"
#include "header_global_team.h"

	pthread_mutex_t Mutex_AndoLoggeando;
	pthread_mutex_t Mutex_AndoLoggeandoEventos;

//Constructor
	t_log* log_crear(char* PROPOSITO, char*KEY);

//Logs del enunciado
	void log_enunciado_entrenador_creado(entrenador*); //Seria el estado NEW
	void log_enunciado_cambio_de_cola(entrenador*, t_estado actual, t_estado final, const char* motivo);
	void log_enunciado_movimiento_de_un_entrenador(entrenador*);
	void log_enunciado_operacion_de_atrapar(entrenador*, pokemon*, t_posicion);
	void log_enunciado_operacion_de_intercambio(entrenador*, entrenador*, especie_pokemon cualDa, especie_pokemon cualRecibe);
	void log_enunciado_inicio_de_algoritmo_de_deteccion_de_deadlock();
	void log_enunciado_resultado_de_algoritmo_de_deteccion_de_deadlock(bool resultado);
	void log_enunciado_llegada_de_mensaje_appeared(t_mensaje_appeared_pokemon*);
	void log_enunciado_llegada_de_mensaje_localized(t_mensaje_localized_pokemon*);
	void log_enunciado_llegada_de_mensaje_caught(t_mensaje_caught_pokemon*);
	void log_enunciado_respuesta_autogenerada_catch();
	void log_enunciado_respuesta_autogenerada_get();
	void team_loggear_resultados();

//Eventos Random

	//GET
	void log_event_pokemon_por_pedir(especie_pokemon);
	void log_event_mensaje_get_enviado(t_mensaje_get_pokemon*, t_id);

	//CATCH
	void log_event_pokemon_por_catchear(pokemon* pokemonCatcheado);
	void log_event_mensaje_catch_enviado(t_mensaje_appeared_catch_pokemon* mensaje, t_id id);

	//LOCALIZED
	void log_event_localized_repetido(especie_pokemon);
	void log_event_localized_descartado_por_id(t_mensaje_localized_pokemon* localized_pokemon);

	//PROCESAR POKEMON
	void log_event_pokemon_mapeado(especie_pokemon);
	void log_event_busco_especie_en_mapa(pokemon* buscado, pokemon*encontrado);
	void log_event_loggear_pokemon_descartado(especie_pokemon);
	void log_event_loggear_situacion_actual(matriz_recursos objetivos, matriz_recursos inventarios);

	//CAUGHT
	void log_event_caught_especie(pokemon*pokemonCatcheado, bool fueExitosa);
	void log_event_captura_desconocida(t_mensaje_caught_pokemon*);

	//ENTRENADOR
	void log_event_de_donde_partio(entrenador*);
	void log_event_cpu_otorgado();
	void log_event_cpu_consumido();

#endif
