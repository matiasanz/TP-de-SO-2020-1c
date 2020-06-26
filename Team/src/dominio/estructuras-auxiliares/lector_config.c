#include "lector_config.h"

#include <commons/string.h> //no pasar al .h porque no deja correr tests

#include "../estructuras-auxiliares/mensajes.h"

entrenadores entrenadores_cargar(){
	entrenadores miEquipo = entrenadores_create();

	t_list*posiciones = config_get_posiciones();
	char**stringsDeInventarios = config_get_array_value(config, "POKEMON_ENTRENADORES");
	char**stringsDeObjetivos = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");

	int i;
	for(i=0; !list_is_empty(posiciones); i++){

		t_posicion* posicion = list_remove(posiciones, 0);

		entrenador* unEntrenador = entrenador_ptr_crear(i, stringsDeInventarios[i], stringsDeObjetivos[i], *posicion);

		list_add(miEquipo, unEntrenador);

		printf("Se agrego un entrenador en [%u %u] con objetivos:", unEntrenador->posicion.pos_x, unEntrenador->posicion.pos_y);
		recursos_mostrar(unEntrenador->objetivos);

		printf(" e inventario:");
		recursos_mostrar(unEntrenador->pokemonesCazados);
		printf("\n");

		free(posicion);
	}


	list_destroy(posiciones);

	string_array_destroy(stringsDeObjetivos);
	string_array_destroy(stringsDeInventarios);

	return miEquipo;
}

//*************************************************

t_posicion posicion_from_string(char* cadena){
	char** coordenadas = string_split(cadena, "|");
	coordenada x = atoi(coordenadas[0]);
	coordenada y = atoi(coordenadas[1]);

	string_array_destroy(coordenadas);
	return posicion_create(x, y);
}

t_list* config_get_posiciones(){

	char**stringDePosiciones =	config_get_array_value(config, "POSICIONES_ENTRENADORES");
	t_list* posiciones = list_create();

	void _listar(char*cadena){
		t_posicion* posicion = malloc(sizeof(t_posicion));
		*posicion = posicion_from_string(cadena);

		list_add(posiciones, posicion);
	}

	string_iterate_lines(stringDePosiciones, _listar);

	string_array_destroy(stringDePosiciones);

	return posiciones;
}

void string_array_destroy(char**stringArray){
	void string_destroy(char*cadena_en_heap){ //solo para elementos de un arr de string
		free(cadena_en_heap);
	}

	string_iterate_lines(stringArray, string_destroy);//TODO VEr

	free(stringArray);
}
