# ifndef _MENSAJES_TEAM_
# define _MENSAJES_TEAM_

#include <crenito-commons/mensajes/mensaje_localized_pokemon.h>
#include <crenito-commons/mensajes/mensaje_caught_pokemon.h>
#include <crenito-commons/mensajes/mensaje_appeared_catch_pokemon.h>

#include "mensajes-utils.h"
#include "../dominio/header_global_team.h"
#include "../dominio/estructuras-principales/pokemon.h"
#include "../dominio/estructuras-auxiliares/captura_pendiente.h"


typedef t_mensaje_appeared_catch_pokemon t_mensaje_appeared_pokemon;
typedef t_mensaje_appeared_catch_pokemon t_mensaje_catch_pokemon;

//mensajes entre modulos
void Get(void* especiePokemon);
void Get_pokemones(matriz_recursos objetivos, matriz_recursos disponibles);
void team_enviar_mensaje_get(especie_pokemon especiePokemon);
void mensaje_get_registrar(t_id idMensaje);
void autoresponder_get(t_mensaje_get_pokemon* mensajeGet, t_id idMensajeFallido);

void Catch(entrenador*, pokemon*); //momentaneamente (TODO) retorna el id del mensaje pendiente que crea
void autoresponder_catch(t_mensaje_catch_pokemon* mensajeCatch, t_id idCapturaPendiente);
void team_enviar_mensaje_catch(captura_pendiente* captura);

//Auxiliares

t_mensaje_catch_pokemon* mensaje_catch_pokemon_crear(pokemon*pokemonCatcheado);
captura_pendiente* captura_crear(entrenador*unEntrenador, pokemon*unPokemon);
void pendiente_set_id(captura_pendiente* captura, t_id idCaptura);
void agregar_captura_pendiente(captura_pendiente* captura, t_id idCaptura);



# endif
