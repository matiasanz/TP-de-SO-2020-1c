# ifndef _AUXILIARES_
# define _AUXILIARES_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <commons/string.h>
#include <commons/error.h>
#include <commons/collections/list.h>
#include <crenito-commons/prueba.h>

//constantes globales; Ver si vale la pena en algun lado usar "union" en C
#define DIMENSION_MAPA 	  2 //cantidad de coordenadas que tendra cada posicion
typedef int coordenada;
typedef coordenada* coordenadas;

//firma funciones auxiliares
void posicion_create(int**nuevaPosicion, coordenada x, coordenada y);


# endif
