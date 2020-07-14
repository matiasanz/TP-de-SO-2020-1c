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

	sem_wait(&FIN_DEL_PROCESO_GAMECARD);
	return EXIT_SUCCESS;
}

void inicializar(void) {

	sem_init(&FIN_DEL_PROCESO_GAMECARD, 0, 0); //TODO, en este momento el main no esta haciendo nada

	inicializar_config();
	inicializar_logs();
	crearEstructuras();
	inicializar_conexiones();
}

void inicializar_config() {
	config = config_create("./config/gamecard.config");
}

void inicializar_logs() {

	logger = log_create("./log/gamecard.log", GAMECARD_STRING, 1,LOG_LEVEL_INFO);
	event_logger = log_create("./log/gamecard_event.log", "GAME_CARD_EVENT", 1, LOG_LEVEL_INFO);
	log_mensajes = log_create("./log/mensajes_enviados.log", "GAME_CARD_EVENT", 1, LOG_LEVEL_INFO);
}

void inicializar_conexiones() {

	id_proceso = config_get_int_value(config, "ID_PROCESO");

	conexion_broker = conexion_server_crear(
			config_get_string_value(config, "IP_BROKER"),
			config_get_string_value(config, "PUERTO_BROKER"), GAMECARD);

	pthread_mutex_init(&mutex_subscripcion, NULL);

	suscribir_y_escuchar_cola_catch_pokemon((void*) mensaje_recibido);
	suscribir_y_escuchar_cola_get_pokemon((void*) mensaje_recibido);
	suscribir_y_escuchar_cola_new_pokemon((void*) mensaje_recibido);

	conectar_gameboy((void*) mensaje_recibido);

}

void crearEstructuras(){

	TIEMPO_REINTENTO_OPERACION=config_get_int_value(config,"TIEMPO_DE_REINTENTO_OPERACION");
	TIEMPO_RETARDO_OPERACION=config_get_int_value(config,"TIEMPO_RETARDO_OPERACION");
	PUNTO_MONTAJE_TALLGRASS=config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS");
	// Inicializo semaforo
	pthread_mutex_init(&mutBitarray, NULL);
	pthread_mutex_init(&mutDiccionarioSemaforos, NULL);
	pthread_mutex_init(&envioPaquete, NULL);

	pthread_mutex_init(&mutexLogger, NULL);
	pthread_mutex_init(&mutexEventLogger, NULL);

	pthread_mutex_init(&mutexConfigMetadata, NULL);

	semaforosDePokemons=dictionary_create();

	char* dir_metadata = string_new();
	char* dir_files = string_new();
	char* dir_blocks = string_new();
	char* bin_metadata = string_new();
	char* bin_bitmap = string_new();
	char* bin_metadataFiles = string_new();

	char* bin_block = string_new();

	FILE* f_metadata;

		bool esNuevoBitmap=false;

		mkdir(PUNTO_MONTAJE_TALLGRASS,0777);

		//-------Crecion de Directorios
		string_append(&dir_metadata,PUNTO_MONTAJE_TALLGRASS);
		string_append(&dir_metadata,"Metadata/");
		mkdir(dir_metadata,0777);
		log_info(event_logger,"Creada carpeta Metadata");

		string_append(&dir_files,PUNTO_MONTAJE_TALLGRASS);
		string_append(&dir_files,"Files/");
		mkdir(dir_files,0777);
		log_info(event_logger,"Creada carpeta Files");

		string_append(&dir_blocks,PUNTO_MONTAJE_TALLGRASS);
		string_append(&dir_blocks,"Blocks/");
		mkdir(dir_blocks,0777);
		log_info(event_logger,"Creada carpeta Blocks");

		//-----Creacion del Metadata, si es que no existe
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

		//---------------Creacion de Bitmap--------------------

		string_append(&bin_bitmap,dir_metadata);
		string_append(&bin_bitmap,"/Bitmap.bin");

		if((f_bitmap=fopen(bin_bitmap,"rb+"))==NULL){// si no existe archivo bitmap
			f_bitmap=fopen(bin_bitmap,"wb+");

			char* bitarray_temp=malloc(tope(config_get_int_value(config_metadata,"BLOCKS"),8));
			fwrite((void*)bitarray_temp,tope(config_get_int_value(config_metadata,"BLOCKS"),8),1,f_bitmap);
			fflush(f_bitmap);
			free(bitarray_temp);
			esNuevoBitmap=true;
		}
			fseek(f_bitmap, 0, SEEK_END);
			int file_size = ftell(f_bitmap);
			fseek(f_bitmap, 0, SEEK_SET);

			char* bitarray_str=(char*)mmap(NULL,file_size,PROT_READ | PROT_WRITE | PROT_EXEC,MAP_SHARED,fileno(f_bitmap),0);

			if(bitarray_str == (char*) -1) {
					log_error(logger, "Fallo el mmap");
			}

			fread((void*) bitarray_str, sizeof(char), file_size, f_bitmap);
			bitmap = bitarray_create_with_mode(bitarray_str, file_size, MSB_FIRST);

			//seteo de bitmap
			if(esNuevoBitmap){
				for(int i=0;i<config_get_int_value(config_metadata,"BLOCKS");i++){
					bitarray_clean_bit(bitmap,i);
				}
			}

			log_info(logger, "Creado el archivo Bitmap.bin");

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
				config_destroy(config_metadata_directorio_files);
				log_info(event_logger,"Creado archivo Metadata.bin de directorio Files");


			///---------------Verificacion y Creacion de bloques--------
				FILE* f_block;

				string_append(&bin_block,dir_blocks);
				string_append(&bin_block,"/0.bin");

				if((f_block=fopen(bin_block,"r"))==NULL){
				free(bin_block);

				int x;
				for(x=0;x<config_get_int_value(config_metadata,"BLOCKS");x++){
					bin_block = string_new();
					char* nroBloque=string_itoa(x);

					string_append(&bin_block,dir_blocks);
					string_append(&bin_block,nroBloque);
					string_append(&bin_block,".bin");
					f_block=fopen(bin_block,"wb+");
					fclose(f_block);
					free(bin_block);
					free(nroBloque);
				}
				}else{
					free(bin_block);
					fclose(f_block);
				}

				log_info(event_logger,"Creado los bloques .bin");
			//----------------------
				paths_estructuras[METADATA] = dir_metadata;
				paths_estructuras[FILES] = dir_files;
				paths_estructuras[BLOCKS] = dir_blocks;

			free(bin_metadata);
			free(bin_bitmap);
			free(bin_metadataFiles);

		}


void string_array_liberar(char** split){
	uint32_t i = 0;
	for(; split[i] != NULL;i++){
		free(split[i]);
	}

	free(split);
}

int cant_elemetos_array(char** array){
	int i=0;

	while(array[i]!=NULL){
		i++;
	}

	return i;
}
