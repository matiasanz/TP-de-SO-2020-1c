# ifndef _MENSAJES_TEAM_
# define _MENSAJES_TEAM_

#include "../header_global_team.h"
#include "../estructuras_principales/pokemon.h" //hardcodeado
//#include <crenito-commons/mensajes/mensaje_new_pokemon.h>

//Prueba
entrenador*entrenador_de_prueba();

//estructuras empaquetadas:
typedef struct{
	numero id;
	numero idCorrelativo;
}t_mensaje_id;

typedef struct{
	t_id idCaptura;
	bool tuvoExito;
}resultado_captura;

typedef struct {
	t_mensaje_id ids;
	especie_pokemon especie;
	numero especie_lenght;
	numero posiciones_lenght;
	t_list* posiciones;
} mensaje_localized_pokemon;

//mensajes entre modulos
void Get(void* especiePokemon);
void Get_pokemones(matriz_recursos);
t_id Catch(especie_pokemon); //momentaneamente (TODO) retorna el id del mensaje pendiente que crea

# endif
