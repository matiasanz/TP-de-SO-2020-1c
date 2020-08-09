#ifndef _MENSAJE_
#define _MENSAJE_

#include "../utils/gameboy_logs.h"
#include "../utils/gameboy_utils.h"
#include "../utils/validaciones_gameboy.h"

t_paquete* crear_paquete(char* proceso, char* mensaje, char* argumentos[], int longitud);
void procesar_envio_mensaje(char* proceso, char* mensaje, char* argumentos[], int longitud);
t_conexion_server* obtener_conexion(char* proceso_string);

#endif
