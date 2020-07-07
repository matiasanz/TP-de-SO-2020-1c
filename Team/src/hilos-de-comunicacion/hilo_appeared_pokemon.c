
#include "../hilos-de-comunicacion/hilo_appeared_pokemon.h"
#include "../team.h"

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

	log_enunciado_mensaje_appeared(appeared_pokemon);

	pokemon*pokemonRecibido = leer_pokemon(appeared_pokemon);

	registrar_pokemon(pokemonRecibido);

	mensaje_appeared_catch_pokemon_destruir(appeared_pokemon);
}

pokemon*leer_pokemon(t_mensaje_appeared_pokemon*unMensaje){
	return pokemon_ptr_create(string_duplicate(unMensaje->pokemon.especie), unMensaje->pokemon.posicion);
}
