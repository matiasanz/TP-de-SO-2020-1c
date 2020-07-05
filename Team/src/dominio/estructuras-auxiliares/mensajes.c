#include "mensajes.h"
#include "../../team.h"

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

void Get(void* especiePokemon) {

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, ">> get(%s)\n", (especie_pokemon) especiePokemon);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

	t_mensaje_get_pokemon* mensajeGet=mensaje_get_pokemon_crear(string_duplicate(especiePokemon));

	t_paquete_header header=paquete_header_crear(MENSAJE,TEAM,GET_POKEMON);
	t_buffer* bufferDepaquete=mensaje_get_pokemon_serializar(mensajeGet);
	t_paquete* paqueteAEnviar=paquete_crear(header,bufferDepaquete);
	int idMensajeEnviado = enviar(conexion_broker,paqueteAEnviar);

	mensaje_get_registrar(idMensajeEnviado);

	if(error_conexion(idMensajeEnviado)){
		pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
		log_warning(event_logger,"Se procedera a responder el mensaje GET por defecto");
		pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

		t_mensaje_localized_pokemon* respuestaAutogenerada = mensaje_localized_pokemon_crear(string_duplicate(especiePokemon), list_create());
		mensaje_localized_pokemon_set_id_correlativo(respuestaAutogenerada, idMensajeEnviado);
		localized_pokemon_recibido(respuestaAutogenerada);
	}

	mensaje_get_pokemon_destruir(mensajeGet);
	paquete_destruir(paqueteAEnviar);
}

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

//Envia mensaje al broker para ser replicado al gamecard, devuelve el id del mensaje pendiente por recibir
void Catch(entrenador*unEntrenador, pokemon* pokemonCatcheado) {

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, ">> catch(%s)", pokemonCatcheado->especie);
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

	//creacion de  paquete catch pokemon y envio a Broker
	t_mensaje_appeared_catch_pokemon* mensajeCatch=mensaje_appeared_catch_pokemon_crear(string_duplicate(pokemonCatcheado->especie),pokemonCatcheado->posicion.pos_x,pokemonCatcheado->posicion.pos_y);

	t_paquete_header header=paquete_header_crear(MENSAJE,TEAM,CATCH_POKEMON);
	t_buffer* bufferDepaquete=mensaje_appeared_catch_pokemon_serializar(mensajeCatch);
	t_paquete* paqueteAEnviar=paquete_crear(header,bufferDepaquete);
	t_id idCapturaPendiente = enviar(conexion_broker,paqueteAEnviar);

	//Agrego a la lista de capturas pendientes
	agregar_pendiente(capturasPendientes, idCapturaPendiente, unEntrenador, pokemonCatcheado);

	unEntrenador->siguienteTarea = CAPTURAR;
	entrenador_pasar_a(unEntrenador, LOCKED_HASTA_CAUGHT, "Debera esperar a que este el resultado de la captura");

	sem_post(&FinDeCiclo_CPU);

	if(error_conexion(idCapturaPendiente)){
		pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
		log_warning(event_logger,"Se procedera a responder el mensaje CATCH por defecto");
		pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);

		t_mensaje_caught_pokemon* respuestaAutogenerada = mensaje_caught_pokemon_crear(true);
		mensaje_caught_pokemon_set_id_correlativo(respuestaAutogenerada, idCapturaPendiente);

		caught_pokemon_recibido(respuestaAutogenerada);
	}

	mensaje_appeared_catch_pokemon_destruir(mensajeCatch);
	paquete_destruir(paqueteAEnviar);
}
