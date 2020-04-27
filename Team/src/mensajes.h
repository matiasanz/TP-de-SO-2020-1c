# ifndef _MENSAJES_TEAM_
# define _MENSAJES_TEAM_

#include "entrenador.h"

//TAD mensaje
typedef struct Mensaje{
	int opcode;
} mensaje;

//mensajes que recibe de otros modulos
#define NUEVO_ENTRENADOR  0
#define LOCALIZED_POKEMON 1
#define CAUGHT 			  2


//mensajes entre modulos
mensaje recibir_mensaje();
void*desempaquetar_mensaje(mensaje);
void get(void* especiePokemon);
int catch(especie_pokemon); //retorna el id del mensaje pendiente que crea;
//no se que quise decir con lo siguiente pero VER: ver si no seria la posicion

# endif
