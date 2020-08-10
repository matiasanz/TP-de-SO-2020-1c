#include <string.h>
#include <commons/config.h>

#include "../estructuras-principales/pokemon.h"

entrenadores config_get_equipo();
t_list* config_get_posiciones();
numero config_get_quantum();
numero config_get_retardo_ciclo_cpu();
double config_get_alfa();
numero config_get_estimacion_inicial();
numero config_get_tiempo_reconexion();
char* config_get_ip_broker();
char* config_get_puerto_broker();

//auxiliares:
especies_pokemones especies_from_string(char*);
void string_array_destroy(char**stringArray); //Pasar a commons?
