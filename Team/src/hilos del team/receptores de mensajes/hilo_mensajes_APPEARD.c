#include "../hilos_team.h"
#include "../../team.h"

/*TODO
 * en registrar_pokemon no importa que ya lo tenga en mapa (o si? REVER), pero si tengo que
 * 							CONTROLAR QUE SE RESPETEN LOS OBJETIVOS (cambiar implementacion)
 */

void team_suscriptor_cola_APPEARD(cr_list* mensajes){

	while(!FinDelProceso){
		mensaje* mensajeRecibido = cr_list_wait_and_remove(mensajes, 0);

		pokemon* unPokemon = desempaquetar_pokemon(mensajeRecibido->serializado);

		pthread_mutex_lock(&Mutex_AndoLoggeando);
		log_info(logger, "APPEARD %s [%u] [%u]", unPokemon->especie, unPokemon->posicion.pos_x, unPokemon->posicion.pos_y); //Ver TODO si pokemon localized hace esto. Ver como saltear esta parte.
		pthread_mutex_unlock(&Mutex_AndoLoggeando);

		registrar_pokemon(unPokemon);

		sem_post(&HayTareasPendientes);
	}

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Finalizo suscripcion a cola APPEARD");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

/********************************** Auxiliares ************************************/

void registrar_pokemon(pokemon*unPokemon){

	especies_agregar(historialDePokemones, unPokemon->especie);

	if( es_objetivo_de_alguien(*unPokemon, equipo ) && !mapa_especie_mapeada(pokemonesRequeridos, unPokemon->especie)){
		mapa_mapear_objetivo(pokemonesRequeridos, unPokemon);
		puts(">>>Signal(hay mas pokemones)");
	}

	else{
		printf("Se recibio un %s y pokemon que no es objetivo de nadie o que ya tengo en mapa, es cartera\n", unPokemon->especie);
		pokemon_destroy(unPokemon); //"descartar al pokemon"
	}
}
