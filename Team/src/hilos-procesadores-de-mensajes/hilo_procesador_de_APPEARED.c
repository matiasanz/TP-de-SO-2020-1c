#include "../hilos-del-team/hilos_team.h"
#include "../team.h"
#include "../estructuras-auxiliares/mensajes.h"

typedef t_mensaje_appeared_catch_pokemon t_mensaje_appeared_pokemon;

pokemon*desempaquetar_pokemon(t_mensaje_appeared_pokemon*unMensaje){
	return pokemon_ptr_create(string_duplicate(unMensaje->pokemon.especie), unMensaje->pokemon.posicion);
}

void team_suscriptor_cola_APPEARD(cr_list* mensajes){

	while(PROCESO_ACTIVO){
		t_mensaje_appeared_pokemon* mensajeRecibido = leer_mensaje_cuando_este_disponible(mensajes);

		pokemon* unPokemon = desempaquetar_pokemon(mensajeRecibido);

		pthread_mutex_lock(&Mutex_AndoLoggeando);
		log_info(logger, "APPEARED %s [%u %u]", unPokemon->especie, unPokemon->posicion.pos_x, unPokemon->posicion.pos_y); //Ver TODO si pokemon localized hace esto. Ver como saltear esta parte.
		pthread_mutex_unlock(&Mutex_AndoLoggeando);

		registrar_pokemon(unPokemon);

		mensaje_appeared_catch_pokemon_destruir(mensajeRecibido);
	}
}
