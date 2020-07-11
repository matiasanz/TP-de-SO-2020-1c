# ifndef _MENSAJES_GAMECARD_
# define _MENSAJES_GAMECARD_ //nombre provisorio

#include "gamecard.h"

//GET
char* pokemon_find_metadata(char*especie);
t_list*localizar_pokemon(t_mensaje_get_pokemon*, char* PATH_METADATA);
void gamecard_enviar_localized(t_mensaje_get_pokemon* mensajeGet, t_list*posiciones);
void simular_acceso_a_disco();
bool archivo_existe(t_config* metadata);
bool archivo_abierto(t_config* config_metadata_pokemon);
t_list* leer_posiciones_de_disco(t_config* config_metadata_pokemon);
t_list* localizar_pokemon(t_mensaje_get_pokemon* mensajeGet, char*bin_metadata);
//***********************************************************************************
pthread_mutex_t* pokemon_get_mutex(char* especie);

//NEW
void actualizar_datos(char* cadenaABuscar, char**bloquesDelPokemon, t_config*config_metadata_pokemon, t_mensaje_new_pokemon* mensajeNew);
char* get_contenido_actualizado(char**lineasDelPokemon, char*stringPosAdaptado, uint32_t cantidad);
t_config* metadata_default(char*especie, char*dir_metadata);
void log_enunciado_intento_fallido_de_new_pokemon(t_mensaje_new_pokemon* mensajeNew);
char*pokemon_find_carpeta(char*especie);
void gamecard_New_Pokemon_Reintento(t_mensaje_new_pokemon* unMsjNewPoke);
void agregar_nueva_linea(t_config* config_metadata_pokemon, char**bloquesDelPokemon, t_mensaje_new_pokemon* mensajeNew);

# endif
