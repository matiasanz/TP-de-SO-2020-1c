#include "mensajes.h"
#include "../team.h"

void team_enviar_mensaje_get(especie_pokemon especiePokemon);

void* leer_mensaje_cuando_este_disponible(cr_list* unaLista){
	return cr_list_wait_and_remove(unaLista, 0);
}

void mensaje_get_registrar(t_id idMensaje){
    t_id* idGet = malloc(sizeof(t_id));
    	* idGet = idMensaje;

    pthread_mutex_lock(&mutexPedidos);
    list_add(registroDePedidos, idGet);
    pthread_mutex_unlock(&mutexPedidos);
}

t_paquete_header team_crear_header(t_id_cola TipoMensaje){
	return paquete_header_crear(MENSAJE, TEAM, TipoMensaje, id_proceso);
}

//Itera los objetivos, aplicando la funcion Get a cada uno
void Get_pokemones(matriz_recursos objetivosTotales, matriz_recursos recursosDisponibles){

	pthread_mutex_lock(&mutexRecursosDisponibles);
	matriz_recursos necesidadDelEquipo = recursos_matriz_diferencia(objetivosTotales, recursosDisponibles);
	pthread_mutex_unlock(&mutexRecursosDisponibles);

	void unGetPorPokemon(char* unaEspecie, void*cantidad){
		if(*((numero*)cantidad) > 0){
			consumir_ciclo_cpu();
			Get(unaEspecie);
		}
	}

	dictionary_iterator(necesidadDelEquipo, unGetPorPokemon);
	recursos_destroy(necesidadDelEquipo);
}

void Get(void* especiePokemon) {

	log_event_pokemon_por_pedir(especiePokemon);

	team_enviar_mensaje_get(string_duplicate(especiePokemon));

//	pthread_t hilo_get_sender;
//	pthread_create(&hilo_get_sender, NULL, (void*) team_enviar_mensaje_get, string_duplicate(especiePokemon));
//	pthread_detach(hilo_get_sender);
}

void team_enviar_mensaje_get(especie_pokemon especiePokemon){

	t_mensaje_get_pokemon* mensajeGet=mensaje_get_pokemon_crear(especiePokemon);

	t_paquete_header header= team_crear_header(GET_POKEMON);
	t_buffer* bufferDepaquete=mensaje_get_pokemon_serializar(mensajeGet);
	t_paquete* paqueteAEnviar=paquete_crear(header,bufferDepaquete);
	int idMensajeEnviado = enviar(conexion_broker,paqueteAEnviar);

	log_event_mensaje_get_enviado(mensajeGet, idMensajeEnviado);

	mensaje_get_registrar(idMensajeEnviado);

	if(error_conexion(idMensajeEnviado)){

		log_enunciado_respuesta_autogenerada_get();
		t_list*ningunaPosicion = list_create();

		t_mensaje_localized_pokemon* respuestaAutogenerada = mensaje_localized_pokemon_crear(especiePokemon, ningunaPosicion);
		mensaje_localized_pokemon_set_id_correlativo(respuestaAutogenerada, idMensajeEnviado);
		localized_pokemon_recibido(respuestaAutogenerada);

		list_destroy(ningunaPosicion);
	}

	mensaje_get_pokemon_destruir(mensajeGet);
	paquete_destruir(paqueteAEnviar);
	free(especiePokemon);
}

void team_enviar_mensaje_catch(pokemon*pokemonCatcheado){
//TODO
}

//Envia mensaje al broker para ser replicado al gamecard, devuelve el id del mensaje pendiente por recibir
void Catch(entrenador*unEntrenador, pokemon* pokemonCatcheado) {

	log_event_pokemon_por_catchear(pokemonCatcheado);

	//creacion de  paquete catch pokemon y envio a Broker
	t_mensaje_appeared_catch_pokemon* mensajeCatch=mensaje_appeared_catch_pokemon_crear(pokemonCatcheado->especie,pokemonCatcheado->posicion.pos_x,pokemonCatcheado->posicion.pos_y);

	t_paquete_header header= team_crear_header(CATCH_POKEMON);
	t_buffer* bufferDepaquete=mensaje_appeared_catch_pokemon_serializar(mensajeCatch);
	t_paquete* paqueteAEnviar=paquete_crear(header,bufferDepaquete);
	t_id idCapturaPendiente = enviar(conexion_broker,paqueteAEnviar);

	//Agrego a la lista de capturas pendientes
	agregar_pendiente(capturasPendientes, idCapturaPendiente, unEntrenador, pokemonCatcheado);

	unEntrenador->siguienteTarea = CAPTURAR;
	entrenador_pasar_a(unEntrenador, LOCKED_HASTA_CAUGHT, "Debera esperar a que este el resultado de la captura");

	log_event_mensaje_catch_enviado(mensajeCatch, idCapturaPendiente);

	sem_post(&FinDeCiclo_CPU);

	if(error_conexion(idCapturaPendiente)){
		log_enunciado_respuesta_autogenerada_catch();

		t_mensaje_caught_pokemon* respuestaAutogenerada = mensaje_caught_pokemon_crear(true);
		mensaje_caught_pokemon_set_id_correlativo(respuestaAutogenerada, idCapturaPendiente);

		caught_pokemon_recibido(respuestaAutogenerada);
	}

	mensaje_appeared_catch_pokemon_destruir(mensajeCatch);
	paquete_destruir(paqueteAEnviar);
}
