#include "factory.h"
#include <commons/string.h>

entrenador entrenador_CREATE(t_list* especiesDePokemones, coordenada X, coordenada Y){
	return entrenador_create(list_create(), especiesDePokemones, posicion_create(X,Y));
}

//unico objetivo "pucho", pos (1,2)
entrenador*factory_entrenador_de_prueba() {
	//datos del entrenador hardcodeado TODO
	t_list*objetivos = list_create();
	especie_pokemon especie = "pucho";
	list_add(objetivos, especie);
	coordenada posX = 1;
	coordenada posY = 2;

	entrenador*nuevoEntrenador = (entrenador*)malloc(sizeof(entrenador));
	*nuevoEntrenador = entrenador_CREATE(objetivos, posX, posY);

	return nuevoEntrenador;
}

char*itoa(int i){ //Ignorar valgrind
	return string_itoa(i);
}

entrenador*factory_entrenador_numerado(int i){
	especies_pokemones objetivos = list_create();
	especie_pokemon especie = itoa(i); //ignorar valgrind

	list_add(objetivos, especie);

	entrenador*unEntrenador = malloc(sizeof(entrenador));
	*unEntrenador = entrenador_CREATE(objetivos, i, i+1);
	return unEntrenador;
}

pokemon*factory_pokemon_de_prueba(){
	pokemon* pkm = (pokemon*)malloc(sizeof(pokemon));
	*pkm= pokemon_create("pucho", 3, 6);
	return pkm;
}

pokemon*factory_pokemon_de_prueba_en(coordenada x, coordenada y){
	pokemon* pkm = (pokemon*)malloc(sizeof(pokemon));
	*pkm= pokemon_create("Crenito Jr", x, y);
	return pkm;
}

//constructor de prueba con un solo entrenador
entrenadores entrenadores_CREATE(){
	entrenadores unEquipo = list_create();

	//Ejemplo de llegada de entrenador:
	entrenador* unEntrenador = factory_entrenador_de_prueba();
	//aca se hace el get. Alternativa: Una vez que estan todos, iterar la lista de objetivos globales.
	list_add(unEquipo, unEntrenador);				//agrego el entrenador al equipo

	puts("Se agrego un nuevo entrenador");

	return unEquipo;
}


entrenadores factory_entrenadores_de_prueba(int cantidad){
	entrenadores equipo = entrenadores_create();

	int i;
	for(i=0; i<cantidad; i++){
		entrenador* unEntrenador = factory_entrenador_numerado(i);
		list_add(equipo, unEntrenador);		//agrego el entrenador al equipo
	}

	return equipo;
}
