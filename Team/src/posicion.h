# ifndef _AUXILIARES_
# define _AUXILIARES_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <commons/string.h>
#include <commons/error.h>
#include <commons/collections/list.h>
//#include <crenito-commons/prueba.h>

//constantes globales; Ver si vale la pena en algun lado usar "union" en C
#define DIMENSION_MAPA 	  2 //cantidad de coordenadas que tendra cada posicion
typedef uint32_t coordenada;
typedef coordenada* coordenadas;

//crea en nueva posicion un array de coordenadas {x,y}
void posicion_create(coordenadas*nuevaPosicion, coordenada x, coordenada y);

void posicion_destroy(coordenadas posicion);

# endif
