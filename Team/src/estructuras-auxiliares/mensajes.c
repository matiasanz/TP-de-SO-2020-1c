#include "mensajes.h"
#include "../team.h"

void* leer_mensaje_cuando_este_disponible(cr_list* unaLista){
	return cr_list_wait_and_remove(unaLista, 0);
}

void Get(void* especiePokemon) {
	//Envia mensaje al broker para ser replicado al gamecard
	log_info(event_logger, ">> get(%s)\n", (especie_pokemon) especiePokemon);
	t_mensaje_get_pokemon* mensajeGet=mensaje_get_pokemon_crear(string_duplicate(especiePokemon));

	t_paquete_header header=paquete_header_crear(MENSAJE,TEAM,GET_POKEMON);
	t_buffer* bufferDepaquete=mensaje_get_pokemon_serializar(mensajeGet);
	t_paquete* paqueteAEnviar=paquete_crear(header,bufferDepaquete);
	int resultadoDeEnvio = enviar(conexion_broker,paqueteAEnviar);

//TODO t_id* idGet = malloc(sizeof(t_id)); *idGet = mensaje_get_pokemon_get_id(mensajeGet); list_add(getsPendientes, idGet); //y funcion que compare en localized

	if(resultadoDeEnvio==ERROR_SOCKET){
		pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
		log_warning(event_logger,"Se procedera a responder el mensaje GET por defecto");
		pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
		t_mensaje_localized_pokemon* respuestaAutogenerada = mensaje_localized_pokemon_crear(string_duplicate(especiePokemon), list_create());
		cr_list_add_and_signal(mensajesLOCALIZED, respuestaAutogenerada);
	}

	mensaje_get_pokemon_destruir(mensajeGet);
	paquete_destruir(paqueteAEnviar);
}

//Itera los objetivos, aplicando la funcion Get a cada uno. No editar esta funcion, sino la de arriba
void Get_pokemones(matriz_recursos objetivosTotales, matriz_recursos recursosDisponibles){

	matriz_recursos necesidadDelEquipo = recursos_matriz_diferencia(objetivosTotales, recursosDisponibles);

	void unGetPorPokemon(char* unaEspecie, void*cantidad){
		if(*((numero*)cantidad) > 0){
			sleep(RETARDO_CICLO_CPU); //Cada get consume 1 cpu
			Get(unaEspecie);
		}
	}

	dictionary_iterator(necesidadDelEquipo, unGetPorPokemon); puts("");
	recursos_destroy(necesidadDelEquipo);
}

//Envia mensaje al broker para ser replicado al gamecard, devuelve el id del mensaje pendiente por recibir
void Catch(entrenador*unEntrenador, pokemon* pokemonCatcheado) {

	//TODO Consumir 1 CPU

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, ">> catch(%s)", pokemonCatcheado->especie);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

	//creacion de  paquete catch pokemon y envio a Broker
	t_mensaje_appeared_catch_pokemon* mensajeCatch=mensaje_appeared_catch_pokemon_crear(string_duplicate(pokemonCatcheado->especie),pokemonCatcheado->posicion.pos_x,pokemonCatcheado->posicion.pos_y);

	t_paquete_header header=paquete_header_crear(MENSAJE,TEAM,CATCH_POKEMON);
	t_buffer* bufferDepaquete=mensaje_appeared_catch_pokemon_serializar(mensajeCatch);
	t_paquete* paqueteAEnviar=paquete_crear(header,bufferDepaquete);
	int resultadoDeEnvio = ERROR_SOCKET;//enviar(conexion_broker,paqueteAEnviar); //TODO DESCOMENTAR

	//Agrego a la lista de capturas pendientes
	t_id idCapturaPendiente = mensaje_appeared_catch_pokemon_get_id(mensajeCatch);
	agregar_pendiente(capturasPendientes, idCapturaPendiente, unEntrenador, pokemonCatcheado);

	unEntrenador->siguienteTarea = CAPTURAR;
	entrenador_pasar_a(unEntrenador, LOCKED_HASTA_CAUGHT, "Debera esperar a que este el resultado de la captura");

	if(resultadoDeEnvio==ERROR_SOCKET){
		pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
		log_warning(event_logger,"Se procedera a responder el mensaje CATCH por defecto");
		pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

		t_mensaje_caught_pokemon* respuestaAutogenerada = mensaje_caught_pokemon_crear(true);
		mensaje_caught_pokemon_set_id_correlativo(respuestaAutogenerada, idCapturaPendiente);
		cr_list_add_and_signal(mensajesCAUGHT, respuestaAutogenerada);
	}

	mensaje_appeared_catch_pokemon_destruir(mensajeCatch);
	paquete_destruir(paqueteAEnviar);
}
