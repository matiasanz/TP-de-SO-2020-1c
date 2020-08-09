#include "mensajes.h"
#include "../team.h"

//Itera los objetivos, aplicando la funcion Get a cada uno. No editar esta funcion, sino la de arriba
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

	pthread_t hilo_get_sender;
	pthread_create(&hilo_get_sender, NULL, (void*) team_enviar_mensaje_get, string_duplicate(especiePokemon));
	pthread_detach(hilo_get_sender);
}

//Envia mensaje al broker para ser replicado al gamecard, devuelve el id del mensaje pendiente por recibir
void Catch(entrenador*unEntrenador, pokemon* pokemonCatcheado) {

	log_event_pokemon_por_catchear(pokemonCatcheado);

	captura_pendiente* captura = captura_crear(unEntrenador, pokemonCatcheado);

	unEntrenador->siguienteTarea = CAPTURAR;
	entrenador_pasar_a(unEntrenador, LOCKED_HASTA_CAUGHT, "Debera esperar a que este el resultado de la captura");

//	team_enviar_mensaje_catch(captura);

	pthread_t hilo_catch_sender;
	pthread_create(&hilo_catch_sender, NULL, (void*) team_enviar_mensaje_catch, captura);
	pthread_detach(hilo_catch_sender);
}

//****************************************************************************************************

void autoresponder_get(t_mensaje_get_pokemon* mensajeGet, t_id idMensajeFallido){

	sem_wait(&finDeSuscripcion);
	log_enunciado_respuesta_autogenerada_get();

	especie_pokemon especiePokemon = mensajeGet->especie;
	t_list*ningunaPosicion = list_create();

	t_mensaje_localized_pokemon* respuestaAutogenerada = mensaje_localized_pokemon_crear(especiePokemon, ningunaPosicion);
	mensaje_localized_pokemon_set_id_correlativo(respuestaAutogenerada, idMensajeFallido);
	localized_pokemon_recibido(respuestaAutogenerada);

	list_destroy(ningunaPosicion);

	sem_post(&finDeSuscripcion);
}

void autoresponder_catch(t_mensaje_catch_pokemon* mensajeCatch, t_id idCapturaPendiente){

	sem_wait(&finDeSuscripcion);
	t_mensaje_caught_pokemon* respuestaAutogenerada = mensaje_caught_pokemon_crear(true);
	mensaje_caught_pokemon_set_id_correlativo(respuestaAutogenerada, idCapturaPendiente);

	caught_pokemon_recibido(respuestaAutogenerada);

	sem_post(&finDeSuscripcion);

}

//****************************************************************************************************

void team_enviar_mensaje_get(especie_pokemon especiePokemon){

	t_mensaje_get_pokemon* mensajeGet=mensaje_get_pokemon_crear(especiePokemon);

	t_buffer* bufferDepaquete=mensaje_get_pokemon_serializar(mensajeGet);
	t_paquete* paqueteAEnviar=paquete_crear(MENSAJE,GET_POKEMON,bufferDepaquete);
	int idMensajeEnviado = enviar(conexion_broker,paqueteAEnviar);

	log_event_mensaje_get_enviado(mensajeGet, idMensajeEnviado);

	mensaje_get_registrar(idMensajeEnviado);

	if(error_conexion(idMensajeEnviado)){
		autoresponder_get(mensajeGet, idMensajeEnviado);
	}

	mensaje_get_pokemon_destruir(mensajeGet);
	paquete_destruir(paqueteAEnviar);
}

void mensaje_get_registrar(t_id idMensaje){
    t_id* idGet = malloc(sizeof(t_id));
    	* idGet = idMensaje;

    pthread_mutex_lock(&mutexPedidos);
    list_add(registroDePedidos, idGet);
    pthread_mutex_unlock(&mutexPedidos);
}

void team_enviar_mensaje_catch(captura_pendiente* captura){

	//creacion de  paquete catch pokemon y envio a Broker
	t_mensaje_catch_pokemon* mensajeCatch = mensaje_catch_pokemon_crear(captura->pokemonCatcheado);

	t_buffer* bufferDepaquete=mensaje_appeared_catch_pokemon_serializar(mensajeCatch);
	t_paquete* paqueteAEnviar=paquete_crear(MENSAJE,CATCH_POKEMON,bufferDepaquete);
	t_id idCapturaPendiente = enviar(conexion_broker,paqueteAEnviar);

	//Agrego a la lista de capturas pendientes
	agregar_captura_pendiente(captura, idCapturaPendiente);

	log_event_mensaje_catch_enviado(mensajeCatch, idCapturaPendiente);

	if(error_conexion(idCapturaPendiente)){
		log_enunciado_respuesta_autogenerada_catch();

		autoresponder_catch(mensajeCatch, idCapturaPendiente);
	}

	mensaje_appeared_catch_pokemon_destruir(mensajeCatch);
	paquete_destruir(paqueteAEnviar);
}


//****************************************************************************************************

void pendiente_set_id(captura_pendiente* captura, t_id idCaptura){
	captura->id = idCaptura;
}

void pendientes_agregar_pendiente(capturas_pendientes capturas, captura_pendiente* captura){
	cr_list_silent_add(capturas, captura);
}

void agregar_captura_pendiente(captura_pendiente* captura, t_id idCaptura){
	pendiente_set_id(captura, idCaptura);
	pendientes_agregar_pendiente(capturasPendientes, captura);
}

t_mensaje_catch_pokemon* mensaje_catch_pokemon_crear(pokemon*pokemonCatcheado){
	return mensaje_appeared_catch_pokemon_crear(pokemonCatcheado->especie,pokemonCatcheado->posicion.pos_x,pokemonCatcheado->posicion.pos_y);
}

captura_pendiente* captura_crear(entrenador*unEntrenador, pokemon*unPokemon){
	captura_pendiente* captura = malloc(sizeof(captura_pendiente));
					 * captura = pendiente_create(0, unEntrenador, unPokemon);
	return captura;
}

