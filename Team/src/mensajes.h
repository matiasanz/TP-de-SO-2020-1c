# ifndef _MENSAJES_TEAM_
# define _MENSAJES_TEAM_

#include "pokemon.h"
#include <crenito-commons/mensajes/mensaje_new_pokemon.h>

//Prueba
entrenador*entrenador_de_prueba();

//mensajes que recibe de otros modulos
typedef enum{
 	LOCALIZED_POKEMON_,
	CAUGHT_POKEMON_,
	MISSED //Ver, no existe este mensaje
}t_opcode;

typedef uint32_t t_id;

//TAD mensaje
typedef struct Mensaje{
	t_opcode opcode;
	void*serializado;
} mensaje;

//estructuras empaquetadas:

//funciones de desempaquetado
pokemon*desempaquetar_pokemon(void*empaquetado);
t_id* desempaquetar_id(void*empaquetado);

//mensajes entre modulos
mensaje recibir_mensaje();
void get(void* especiePokemon);
t_id catch(especie_pokemon); //retorna el id del mensaje pendiente que crea;
//no se que quise decir con lo siguiente pero VER: ver si no seria la posicion

# endif
