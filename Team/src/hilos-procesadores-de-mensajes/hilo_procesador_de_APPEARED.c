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

	pthread_mutex_lock(&Mutex_AndoLoggeandoEventos);
	log_info(event_logger, "Finalizo suscripcion a cola APPEARD");
	pthread_mutex_unlock(&Mutex_AndoLoggeandoEventos);
}

/*************************************** Funciones Auxiliares ************************************************/

// Agrega los pokemones que van llegando al mapa en caso de ser requeridos
void registrar_pokemon(pokemon*unPokemon){

	if( pokemon_es_requerido(*unPokemon) ){
		mapa_mapear_requerido(unPokemon);
		puts("\nsignal(cantTareas)");
		sem_post(&HayTareasPendientes);
	}

	else{
		printf("Se recibio un %s y se descarto al no ser requerido\n", unPokemon->especie);
		pokemon_destroy(unPokemon); //"descartar al pokemon"
	}
}

bool pokemon_es_requerido(pokemon unPokemon){
	return objetivos_cantidad_requerida_de(unPokemon.especie);
}

numero objetivos_cantidad_bruta_requerida_de(especie_pokemon unaEspecie){
	numero instanciasTotales = recursos_cantidad_de_instancias_de(objetivosGlobales, unaEspecie);

	pthread_mutex_lock(&mutexInventariosGlobales);
	numero instanciasDisponibles = recursos_cantidad_de_instancias_de(inventariosGlobales, unaEspecie);
	pthread_mutex_unlock(&mutexInventariosGlobales);

	return instanciasTotales - instanciasDisponibles;
}

numero objetivos_cantidad_requerida_de(especie_pokemon unaEspecie){

	numero instanciasBrutas = objetivos_cantidad_bruta_requerida_de(unaEspecie);

	pthread_mutex_lock(&mutexRecursosEnMapa);
	numero instanciasEnMapa = recursos_cantidad_de_instancias_de(recursosEnMapa, unaEspecie);
	pthread_mutex_unlock(&mutexRecursosEnMapa);

	return instanciasBrutas - instanciasEnMapa;
}

void mapa_mapear_requerido(pokemon*unPokemon){

	printf("-----------------------------------------> se Mapeo un %s\n", unPokemon->especie);

	mapa_mapear_pokemon(pokemonesRequeridos, unPokemon);

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
