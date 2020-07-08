#include "team_logs.h"
#include "../team.h"

void log_enunciado_entrenador_creado(entrenador* unEntrenador){

	char*posicion = posicion_to_string(unEntrenador->posicion);

	char* log = string_from_format("Se cargo al Entrenador N°%u en estado NEW, en la posicion %s", unEntrenador->id, posicion);
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, log);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);

	free(posicion);
	free(log);
}

void log_enunciado_cambio_de_cola(entrenador*unEntrenador, t_estado estadoActual, t_estado estadoFinal, const char* motivo){
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "\n--------------- Cambio de Estado ---------------\n"
						" Proceso: Entrenador N°%u\n"
						" Estado Inicial: %s\n"
						" Estado Final: %s\n"
						" Motivo: %s\n"
						"", unEntrenador->id, estadoFromEnum(estadoActual),  estadoFromEnum(estadoFinal), motivo);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

void log_enunciado_operacion_de_atrapar(entrenador* unEntrenador, pokemon* victima, t_posicion posicionDelEvento){

	char*posicion = posicion_to_string(posicionDelEvento);

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El Entrenador N°%u ha capturado un %s en la posicion%s", unEntrenador->id, victima->especie, posicion);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);

	free(posicion);
}

void log_enunciado_movimiento_de_un_entrenador(entrenador* unEntrenador){
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "\nEl entrenador N°%u se desplazo a la posicion [%u %u]\n", unEntrenador->id, unEntrenador->posicion.pos_x, unEntrenador->posicion.pos_y);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

void log_enunciado_operacion_de_intercambio(entrenador*unEntrenador, entrenador*pareja, especie_pokemon cualDa, especie_pokemon cualRecibe){
	log_info(logger, "El Entrenador N°%u le intercambio al N°%u un %s por un %s", unEntrenador->id, pareja->id, cualDa, cualRecibe);
}

void log_enunciado_inicio_de_algoritmo_de_deteccion_de_deadlock(){
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "Se ejecuto el algoritmo de deteccion de deadlock");
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

void log_enunciado_resultado_de_algoritmo_de_deteccion_de_deadlock(bool resultado){
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "Resultado de la deteccion: %s", ( resultado? "Positivo": "Negativo") );
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

void log_enunciado_llegada_de_mensaje_appeared(t_mensaje_appeared_pokemon* mensaje){
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	mensaje_appeared_catch_pokemon_log(logger, mensaje, APPEARED_POKEMON_STRING);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

void log_enunciado_llegada_de_mensaje_localized(t_mensaje_localized_pokemon* localized_pokemon){
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	mensaje_localized_pokemon_log(logger, localized_pokemon);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

void log_enunciado_llegada_de_mensaje_caught(t_mensaje_caught_pokemon* mensajeRecibido){
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	mensaje_caught_pokemon_log(logger, mensajeRecibido);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

void team_loggear_resultados(){

	char*resultados = estadisticas_to_string(Estadisticas);

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "\n*************** Resultados del Team ****************\n\n%s", resultados);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);

	free(resultados);
}

//**************************************************************

void log_event_localized_repetido(especie_pokemon especie){
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "\n********* MENSAJE DESCARTADO ********\n"
							  " Tipo: localized\n"
							  " Motivo: La especie %s habia sido recibida con anterioridad", especie);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

void log_event_loggear_situacion_actual(matriz_recursos objetivos, matriz_recursos inventarios){
	puts("************** SITUACION ACTUAL ********************");
	printf("Objs: "); recursos_mostrar(objetivosGlobales); puts("");
	printf("Invs: "); recursos_mostrar(inventariosGlobales);puts("");
}

void log_event_pokemon_mapeado(char* especie){
	printf("************* Agrego repuesto -> %s\n\n", especie);
}

void log_event_loggear_pokemon_descartado(char* especie){
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Se recibio un %s y se descarto al no ser requerido\n", especie);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

void log_event_de_donde_partio(entrenador*unEntrenador, t_posicion deDonde){
	char*posicion = posicion_to_string(deDonde);

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El entrenador N°%u partio de la posicion%s\n", unEntrenador->id, posicion);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);

	free(posicion);
}

//Planificador
void log_event_cpu_otorgado(){
	puts("***************************************** LE DOY CPU");
}

void log_event_cpu_consumido(){
	puts("***************************************** CONSUMIO CPU");
}

//Mensajes
void log_event_localized_descartado_por_id(t_mensaje_localized_pokemon* localized_pokemon){
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	mensaje_localized_pokemon_log(event_logger, localized_pokemon);
	log_warning(event_logger, "Se descarto un mensaje LOCALIZED por id correlativo desconocido");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

void log_event_pokemon_por_pedir(especie_pokemon especiePokemon){
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, ">> get(%s)\n", especiePokemon);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

void log_event_mensaje_get_enviado(t_mensaje_get_pokemon* mensaje, t_id id){
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "\n*********** MENSAJE ENVIADO ***********\n"
						   " Tipo: GET\n"
						   " Especie: %s\n"
						   " Id: %u\n", mensaje->especie, id);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

void log_event_caught_especie(pokemon*pokemonCatcheado, bool fueExitosa){
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "CAUGHT %s: %s", pokemonCatcheado->especie, (fueExitosa? "OK": "FAIL"));
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

void log_event_captura_desconocida(t_mensaje_caught_pokemon*mensaje){

	t_id idCaptura = mensaje_caught_pokemon_get_id_correlativo(mensaje);

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Se recibio el resultado de una captura id %u desconocida\n", idCaptura);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

void log_event_busco_especie_en_mapa(pokemon* unPokemon, pokemon*pokemonCatcheado){
	printf("%s esta en mapa si: %s == %s\n", pokemonCatcheado->especie, unPokemon->especie, pokemonCatcheado->especie);
}