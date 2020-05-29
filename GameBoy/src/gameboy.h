#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include <stdio.h>
#include <stdlib.h>
#include <crenito-commons/utils.h>
#include <crenito-commons/mensajes/mensaje_new_pokemon.h>
#include <crenito-commons/mensajes/mensaje_appeared_catch_pokemon.h>
#include <crenito-commons/mensajes/mensaje_caught_pokemon.h>
#include <crenito-commons/mensajes/mensaje_get_pokemon.h>
#include <crenito-commons/mensajes/mensaje_localized_pokemon.h>
#include <crenito-commons/conexiones/socket.h>
#include <commons/log.h>
#include <commons/config.h>

t_config* config;
t_log* logger;
t_log* event_logger;

void inicializar(void);


#endif
