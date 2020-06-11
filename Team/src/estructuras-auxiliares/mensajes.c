#include "mensajes.h"
#include "../team.h"

void* leer_mensaje_cuando_este_disponible(cr_list* unaLista){
	return cr_list_wait_and_remove(unaLista, 0);
}

void Get(void* especiePokemon) {
	//Envia mensaje al broker para ser replicado al gamecard
	printf("get(%s), ", (especie_pokemon) especiePokemon);
//	log_info(event_logger, ">> get(%s)\n", (especie_pokemon) especiePokemon);
}

//Itera los objetivos, aplicando la funcion Get a cada uno. No editar esta funcion, sino la de arriba
void Get_pokemones(matriz_recursos pokemones){

	void unGetPorPokemon(char* unaEspecie, void*cantidad){
		Get(unaEspecie);
	}

	dictionary_iterator(pokemones, unGetPorPokemon); puts("");
}

t_id unIDhardcodeado=0;

//Envia mensaje al broker para ser replicado al gamecard, devuelve el id del mensaje pendiente por recibir
void Catch(entrenador*unEntrenador, pokemon* pokemonCatcheado) {

	//TODO Consumir 1 CPU

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, ">> catch(%s)\n", pokemonCatcheado->especie);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

	//creacion de  paquete catch pokemon y envio a Broker
	t_mensaje_appeared_catch_pokemon* mensajeCatch=mensaje_appeared_catch_pokemon_crear(string_duplicate(pokemonCatcheado->especie),pokemonCatcheado->posicion.pos_x,pokemonCatcheado->posicion.pos_y);

	t_paquete_header header=paquete_header_crear(MENSAJE,TEAM,CATCH_POKEMON);
	t_buffer* bufferDepaquete=mensaje_appeared_catch_pokemon_serializar(mensajeCatch);
	t_paquete* paqueteAEnviar=paquete_crear(header,bufferDepaquete);
	t_conexion_server* unaConexion=conexion_server_crear(conexion_broker->ip, conexion_broker->puerto, TEAM);
	int resultadoDeEnvio = enviar(unaConexion,paqueteAEnviar);

	//Agrego a la lista de capturas pendientes
	t_id idCapturaPendiente = mensaje_appeared_catch_pokemon_get_id(mensajeCatch);
	agregar_pendiente(capturasPendientes, idCapturaPendiente, unEntrenador, pokemonCatcheado);

	if(resultadoDeEnvio==ERROR_SOCKET){
		pthread_mutex_lock(&Mutex_AndoLoggeando);
		log_warning(logger,"No se ha podido realizar la conexion con el Broker" /*, se procedera a responder el mensaje por defecto"*/);
		pthread_mutex_unlock(&Mutex_AndoLoggeando);

//		t_mensaje_caught_pokemon* respuestaAutogenerada = mensaje_caught_pokemon_crear(true);
//		mensaje_caught_pokemon_set_id_correlativo(respuestaAutogenerada, idCapturaPendiente);
//		cr_list_add_and_signal(mensajesCAUGHT, respuestaAutogenerada);
	}

	mensaje_appeared_catch_pokemon_destruir(mensajeCatch);
	paquete_destruir(paqueteAEnviar);
}
