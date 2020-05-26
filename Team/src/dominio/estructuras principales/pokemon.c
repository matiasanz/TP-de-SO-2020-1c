#include "pokemon.h"

//TAD especie (definido en entrenador)
//cmp
bool especie_cmp(especie_pokemon unaEspecie, especie_pokemon otraEspecie){
	return !strcmp(unaEspecie, otraEspecie);
}

void especies_agregar(especies_pokemones especies, especie_pokemon especie){
	list_add(especies, especie);
}

void especies_agregar_validado(especies_pokemones especies, especie_pokemon unaEspecie){
	if(!unaEspecie){
		error_show("pokemon nulo");
		exit(1);
	}

	especies_agregar(especies, unaEspecie);
}

bool especie_recibida_con_anterioridad(especie_pokemon especie, especies_pokemones historial){

	bool yaLaTengo(void*especieDeLista){
		return especie_cmp((especie_pokemon)especieDeLista, especie);
	}

	return list_any_satisfy(historial, &yaLaTengo);
}

//TAD pokemon
pokemon pokemon_create(especie_pokemon especie, coordenada coordenadaX, coordenada coordenadaY){
	return (pokemon) {especie, posicion_create(coordenadaX, coordenadaY)};
}

pokemon* pokemon_ptr_create(especie_pokemon especie, coordenada coordenadaX, coordenada coordenadaY){
	pokemon*unPokemon = malloc(sizeof(pokemon));
		   *unPokemon = pokemon_create(especie, coordenadaX, coordenadaY);
    return unPokemon;
}

bool pokemon_misma_especie(pokemon unPokemon, pokemon otroPokemon){
	return especie_cmp(unPokemon.especie, otroPokemon.especie); //string_equals_ignore_case(objetivo->especie, deLista->especie);
}

bool pokemon_es_objetivo(pokemon unPokemon, matriz_recursos objetivos){
	return recursos_cantidad_de_instancias_de(objetivos, unPokemon.especie);
}

void entrenador_capturar(entrenador*entrenador, pokemon*victima){
	recursos_agregar_recurso(entrenador->pokemonesCazados, victima->especie);

	t_posicion posicionDelEvento = entrenador->posicion;

	pthread_mutex_lock(&Mutex_AndoLoggeando);
	log_info(logger, "El Entrenador N°%u ha capturado un %s en la posicion [%u %u]", entrenador->id, victima->especie, posicionDelEvento.pos_x, posicionDelEvento.pos_y);
	pthread_mutex_unlock(&Mutex_AndoLoggeando);

	pokemon_destroy(victima);
}

//Destructor
void pokemon_destroy(pokemon*destruido){
	free(destruido);
}

//**********************************************************************
//TAD MAPA

//Constructor
mapa_pokemones mapa_create(){
	return cr_list_create();
}

void mapa_mapear_objetivo(mapa_pokemones unMapa, pokemon* objetivo){
	cr_list_add_and_signal(unMapa, objetivo);
}

pokemon*mapa_first(mapa_pokemones unMapa){
	return cr_list_get(unMapa, 0);
}

bool mapa_especie_mapeada(mapa_pokemones unMapa, especie_pokemon unaEspecie){
	bool mismaEspecieQue(pokemon* deLista){
		return especie_cmp(deLista->especie, unaEspecie);
	}

	pthread_mutex_lock(unMapa->mutex); //TODO
	bool resultado = list_any_satisfy(unMapa->lista, (bool(*)(void*))&mismaEspecieQue);
	pthread_mutex_unlock(unMapa->mutex);

	return resultado;
}

pokemon* mapa_desmapear(mapa_pokemones unMapa){
	return (pokemon*) cr_list_wait_and_remove(unMapa, 0);
}

//Destructor
void mapa_destroy(mapa_pokemones destruido){
	cr_list_destroy_and_destroy_elements(destruido, (void(*)(void*)) &pokemon_destroy);
}
