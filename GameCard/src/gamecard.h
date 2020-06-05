/*
 * gamecard.h
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_GAMECARD_H_
#define SRC_GAMECARD_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "utils/mensajes-utils.h"


#include <commons/string.h>
#include <sys/stat.h>



typedef enum{METADATA,FILES,BLOCKS,POKEMON} e_key_path_estructura;

//---variables Globales-----------

t_config* config_metadata;
t_config* config_metadata_directorio_pokemon;

int tiempo_de_reintento_operacion;
char* punto_montaje_tallgrass;

char* paths_estructuras[4]; //e_key_path_estructura

//-------------------------

void inicializar();
void inicializar_config();
void inicializar_logs();
void inicializar_conexiones();
void subscribir_y_escuchar_cola_catch_pokemon();
void subscribir_y_escuchar_cola_get_pokemon();
void subscribir_y_escuchar_cola_new_pokemon();


void crearEstructuras();



#endif /* SRC_GAMECARD_H_ */
