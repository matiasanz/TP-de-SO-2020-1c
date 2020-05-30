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
		log_info(logger, "APPEARD %s [%u %u]", unPokemon->especie, unPokemon->posicion.pos_x, unPokemon->posicion.pos_y); //Ver TODO si pokemon localized hace esto. Ver como saltear esta parte.
		pthread_mutex_unlock(&Mutex_AndoLoggeando);

		registrar_pokemon(unPokemon);
		sem_post(&HayTareasPendientes);
	}

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Finalizo suscripcion a cola APPEARD");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

/********************************** Auxiliares ************************************/
// Agrega los pokemones que van llegando al mapa en caso de ser requeridos
void registrar_pokemon(pokemon*unPokemon){

	if( pokemon_es_requerido(*unPokemon, objetivosGlobales) ){
		mapa_mapear_requerido(unPokemon);
	}

	else{
		printf("Se recibio un %s y pokemon que no es objetivo de nadie o que ya tengo en mapa, es cartera\n", unPokemon->especie);
		pokemon_destroy(unPokemon); //"descartar al pokemon"
	}
}

bool pokemon_es_requerido(pokemon unPokemon, matriz_recursos objetivos){
	return objetivos_cantidad_requerida_de(unPokemon.especie);
}

numero objetivos_cantidad_requerida_de(especie_pokemon unaEspecie){
	numero instanciasTotales = recursos_cantidad_de_instancias_de(objetivosGlobales, unaEspecie);

	pthread_mutex_lock(&mutexInventariosGlobales);
	numero instanciasDisponibles = recursos_cantidad_de_instancias_de(inventariosGlobales, unaEspecie);
	pthread_mutex_unlock(&mutexInventariosGlobales);

	pthread_mutex_lock(&mutexRecursosEnMapa);
	numero instanciasEnMapa = recursos_cantidad_de_instancias_de(recursosEnMapa, unaEspecie);
	pthread_mutex_unlock(&mutexRecursosEnMapa);

	return instanciasTotales - (instanciasDisponibles + instanciasEnMapa);
}

void mapa_mapear_requerido(pokemon*unPokemon){
	mapa_mapear_pokemon(pokemonesRequeridos, unPokemon);

	pthread_mutex_lock(&mutexRecursosEnMapa);
	recursos_agregar_recurso(recursosEnMapa, unPokemon->especie);
	pthread_mutex_unlock(&mutexRecursosEnMapa);
}

// Retorna true si la especie ya se encuentra en el registro de especies
bool especie_recibida_con_anterioridad(especie_pokemon especie, especies_pokemones historial){

	bool yaLaTengo(void*especieDeLista){
		return especie_cmp((especie_pokemon)especieDeLista, especie);
	}

	pthread_mutex_lock(&mutexHistorialEspecies);
	bool siONo = list_any_satisfy(historial, &yaLaTengo);
	if(!siONo) list_add(historial, especie);
	pthread_mutex_unlock(&mutexHistorialEspecies);

	return siONo;
}

//matriz_recursos recursos_objetivos_actuales(){
//	pthread_mutex_lock(&mutexInventariosGlobales);
//	matriz_recursos objetivosActuales = recursos_matriz_diferencia(objetivosGlobales, inventariosGlobales);
//	pthread_mutex_unlock(&mutexInventariosGlobales);
//
//	return objetivosActuales;
//}
