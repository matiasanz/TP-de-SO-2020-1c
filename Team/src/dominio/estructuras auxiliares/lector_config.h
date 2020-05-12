#include <string.h>
//#include <commons/string.h>
#include <commons/config.h>

#include "../estructuras principales/pokemon.h"

void entrenadores_cargar();

//auxiliares:
t_list* config_get_posiciones();
especies_pokemones especies_from_string(char*);
void string_array_destroy(char**stringArray); //Pasar a commons?
