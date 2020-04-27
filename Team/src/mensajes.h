# ifndef _MENSAJES_TEAM_
# define _MENSAJES_TEAM_

#include "pokemon.h"

//mensajes que recibe de otros modulos
typedef enum{
	NUEVO_ENTRENADOR = 0,
 	LOCALIZED_POKEMON = 1,
	CAUGHT_POKEMON = 2
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
}t_mensaje_new_pokemon;

typedef struct T_mensaje_new_entrenador{ //Obs: esta acoplado al anterior, ver si es conveniente
	t_mensaje_new_pokemon*objetivos;
	coordenadas posicion;
}t_mensaje_new_entrenador;


//mensajes entre modulos
mensaje recibir_mensaje();
entrenador*desempaquetar_entrenador(void*serializado);
pokemon*desempaquetar_pokemon(void*serializado);
void get(void* especiePokemon);
t_id catch(especie_pokemon); //retorna el id del mensaje pendiente que crea;
//no se que quise decir con lo siguiente pero VER: ver si no seria la posicion

# endif
