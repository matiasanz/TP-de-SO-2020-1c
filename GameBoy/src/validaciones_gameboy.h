#ifndef _VALIDACIONES_GAMEBOY_
#define _VALIDACIONES_GAMEBOY_

#include "crenito-commons/conexiones/conexiones.h"

void validar_proceso(char* proceso);
void validar_mensaje(char* mensaje);
//void obtener_conexion(char* proceso, t_conexion_server* conexion);
void validar_cantidad_argumentos(int cantidad,int numero);
void validar_quien_conoce_newpokemon(char* proceso);
void validar_quien_conoce_appearedpokemon(char* proceso);
void validar_quien_conoce_catchpokemon(char* proceso);
void validar_quien_conoce_caughtpokemon(char* proceso);
void validar_quien_conoce_getpokemon(char* proceso);
void validar_ok_fail(char* argumento);
void validar_mayor_a_cero(int *cantidad,char* num);
void validar_mayor_igual_a_cero(int *pos,char* num);




#endif
