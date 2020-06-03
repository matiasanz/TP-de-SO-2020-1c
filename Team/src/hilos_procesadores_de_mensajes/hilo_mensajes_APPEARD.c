#include "../hilos_del_team/hilos_team.h"
#include "../team.h"

/*TODO
 * en registrar_pokemon no importa que ya lo tenga en mapa (o si? REVER), pero si tengo que
 * 							CONTROLAR QUE SE RESPETEN LOS OBJETIVOS (cambiar implementacion)
 */

typedef t_mensaje_appeared_catch_pokemon mensaje;

pokemon*desempaquetar_pokemon(t_mensaje_appeared_catch_pokemon*mensaje){
	return pokemon_ptr_create(mensaje->pokemon.especie, mensaje->pokemon.posicion);
}

void team_suscriptor_cola_APPEARD(cr_list* mensajes){

	while(PROCESO_ACTIVO){
		mensaje* mensajeRecibido = cr_list_wait_and_remove(mensajes, 0);

		pokemon* unPokemon = desempaquetar_pokemon(mensajeRecibido);

		log_info(logger, "APPEARD %s [%u %u]", unPokemon->especie, unPokemon->posicion.pos_x, unPokemon->posicion.pos_y); //Ver TODO si pokemon localized hace esto. Ver como saltear esta parte.

		registrar_pokemon(unPokemon);
	}

	log_info(event_logger, "Finalizo suscripcion a cola APPEARD");
}

/*************************************** Funciones Auxiliares ************************************************/

// Agrega los pokemones que van llegando al mapa en caso de ser requeridos
void registrar_pokemon(pokemon*unPokemon){

	if( pokemon_es_requerido(*unPokemon) ){
		mapa_mapear_requerido(unPokemon);
		sem_post(&HayTareasPendientes);
	}

	else{
		printf("Se recibio un %s y pokemon que no es objetivo de nadie o que ya tengo en mapa, es cartera\n", unPokemon->especie);
		pokemon_destroy(unPokemon); //"descartar al pokemon"
	}
}

bool pokemon_es_requerido(pokemon unPokemon){
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
