# ifndef _MENSAJES_TEAM_
# define _MENSAJES_TEAM_

#include "pokemon.h"

//Prueba
entrenador*entrenador_de_prueba();

//mensajes que recibe de otros modulos
typedef enum{
 	LOCALIZED_POKEMON,
	CAUGHT_POKEMON,
	MISSED
}t_opcode;

typedef uint32_t t_id;

//TAD mensaje
typedef struct Mensaje{
	t_opcode opcode;
	void*serializado;
} mensaje;

//estructuras empaquetadas:
typedef struct T_mensaje_new_pokemon{
	especie_pokemon especie;
	uint32_t especieLength;
	coordenada posX;
	coordenada posY;
}t_mensaje_new_pokemon;


//funciones de desempaquetado
pokemon*desempaquetar_pokemon(void*serializado);
t_id* desempaquetar_id(void*empaquetado);

//mensajes entre modulos
mensaje recibir_mensaje();
void get(void* especiePokemon);
t_id catch(especie_pokemon); //retorna el id del mensaje pendiente que crea;
//no se que quise decir con lo siguiente pero VER: ver si no seria la posicion

# endif
