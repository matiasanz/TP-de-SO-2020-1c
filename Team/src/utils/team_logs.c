#include "../utils/team_logs.h"

#include "../team.h"


void log_enunciado_entrenador_creado(entrenador* unEntrenador){

	char*posicion = posicion_to_string(unEntrenador->posicion);

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "Se cargo al Entrenador N°%u en estado NEW, en la posicion %s", unEntrenador->id, posicion);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);

	free(posicion);
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
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El Entrenador N°%u le intercambio al N°%u un %s por un %s", unEntrenador->id, pareja->id, cualDa, cualRecibe);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

void log_enunciado_inicio_de_algoritmo_de_deteccion_de_deadlock(){
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger,  "Se ejecuto el algoritmo de deteccion de deadlock");
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

void log_enunciado_respuesta_autogenerada_catch(){
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_warning(logger,"Se procedera a responder el mensaje CATCH por defecto");
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

void log_enunciado_respuesta_autogenerada_get(){
	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_warning(logger,"Se procedera a responder el mensaje GET por defecto");
	pthread_mutex_unlock(&Mutex_AndoLoggeando);
}

void team_loggear_resultados(){

	char*resultados = estadisticas_to_string(Estadisticas);
	log_info(logger, "\n*************** Resultados del Team ****************\n\n%s", resultados);
	free(resultados);
}

//**************************************************************

void log_event_inicio_proceso(char*NombreEquipo){
	log_info(event_logger, "\n\n*************************************************************************\n"
         		     "                        Inicio del proceso Team %s\n\n", NombreEquipo);
}

void log_event_fin_del_proceso(){
	log_info(logger, "\n\n                              Fin del proceso Team\n"
						      "****************************************************************************");
}

void log_event_localized_repetido(especie_pokemon especie){ //TODO localized to string
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "\n********* MENSAJE DESCARTADO ********\n"
				" Tipo: localized\n"
				" Motivo: La especie %s habia sido recibida con anterioridad", especie);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

void log_event_loggear_situacion_actual(matriz_recursos objetivos, matriz_recursos inventarios){
	char*objetivosString = recursos_to_string(objetivos);
	char*inventarioString = recursos_to_string(inventarios);

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "************** SITUACION ACTUAL ********************\n"
		" Objetivos: %s\n"
		" Inventario:%s", objetivosString, inventarioString);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);


	free(objetivosString);
	free(inventarioString);
}

void log_event_pokemon_mapeado(char* especie){
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "****** Agrego repuesto -> %s\n\n", especie);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

void log_event_loggear_pokemon_descartado(char* especie){
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger,  "Se recibio un %s y se descarto al no ser requerido\n", especie);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

void log_event_de_donde_partio(entrenador*unEntrenador){
	char*posicion = posicion_to_string(unEntrenador->posicion);
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger,  "El entrenador N°%u partio de la posicion%s\n", unEntrenador->id, posicion);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

	free(posicion);
}

//Planificador
void log_event_cpu_otorgado(){
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger,  "--> LE DOY CPU");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

}

void log_event_cpu_consumido(){
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "--> CONSUMIO CPU");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

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
//	mensaje_get_pokemon_set_id(mensaje, id);
//	char* mensajeGet = mensaje_get_pokemon_to_string(mensaje);
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "\n*********** MENSAJE ENVIADO ***********\n"
				"Tipo: GET\n"
				"Especie: %s\n"
				"Id: %u\n\n", mensaje->especie, id);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

//	free(mensajeGet);
}

void log_event_pokemon_por_catchear(pokemon* pokemonCatcheado){
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, ">> catch(%s)", pokemonCatcheado->especie);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

}

void log_event_mensaje_catch_enviado(t_mensaje_appeared_catch_pokemon* mensaje, t_id id){

//	mensaje_appeared_catch_pokemon_set_id(mensaje, id);
//
//	char* mensajeCatch = mensaje_appeared_catch_pokemon_to_string(mensaje, "CATCH");

	char*posicion = posicion_to_string(mensaje->pokemon.posicion);
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "\n*********** MENSAJE ENVIADO ***********\n"
			    "Tipo: CATCH"
			    "Especie: %s\n"
			    "Posicion: %s\n"
			    "ID: %u", mensaje->pokemon.especie, posicion, id);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

	free(posicion);
//	free(mensajeCatch);
}

void log_event_caught_especie(pokemon*pokemonCatcheado, bool fueExitosa){
	log_info(event_logger, "CAUGHT %s: %s", pokemonCatcheado->especie, (fueExitosa? "OK": "FAIL"));
}

void log_event_captura_desconocida(t_mensaje_caught_pokemon*mensaje){

	t_id idCaptura = mensaje_caught_pokemon_get_id_correlativo(mensaje);
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Se recibio el resultado de una captura id %u desconocida\n", idCaptura);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

}

void log_event_busco_especie_en_mapa(pokemon* unPokemon, pokemon*pokemonCatcheado){
//	printf("%s esta en mapa si: %s == %s\n", pokemonCatcheado->especie, unPokemon->especie, pokemonCatcheado->especie);
}

void log_event_entrenador_por_ejecutar(entrenador*proximoEntrenador){
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "signal(entrenador %u)\n", proximoEntrenador->id);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

}

void log_event_entrenador_termino_de_ejecutar(entrenador*unEntrenador){
	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "--> TERMINO DE EJECUTAR");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

}

void log_event_entrenador_creado(entrenador*unEntrenador){
//	printf("Se agrego un entrenador en [%u %u] con objetivos:", unEntrenador->posicion.pos_x, unEntrenador->posicion.pos_y);
//	recursos_mostrar(unEntrenador->objetivos);
//
//	printf(" e inventario:");
//	recursos_mostrar(unEntrenador->pokemonesCazados);
//	printf("\n");
}

void log_event_situacion_del_entrenador(entrenador* unEntrenador){
	char* objetivosString = recursos_to_string(unEntrenador->objetivos);
	char* inventarioString = recursos_to_string(unEntrenador->pokemonesCazados);

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, " Objetivos: %s\n Inventarios: %s\n\n", objetivosString, inventarioString);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);


	free(objetivosString);
	free(inventarioString);
}

//***********************************************************************

void validar_pokemon(pokemon*pokemonCatcheado, t_id pid){
	if(!pokemonCatcheado){
		log_error(event_logger, "El entrenador N°%u intento capturar un pokemon nulo\n", pid);
		exit(1);
	}
}

void validar_entrenador(entrenador*unEntrenador){
	if(!unEntrenador){
		log_error(event_logger, "Se intento leer un entrenador nulo");
		exit(1);
	}
}

//*******************************************************************************

// Idea descartada por memory leaks

//static void log_synchronized(t_log*unLogger, char*mensaje, va_list args, pthread_mutex_t* mutex){
//	char* log = string_from_vformat(mensaje, args);
//	pthread_mutex_lock(mutex);
//	log_info(unLogger, log);
//	pthread_mutex_unlock(mutex);
//	free(log);
//}

//void log_enunviado(logger, char*mensaje, ...){
//	va_list args;
//	va_start(args, mensaje);
//	log_synchronized(logger, mensaje, args, &Mutex_AndoLoggeando);
//	va_end(args);
//}
//
//void log_event(char*mensaje, ...){
//	va_list args;
//	va_start(args, mensaje);
//	log_synchronized(event_logger, mensaje, args, &Mutex_AndoLoggeandoEventos);
//	va_end(args);
//}
