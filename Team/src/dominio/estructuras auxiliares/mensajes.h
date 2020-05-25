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

//funciones de desempaquetado
pokemon*desempaquetar_pokemon(void*empaquetado);
void* desempaquetar_resultado(void*empaquetado);
mensaje_localized_pokemon* desempaquetar_localized(void*empaquetado);

//mensajes entre modulos
mensaje recibir_mensaje();
void Get(void* especiePokemon);
void Get_pokemones(especies_pokemones);
t_id Catch(especie_pokemon); //momentaneamente (TODO) retorna el id del mensaje pendiente que crea

//constructores de empaquetados
mensaje_localized_pokemon* localized_pokemon_crear(especie_pokemon especie);

# endif
