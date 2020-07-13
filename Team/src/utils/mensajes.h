# ifndef _MENSAJES_TEAM_
# define _MENSAJES_TEAM_

#include <crenito-commons/mensajes/mensaje_localized_pokemon.h>
#include <crenito-commons/mensajes/mensaje_caught_pokemon.h>
#include <crenito-commons/mensajes/mensaje_appeared_catch_pokemon.h>

#include "../dominio/header_global_team.h"
#include "../dominio/estructuras-principales/pokemon.h"
#include "mensajes-utils.h"

typedef t_mensaje_appeared_catch_pokemon t_mensaje_appeared_pokemon;
typedef t_mensaje_appeared_catch_pokemon t_mensaje_catch_pokemon;

//mensajes entre modulos
void* leer_mensaje_cuando_este_disponible(cr_list*);
void Get(void* especiePokemon);
void Get_pokemones(matriz_recursos objetivos, matriz_recursos disponibles);
void Catch(entrenador*, pokemon*); //momentaneamente (TODO) retorna el id del mensaje pendiente que crea

# endif
