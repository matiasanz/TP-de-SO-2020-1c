
#include "../suscripciones/hilo_localized_pokemon.h"

#include "../team.h"

void suscribir_y_escuchar_cola_localized_pokemon(void (*callback)(t_id_cola, void*)) {

	conexion_localized_pokemon = conexion_cliente_crear(LOCALIZED_POKEMON, TIEMPO_RECONEXION, callback);

	t_conexion* args_s = conexion_crear(conexion_broker, conexion_localized_pokemon);

	pthread_create(&hilo_localized_pokemon, NULL, (void*) suscribir_y_escuchar_cola, args_s);
	pthread_detach(hilo_localized_pokemon);
}

void localized_pokemon_recibido(t_mensaje_localized_pokemon* localized_pokemon) {

	if(mensaje_localized_es_para_mi(localized_pokemon)){
		log_enunciado_llegada_de_mensaje_localized(localized_pokemon);

		mensaje_localized_pokemon_procesar(localized_pokemon);
	}

	else{
		log_event_localized_descartado_por_id(localized_pokemon);
		mensaje_localized_pokemon_destruir(localized_pokemon);
	}

	if(!pedidos_pendientes()){
		borrar_historial();
	}
}

void mensaje_localized_pokemon_procesar(t_mensaje_localized_pokemon* pokemonLocalizado){

	if(!especie_recibida_con_anterioridad(pokemonLocalizado->especie, historialDePokemones)){
		especie_pokemon especie = pokemonLocalizado->especie;
		posiciones_ordenar_por_cercania_al_equipo(pokemonLocalizado->posiciones);
		registrar_en_cada_posicion(especie, pokemonLocalizado->posiciones);
	}

	else{
		log_event_localized_repetido(pokemonLocalizado->especie);
	}

	mensaje_localized_pokemon_destruir(pokemonLocalizado);
}

/*************************************** Funciones Auxiliares ************************************************/

void registrar_en_cada_posicion(especie_pokemon unaEspecie, posiciones listaDePosiciones){

	void registrarEnCadaPosicion(t_posicion* pos){
		pokemon* unPokemon = pokemon_ptr_create(string_duplicate(unaEspecie), *pos);
		registrar_pokemon(unPokemon);
	}

	list_iterate(listaDePosiciones, (void(*)(void*)) registrarEnCadaPosicion);
}

void posiciones_ordenar_por_cercania_al_equipo(posiciones listaPosiciones){
	bool cercaniaAEntrenadores(void*unaPos, void*otraPos){
		return posicion_distancia_a_equipo(unaPos) >= posicion_distancia_a_equipo(otraPos);
	}

	list_sort(listaPosiciones, &cercaniaAEntrenadores);
}


numero posicion_distancia_a_equipo(t_posicion*posicion){
	return posicion_distancia_a_entrenadores(posicion, equipo);
}

numero posicion_distancia_a_entrenadores(t_posicion* posicion, entrenadores unEquipo){

	numero distancia = NUMERO_MAXIMO;

	void menorDistancia(void*unEntrenador){
		t_id eid = ((entrenador*)unEntrenador)->id;
		pthread_mutex_lock(&mutexPosicionEntrenador[eid]);
		t_posicion posicionDelEntrenador = ((entrenador*)unEntrenador)->posicion;
		pthread_mutex_unlock(&mutexPosicionEntrenador[eid]);

		numero distanciaAlEntrenador = posicion_distancia(*posicion, posicionDelEntrenador);

		if(distancia > distanciaAlEntrenador) distancia = distanciaAlEntrenador;
	}

	pthread_mutex_lock(&mutexEntrenadores);
	list_iterate(equipo, &menorDistancia);
	pthread_mutex_unlock(&mutexEntrenadores);

	return distancia;
}

/*************************************** Historial de Pokemones ************************************************/

void agregar_al_historial(pokemon*unPokemon){
	especie_recibida_con_anterioridad(unPokemon->especie, historialDePokemones);
}

bool pedidos_pendientes(){
	pthread_mutex_lock(&mutexPedidos);
	bool pedidosPendientes = !list_is_empty(registroDePedidos);
	pthread_mutex_unlock(&mutexPedidos);

	return pedidosPendientes;
}

// Agrega los pokemones que van llegando al mapa en caso de ser requeridos
void registrar_pokemon(pokemon*unPokemon){
	if(pedidos_pendientes()) agregar_al_historial(unPokemon);

	mapa_mapear_pokemon(pokemonesRecibidos, unPokemon);
}

// Retorna true si la especie ya se encuentra en el registro de especies
bool especie_recibida_con_anterioridad(especie_pokemon especie, especies_pokemones historial){

	pthread_mutex_lock(&mutexHistorialEspecies);
	bool yaLaTengo = list_get_by_comparation(historial, especie, (bool(*)(void*, void*))especie_cmp);
	if(!yaLaTengo)  list_add(historial, string_duplicate(especie));
	pthread_mutex_unlock(&mutexHistorialEspecies);

	return yaLaTengo;
}

bool mensaje_localized_es_para_mi(t_mensaje_localized_pokemon* unMensaje){
	t_id idCorrelativo = mensaje_localized_pokemon_get_id_correlativo(unMensaje);
	return id_responde_a_mi_pedido(idCorrelativo);
}

bool id_responde_a_mi_pedido(t_id idCorrelativo){

	bool cmp(void*unId, void*otroId){
		return *((numero*)unId) == *((numero*)otroId);
	}

	pthread_mutex_lock(&mutexPedidos);
	t_id* idGet = list_remove_by_comparation(registroDePedidos, &idCorrelativo, &cmp);
	pthread_mutex_unlock(&mutexPedidos);

	if(idGet) free(idGet);

	return idGet;
}

void borrar_historial(){
	pthread_mutex_lock(&mutexHistorialEspecies);
	list_clean_and_destroy_elements(historialDePokemones, free);
	pthread_mutex_unlock(&mutexHistorialEspecies);
}
