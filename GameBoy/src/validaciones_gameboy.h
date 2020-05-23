#ifndef _VALIDACIONES_GAMEBOY_
#define _VALIDACIONES_GAMEBOY_

void validar_proceso(char* proceso);
void validar_mensaje(char* mensaje);
void obtener_ip_puerto(char* proceso,char **ip,char  **puerto);
void validar_cantidad_argumentos(int cantidad,int numero);
void validar_quien_conoce_newpokemon(char* proceso);
void validar_mayor_a_cero(int *cantidad,char* num);
void validar_mayor_igual_a_cero(int *pos,char* num);




#endif
