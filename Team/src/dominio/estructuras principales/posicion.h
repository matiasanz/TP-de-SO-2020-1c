# ifndef _POSICION_
# define _POSICION_

#include "../header_global_team.h"

typedef numero coordenada;

typedef struct T_posicion{
	coordenada x;
	coordenada y;
} t_posicion;

//crea en nueva posicion un array de coordenadas {x,y}
t_posicion posicion_create(coordenada x, coordenada y);

numero posicion_distancia(t_posicion, t_posicion);

bool posicion_cmp(t_posicion, t_posicion);

# endif
