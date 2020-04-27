#include "auxiliares.h"

//implementacion funciones auxiliares

void posicion_create(coordenadas* nuevaPosicion, coordenada x, coordenada y){
   (*nuevaPosicion)= malloc(sizeof(int)*DIMENSION_MAPA);
   (*nuevaPosicion)[0] = x;
   (*nuevaPosicion)[1] = y;
}
