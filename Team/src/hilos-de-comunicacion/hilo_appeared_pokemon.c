
#include "../hilos-de-comunicacion/hilo_appeared_pokemon.h"
#include "../team.h"
#define ID_GAMEBOY 999

void subscribir_y_escuchar_cola_appeared_pokemon(void (*callback)(t_id_cola, void*)) {

	conexion_appeared_pokemon = conexion_cliente_crear(APPEARED_POKEMON,
			config_get_int_value(config, "TIEMPO_RECONEXION"),
			(void*) callback);

	t_conexion* args = conexion_crear(conexion_broker,
			conexion_appeared_pokemon);

	pthread_create(&hilo_appeared_pokemon, NULL,
			(void*) subscribir_y_escuchar_cola, args);

	pthread_detach(hilo_appeared_pokemon);

}

void appeared_pokemon_recibido(t_mensaje_appeared_pokemon* appeared_pokemon) {

	mensaje_appeared_catch_pokemon_log(logger, appeared_pokemon, APPEARED_POKEMON_STRING);



	if(mensaje_appeared_responde_a_mi_pedido(appeared_pokemon)){

		pokemon*pokemonRecibido = leer_pokemon(appeared_pokemon);

		registrar_pokemon(pokemonRecibido);
	}

	else{
		pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
		log_info(event_logger, "Se descarto el mensaje APPEARED ya que el id no coincide con un get");
		pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
	}

	mensaje_appeared_catch_pokemon_destruir(appeared_pokemon);
}

bool mensaje_appeared_viene_del_gameboy(t_mensaje_appeared_pokemon*unMensaje){
	t_id idMensaje = mensaje_appeared_catch_pokemon_get_id(unMensaje);
	return idMensaje == ID_GAMEBOY;
}

bool mensaje_appeared_responde_a_mi_pedido(t_mensaje_appeared_pokemon*unMensaje){
	t_id idCorrelativo = mensaje_appeared_catch_pokemon_get_id_correlativo(unMensaje);
	return id_responde_a_mi_pedido(idCorrelativo);
}

pokemon*leer_pokemon(t_mensaje_appeared_pokemon*unMensaje){
	return pokemon_ptr_create(string_duplicate(unMensaje->pokemon.especie), unMensaje->pokemon.posicion);
}
