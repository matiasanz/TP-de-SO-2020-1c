#ifndef _MENSAJE_
#define _MENSAJE_

#include "validaciones_gameboy.h"
#include <crenito-commons/utils.h>
#include "gameboy_logs.h"

t_paquete* crear_paquete(char* proceso, char* mensaje, char* argumentos[], int longitud);
void procesar_envio_mensaje(char* proceso, char* mensaje, char* argumentos[], int longitud);
t_conexion_server* obtener_conexion(char* proceso_string);

#endif
