#include "../estructuras-principales/posicion.h"

#include "commons/string.h"

//implementacion funciones auxiliares

t_posicion posicion_create(coordenada x, coordenada y){
   return (t_posicion) {x,y};
}

t_posicion*posicion_ptr_create(coordenada x, coordenada y){
	t_posicion*unaPos = malloc(sizeof(t_posicion));
		      *unaPos = posicion_create(x,y);

    return unaPos;
}

numero posicion_distancia(t_posicion unaPosicion, t_posicion otraPosicion){
	numero distanciaHorizontal = abs(unaPosicion.pos_x - otraPosicion.pos_x);
	numero distanciaVertical = abs(unaPosicion.pos_y - otraPosicion.pos_y);

	return distanciaHorizontal+distanciaVertical;
}

bool posicion_cmp(t_posicion unaPos, t_posicion otraPos){
	return (unaPos.pos_x == otraPos.pos_x) && (unaPos.pos_y == otraPos.pos_y);
}

char* posiciones_to_string(t_list* posiciones){

	char*stringDePosiciones = string_new();

	void posicion_concatenar_string(t_posicion*posicion){
		string_append_with_format(&stringDePosiciones, " [%u %u]", posicion->pos_x, posicion->pos_y);
	}

	list_iterate(posiciones, (void*) posicion_concatenar_string);

	return stringDePosiciones;
}
