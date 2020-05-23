#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include <stdio.h>
#include <stdlib.h>
#include <crenito-commons/utils.h>
#include <crenito-commons/mensajes/mensaje_new_pokemon.h>
#include <crenito-commons/conexiones/socket.h>
#include <commons/log.h>
#include <commons/config.h>

t_config* config;
t_log* logger;
t_log* event_logger;

void inicializar(void);


#endif
