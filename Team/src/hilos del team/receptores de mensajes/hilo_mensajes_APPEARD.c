#include "../hilos_team.h"
#include "../../team.h"

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
}
