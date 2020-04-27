#include "mapa.h"
#include <commons/collections/list.h>
#include <commons/string.h>
//Constructor
mapa mapa_create(){
	return list_create();
}

void mapear_objetivo(mapa unMapa, pokemon* objetivo){

	bool mismaEspecieQue(void* pokemon_de_lista){
		pokemon* deLista = pokemon_de_lista;
		return string_equals_ignore_case(objetivo->especie, deLista->especie);
	}

	if(list_all_satisfy(unMapa, &mismaEspecieQue)){
		list_add(unMapa, objetivo);
	}
}
