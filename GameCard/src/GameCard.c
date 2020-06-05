/*
 ============================================================================
 Name        : GameCard.c
 Author      : 
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "gamecard.h"

int main(void) {

	inicializar();

	sem_wait(&objetivos_gamecard);
	return EXIT_SUCCESS;
}

void inicializar(void) {

	sem_init(&objetivos_gamecard, 0, 0);

	inicializar_config();
	inicializar_logs();
	crearEstructuras();
	inicializar_conexiones();
}

void inicializar_config() {

	config = config_create("./config/gamecard.config");
}

void inicializar_conexiones() {

	conexion_broker = conexion_server_crear(
			config_get_string_value(config, "IP_BROKER"),
			config_get_string_value(config, "PUERTO_BROKER"), GAMECARD);

	pthread_mutex_init(&mutex_subscripcion, NULL);

	subscribir_y_escuchar_cola_catch_pokemon((void*) mensaje_recibido);
	subscribir_y_escuchar_cola_get_pokemon((void*) mensaje_recibido);
	subscribir_y_escuchar_cola_new_pokemon((void*) mensaje_recibido);

	conectar_gameboy((void*) mensaje_recibido);

}

void inicializar_logs() {

	logger = log_create("./log/gamecard.log", GAMECARD_STRING, 1,LOG_LEVEL_INFO);
	event_logger = log_create("./log/gamecard_event.log", "GAME_CARD_EVENT", 1, LOG_LEVEL_INFO);
}


void crearEstructuras(){

	tiempo_de_reintento_operacion=config_get_int_value(config,"TIEMPO_DE_REINTENTO_OPERACION");
	punto_montaje_tallgrass=config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS");

	char* dir_metadata = string_new();
	char* dir_files = string_new();
	char* dir_blocks = string_new();
	char* bin_metadata = string_new();
	char* bin_bitmap = string_new();

	char* dir_pokemon = string_new();
	char* bin_metadataPokemon = string_new();

	FILE* f_metadata;

	mkdir(punto_montaje_tallgrass,0777);

	string_append(&dir_metadata,punto_montaje_tallgrass);
	string_append(&dir_metadata,"Metadata/");
	mkdir(dir_metadata,0777);
	log_info(event_logger,"Creada carpeta Metadata");

	string_append(&dir_files,punto_montaje_tallgrass);
	string_append(&dir_files,"Files/");
	mkdir(dir_files,0777);
	log_info(event_logger,"Creada carpeta Files");

	string_append(&dir_blocks,punto_montaje_tallgrass);
	string_append(&dir_blocks,"Blocks/");
	mkdir(dir_blocks,0777);
	log_info(event_logger,"Creada carpeta Blocks");

	string_append(&bin_metadata,dir_metadata);
	string_append(&bin_metadata,"/Metadata.bin");

	if((f_metadata=fopen(bin_metadata,"r"))==NULL){ //si no existe el archivo metadata
		log_info(event_logger,"[ERROR FATAL] FILESYSTEM NO ENCONTRADO (se creara uno nuevo)");
		f_metadata=fopen(bin_metadata,"wb+");
		config_metadata=config_create(bin_metadata);
		config_set_value(config_metadata,"BLOCK_SIZE","64");
		config_set_value(config_metadata,"BLOCKS","5192");
		config_set_value(config_metadata,"MAGIC_NUMBER","TALL_GRASS");
		config_save(config_metadata);
	}else
		config_metadata=config_create(bin_metadata);

	fclose(f_metadata);
	log_info(event_logger,"Creado archivo Metadata.bin");

	//-----------------------------------

	string_append(&bin_bitmap,dir_metadata);
	string_append(&bin_bitmap,"/Bitmap.bin");
	/*
	if(){
		//todo creacion de bitmap
	}
	 */
	//-----------------------------

	string_append(&dir_pokemon,dir_files);
	string_append(&dir_pokemon,"Pokemon/");
	mkdir(dir_pokemon,0777);
	log_info(event_logger,"Creada carpeta Pokemon");

	string_append(&bin_metadataPokemon,dir_pokemon);
	string_append(&bin_metadataPokemon,"/Metadata.bin");
	if((f_metadata=fopen(bin_metadataPokemon,"r"))==NULL){ //si no existe el archivo metadata
			f_metadata=fopen(bin_metadataPokemon,"wb+");
			config_metadata_directorio_pokemon=config_create(bin_metadataPokemon);
			config_set_value(config_metadata_directorio_pokemon,"DIRECTORY","Y");
			config_save(config_metadata_directorio_pokemon);
		}else
			config_metadata_directorio_pokemon=config_create(bin_metadata);

		fclose(f_metadata);
		log_info(event_logger,"Creado archivo Metadata.bin de directorio Pokemon");


		paths_estructuras[METADATA] = dir_metadata;
		paths_estructuras[FILES] = dir_files;
		paths_estructuras[BLOCKS] = dir_blocks;
		paths_estructuras[POKEMON] = dir_pokemon;

	free(bin_metadata);
	free(bin_bitmap);
	free(bin_metadataPokemon);
	config_destroy(config_metadata_directorio_pokemon);

}



