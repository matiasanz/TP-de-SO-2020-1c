#ifndef _VALIDACIONES_GAMEBOY_
#define _VALIDACIONES_GAMEBOY_

#include <ctype.h>
#include <crenito-commons/utils.h>

void validar_proceso(char* proceso);
void validar_mensaje(char* mensaje);
void validar_cantidad_argumentos(int cantidad,int numero);
void validar_quien_conoce_newpokemon(char* proceso);
void validar_quien_conoce_appearedpokemon(char* proceso);
void validar_quien_conoce_catchpokemon(char* proceso);
void validar_quien_conoce_caughtpokemon(char* proceso);
void validar_quien_conoce_getpokemon(char* proceso);
void validar_ok_fail(char* argumento);
void validar_mayor_a_cero(char* num);
void validar_mayor_igual_a_cero(char* num);
void validar_que_es_numero(char* num);

#endif
