# ifndef _POSICION_
# define _POSICION_

#include "../header_global_team.h"

typedef numero coordenada;
typedef t_list* posiciones;

typedef struct T_posicion{
	coordenada pos_x;
	coordenada pos_y;
} t_posicion;

//crea en nueva posicion un array de coordenadas {x,y}
t_posicion posicion_create(coordenada x, coordenada y);

//Retorna la distancia entre ambas posiciones
numero posicion_distancia(t_posicion, t_posicion);

//Retorna true si ambas posiciones son iguales
bool posicion_cmp(t_posicion, t_posicion);

//Retorna una cadena de caracteres con las posiciones de forma " [x y] [x y] ... [x y]"
char* posiciones_to_string(t_list* posiciones);

# endif
