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

bool pokemon_es_objetivo_de(pokemon unPokemon, entrenador unEntrenador){
	return list_any_satisfy(unEntrenador.objetivos, (bool(*)(void*))&pokemon_misma_especie);
}

bool es_objetivo_de_alguien(pokemon unPokemon, entrenadores unEquipo){
	t_list* objetivos = entrenadores_objetivos_globales(unEquipo);

	bool esElMismo(void* unaEspecie){
		return especie_cmp((especie_pokemon) unaEspecie, unPokemon.especie);
	}

	bool esObjetivo = list_any_satisfy(objetivos, (bool(*)(void*))&esElMismo);

	list_destroy(objetivos);

	return esObjetivo;
}

void entrenador_capturar(entrenador*entrenador, pokemon*victima){
	/*Ver si conviene en caso de que sea un objetivo directamente eliminarlo de la lista.
	*Seria mas facil pero no se si respeta la consigna y aparte si no complica los test...*/
	list_add(entrenador->pokemonesCazados, victima->especie);
	printf("Se ha capturado a %s\n", victima->especie);
	pokemon_destroy(victima);//ROMPE
}

//Destructor
void pokemon_destroy(pokemon*destruido){
	free(destruido);
}

//**********************************************************************
//TAD MAPA

//Constructor
mapa_pokemones mapa_create(){
	return list_create();
}

void mapa_mapear_objetivo(mapa_pokemones unMapa, pokemon* objetivo){

	bool distintaEspecieQue(pokemon* deLista){
		return !pokemon_misma_especie(*objetivo, *deLista);
	}

	if(list_all_satisfy(unMapa, (bool(*)(void*))&distintaEspecieQue)){
		list_add(unMapa, objetivo);
		printf(" Se mapeo %s en la posicion (%d, %d)\n", objetivo->especie, objetivo->posicion.x, objetivo->posicion.y);
	}
}

pokemon*mapa_first(mapa_pokemones unMapa){
	return list_get(unMapa, 0);
}

pokemon* mapa_desmapear(mapa_pokemones unMapa){
	return (pokemon*) list_remove(unMapa, 0);
}

//Destructor
void mapa_destroy(mapa_pokemones destruido){
	list_destroy_and_destroy_elements(destruido, (void(*)(void*)) &pokemon_destroy);
}
