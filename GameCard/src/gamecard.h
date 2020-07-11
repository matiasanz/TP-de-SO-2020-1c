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

#include <stdbool.h>
#include <commons/string.h>
#include <commons/bitarray.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "dominio/metadata.h"
#include "utils/logs.h"

#define tope(x,y) (((x)+(y)-1)/(y))

typedef enum{METADATA,FILES,BLOCKS} e_key_path_estructura;

//---variables Globales-----------

t_config* config_metadata;
t_config* config_metadata_directorio_files;

uint32_t TIEMPO_REINTENTO_OPERACION;
uint32_t TIEMPO_RETARDO_OPERACION;
char* PUNTO_MONTAJE_TALLGRASS;

t_bitarray* bitmap;
FILE* f_bitmap;

char* paths_estructuras[3]; //e_key_path_estructura

pthread_mutex_t mutBitarray;
t_dictionary* semaforosDePokemons; //key:char* especiePokemon, value: pthread_mutex_t* mutex
pthread_mutex_t mutDiccionarioSemaforos;

pthread_mutex_t mutexConfigMetadata;

pthread_mutex_t envioPaquete;

pthread_mutex_t mutexLogger;
pthread_mutex_t mutexEventLogger;

//-------------------------

void inicializar();
void inicializar_config();
void leer_config();
void inicializar_logs();
void inicializar_conexiones();
void suscribir_y_escuchar_cola_catch_pokemon();
void suscribir_y_escuchar_cola_get_pokemon();
void suscribir_y_escuchar_cola_new_pokemon();


void crearEstructuras();
void crear_metadata(char**dir_metadata);
void crear_carpeta_files(char** dir_files);
void crear_carpeta_blocks(char** dir_blocks);
void crear_directorios(char** dir_metadata, char** dir_files, char** dir_blocks);
void leer_metadata(char**dir_metadata, char**bin_metadata, FILE**f_metadata);
void crear_bitmap(char**dir_metadata);
void crear_bloques(char** bin_block, char*dir_blocks);


void gamecard_New_Pokemon(t_mensaje_new_pokemon* unMsjNewPoke);
void gamecard_Catch_Pokemon(t_mensaje_appeared_catch_pokemon* unMsjCatchPoke);
void gamecard_procesar_Get_Pokemon(t_mensaje_get_pokemon* unMsjGetPoke);

void gamecard_New_Pokemon_Reintento(t_mensaje_new_pokemon* unMsjNewPoke);
void gamecard_Get_Pokemon_reintento(t_mensaje_get_pokemon* unMsjGetPoke);

int cant_elemetos_array(char** array);
void string_array_liberar(char** split);
int bloque_disponible(t_bitarray* bitmap,int totalBloques);
void guardarLinea(char* path,char* nuevalinea,int len);
char* crearLinea(t_mensaje_new_pokemon* unMsjNewPoke );
int size_bloque(char* path);
bool contiene_string_en_bloques(char* string, char**bloques);
int espacioDisponibleEnBloque(char* path);
int bloquesNecesarios(char* lineaNueva,int maxSizeBloque);
bool contienePosicionEnBloques(char* string, char**bloques);
//DEPRECATED
char* contenidoDeBloques(char** bloques);
void sobrescribirLineas(char* path,char* nuevalinea,int len);
char* contenido_de_Bloques_con_mmap(char** bloques);

#endif /* SRC_GAMECARD_H_ */
