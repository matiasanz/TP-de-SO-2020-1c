# ifndef _MENSAJES_TEAM_
# define _MENSAJES_TEAM_

#include "../header_global_team.h"
#include "../estructuras principales/pokemon.h" //hardcodeado
//#include <crenito-commons/mensajes/mensaje_new_pokemon.h>

//Prueba
entrenador*entrenador_de_prueba();

//mensajes que recibe de otros modulos
typedef enum{
	LOCALIZED_POKEMON_, APPEARD_POKEMON_, CAUGHT_POKEMON_
}t_opcode;

//TAD mensaje
typedef struct Mensaje{
	t_opcode opcode;
	void*serializado;
} mensaje;

//estructuras empaquetadas:
typedef struct{
	t_id idCaptura;
	bool tuvoExito;
}resultado_captura;

//funciones de desempaquetado
pokemon*desempaquetar_pokemon(void*empaquetado);
void* desempaquetar_resultado(void*empaquetado);

//mensajes entre modulos
mensaje recibir_mensaje();
void Get(void* especiePokemon);
void Get_pokemones(especies_pokemones);
t_id Catch(especie_pokemon); //momentaneamente (TODO) retorna el id del mensaje pendiente que crea
							 //sin embargo, retorna tambien un booleano por medio del cual me avisa si se concreto la captura o no

# endif
