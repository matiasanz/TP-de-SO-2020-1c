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
	tiempo_retardo_operacion=config_get_int_value(config,"TIEMPO_RETARDO_OPERACION");
	punto_montaje_tallgrass=config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS");

	char* dir_metadata = string_new();
	char* dir_files = string_new();
	char* dir_blocks = string_new();
	char* bin_metadata = string_new();
	char* bin_bitmap = string_new();
	char* bin_metadataFiles = string_new();

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
	FILE* f_bitmap;

	string_append(&bin_bitmap,dir_metadata);
	string_append(&bin_bitmap,"/Bitmap.bin");

	if((f_bitmap=fopen(bin_bitmap,"rb+"))==NULL){// si no existe archivo bitmap
		f_bitmap=fopen(bin_bitmap,"wb+");
	}
	//todo
	fclose(f_bitmap);

	//-----------------------------

	string_append(&bin_metadataFiles,dir_files);
	string_append(&bin_metadataFiles,"/Metadata.bin");
	if((f_metadata=fopen(bin_metadataFiles,"r"))==NULL){ //si no existe el archivo metadata
			f_metadata=fopen(bin_metadataFiles,"wb+");
			config_metadata_directorio_files=config_create(bin_metadataFiles);
			config_set_value(config_metadata_directorio_files,"DIRECTORY","Y");
			config_save(config_metadata_directorio_files);
		}else
			config_metadata_directorio_files=config_create(bin_metadataFiles);

		fclose(f_metadata);
		log_info(event_logger,"Creado archivo Metadata.bin de directorio Files");


		paths_estructuras[METADATA] = dir_metadata;
		paths_estructuras[FILES] = dir_files;
		paths_estructuras[BLOCKS] = dir_blocks;

	free(bin_metadata);
	free(bin_bitmap);
	free(bin_metadataFiles);
	config_destroy(config_metadata_directorio_files);

}

void gamecard_New_Pokemon(t_mensaje_new_pokemon* unMsjNewPoke){
	char* dir_unNuevoPokemon = string_new();
	char* bin_metadata = string_new();

	FILE* f_metadata;

	t_config* config_metadata_pokemon;

	string_append(&dir_unNuevoPokemon,paths_estructuras[FILES]);
	string_append(&dir_unNuevoPokemon,unMsjNewPoke->pokemon.especie);
	mkdir(dir_unNuevoPokemon,0777);

	string_append(&bin_metadata,dir_unNuevoPokemon);
	string_append(&bin_metadata,"/Metadata.bin");

	if((f_metadata=fopen(bin_metadata,"r"))==NULL){ //si no existe el archivo metadata
			f_metadata=fopen(bin_metadata,"wb+");
			config_metadata_pokemon=config_create(bin_metadata);
			config_set_value(config_metadata_pokemon,"DIRECTORY","N");
			config_set_value(config_metadata_pokemon,"SIZE","0");
			config_set_value(config_metadata_pokemon,"BLOCKS","[]");
			config_set_value(config_metadata_pokemon,"OPEN","Y");
			config_save(config_metadata_pokemon);
		}else
			config_metadata_pokemon=config_create(bin_metadata);
		fclose(f_metadata);

		//todo buscar en el bitmap los bloques libres




		config_destroy(config_metadata_pokemon);
		free(bin_metadata);
		free(dir_unNuevoPokemon);

	log_info(event_logger,"pokemon guardado");
}
void gamecard_Catch_Pokemon(t_mensaje_appeared_catch_pokemon* unMsjCatchPoke){

}
void gamecard_Get_Pokemon(t_mensaje_get_pokemon* unMsjGetPoke){

}

