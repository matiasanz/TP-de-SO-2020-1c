#include "posicion.h"
//implementacion funciones auxiliares

t_posicion posicion_create(coordenada x, coordenada y){
   return (t_posicion) {x,y};
}

numero posicion_distancia(t_posicion unaPosicion, t_posicion otraPosicion){
	numero distanciaHorizontal = abs(unaPosicion.pos_x - otraPosicion.pos_x);
	numero distanciaVertical = abs(unaPosicion.pos_y - otraPosicion.pos_y);

	return distanciaHorizontal+distanciaVertical;
}

bool posicion_cmp(t_posicion unaPos, t_posicion otraPos){
	return (unaPos.pos_x == otraPos.pos_x) && (unaPos.pos_y == otraPos.pos_y);
}
