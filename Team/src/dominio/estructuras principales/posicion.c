#include "posicion.h"
//implementacion funciones auxiliares

t_posicion posicion_create(coordenada x, coordenada y){
   return (t_posicion) {x,y};
}

numero posicion_distancia(t_posicion unaPosicion, t_posicion otraPosicion){
	numero distanciaHorizontal = abs(unaPosicion.x - otraPosicion.x);
	numero distanciaVertical = abs(unaPosicion.y - otraPosicion.y);

	return distanciaHorizontal+distanciaVertical;
}

bool posicion_cmp(t_posicion unaPos, t_posicion otraPos){
	return (unaPos.x == otraPos.x) && (unaPos.y == otraPos.y);
}
