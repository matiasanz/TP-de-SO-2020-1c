# ifndef _MENSAJES_TEAM_
# define _MENSAJES_TEAM_

#include "../header_global_team.h"
#include "../estructuras-principales/pokemon.h"
#include "../utils/mensajes-utils.h"
#include <crenito-commons/mensajes/mensaje_localized_pokemon.h>
#include <crenito-commons/mensajes/mensaje_caught_pokemon.h>
#include <crenito-commons/mensajes/mensaje_appeared_catch_pokemon.h>

//Prueba
entrenador*entrenador_de_prueba();

typedef struct{
	t_id idCaptura;
	bool tuvoExito;
}resultado_captura;

//funciones de desempaquetado
pokemon*desempaquetar_pokemon(t_mensaje_appeared_catch_pokemon*empaquetado);
//Realizan copia del contenido que me sirve del mensaje antes de destruirse
//resultado desempaquetar_resultado(t_mensaje_caught_pokemon*empaquetado);

//mensajes entre modulos
void* leer_mensaje_cuando_este_disponible(cr_list*);
void Get(void* especiePokemon);
void Get_pokemones(matriz_recursos);
void Catch(entrenador*, pokemon*); //momentaneamente (TODO) retorna el id del mensaje pendiente que crea

# endif
