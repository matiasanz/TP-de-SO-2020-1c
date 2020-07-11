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

	sem_init(&objetivos_gamecard, 0, 0); //TODO, en este momento el main no esta haciendo nada

	inicializar_config();
	inicializar_logs();
	crearEstructuras();
	inicializar_conexiones();
}

void inicializar_config() {
	config = config_create("./config/gamecard.config");
}

void leer_config(){
	TIEMPO_REINTENTO_OPERACION=config_get_int_value(config,"TIEMPO_DE_REINTENTO_OPERACION");
	TIEMPO_RETARDO_OPERACION=config_get_int_value(config,"TIEMPO_RETARDO_OPERACION");
	PUNTO_MONTAJE_TALLGRASS=config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS");
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

void inicializar_logs() {

	logger = log_create("./log/gamecard.log", GAMECARD_STRING, 1,LOG_LEVEL_INFO);
	event_logger = log_create("./log/gamecard_event.log", "GAME_CARD_EVENT", 1, LOG_LEVEL_INFO);
}

//**********************
void inicializar_semaforos(){
	pthread_mutex_init(&mutBitarray, NULL);
	pthread_mutex_init(&mutDiccionarioSemaforos, NULL);
	pthread_mutex_init(&envioPaquete, NULL);

	pthread_mutex_init(&mutexLogger, NULL);
	pthread_mutex_init(&mutexEventLogger, NULL);

	pthread_mutex_init(&mutexConfigMetadata, NULL);

	semaforosDePokemons=dictionary_create();
}

void crearEstructuras(){

	leer_config(); //Observacion: a esta altura, aun no se sabria si hay config TODO
	// Inicializo semaforo
	inicializar_semaforos();

	char* dir_metadata  	= string_new();
	char* dir_files     	= string_new();
	char* dir_blocks    	= string_new();
	char* bin_metadata  	= string_new();
	char* bin_metadataFiles = string_new();

	char* bin_block = string_new();

	mkdir(PUNTO_MONTAJE_TALLGRASS,0777);

	//-------Crecion de Directorios
	crear_directorios(&dir_metadata, &dir_files, &dir_blocks);

	FILE*f_metadata; //TODO ver si vale la pena

	leer_metadata(&dir_metadata, &bin_metadata, &f_metadata);



//	printf("\n\nCree -> metadata: %s\n files: %s\n blocks: %s\n bin metadata: %s\n\n", dir_metadata, dir_files, dir_blocks, bin_metadata);

	//---------------Creacion de Bitmap--------------------

	crear_bitmap(/*&bin_bitmap, */&dir_metadata);

	//-----------------------------

	string_append_with_format(&bin_metadataFiles,"%s%s", dir_files,"/Metadata.bin");

	f_metadata=fopen(bin_metadataFiles,"r");

	if(f_metadata==NULL){ //si no existe el archivo metadata

		f_metadata=fopen(bin_metadataFiles,"wb+");
		config_metadata_directorio_files=config_create(bin_metadataFiles);
		config_set_value(config_metadata_directorio_files,"DIRECTORY","Y");
		config_save(config_metadata_directorio_files);

	}

	else config_metadata_directorio_files=config_create(bin_metadataFiles);

	fclose(f_metadata);
	config_destroy(config_metadata_directorio_files);
	log_info(event_logger,"Creado archivo Metadata.bin de directorio Files");


	///---------------Verificacion y Creacion de bloques--------

	crear_bloques(&bin_block, dir_blocks);
	//----------------------
		paths_estructuras[METADATA] = dir_metadata;
		paths_estructuras[FILES] = dir_files;
		paths_estructuras[BLOCKS] = dir_blocks;

	free(bin_metadata);
	free(bin_metadataFiles);

}

//Crear directorios
void crear_metadata(char**dir_metadata){
	*dir_metadata = string_from_format("%s%s", PUNTO_MONTAJE_TALLGRASS, "Metadata/");
	mkdir(*dir_metadata,0777);
	log_info(event_logger,"Creada carpeta Metadata");
}

void crear_carpeta_files(char** dir_files){
	*dir_files = string_from_format("%s%s", PUNTO_MONTAJE_TALLGRASS, "Files/");
	mkdir(*dir_files,0777);
	log_info(event_logger,"Creada carpeta Files");
}

void crear_carpeta_blocks(char** dir_blocks){
	*dir_blocks = string_from_format("%s%s", PUNTO_MONTAJE_TALLGRASS, "Blocks/");
	mkdir(*dir_blocks,0777);
	log_info(event_logger,"Creada carpeta Blocks");
}

void crear_directorios(char** dir_metadata, char** dir_files, char** dir_blocks){

	crear_metadata(dir_metadata);
	crear_carpeta_files(dir_files);
	crear_carpeta_blocks(dir_blocks);

}

void leer_metadata(char**dir_metadata, char**bin_metadata, FILE**f_metadata){

	//-----Creacion del Metadata (config), si es que no existe
	string_append(bin_metadata,*dir_metadata);
	string_append(bin_metadata,"Metadata.bin"); //Va en la funcion de abajo TODO

	*f_metadata=fopen(*bin_metadata,"r");

	if(*f_metadata==NULL){ //si no existe el archivo metadata
		log_info(event_logger,"[ERROR FATAL] FILESYSTEM NO ENCONTRADO (se creara uno nuevo)");
		*f_metadata=fopen(*bin_metadata,"wb+");
		config_metadata=config_create(*bin_metadata);
		config_set_value(config_metadata,"BLOCK_SIZE","64");
		config_set_value(config_metadata,"BLOCKS","5192");
		config_set_value(config_metadata,"MAGIC_NUMBER","TALL_GRASS");
		config_save(config_metadata);
	}

	else {
		config_metadata=config_create(*bin_metadata);
	}

	fclose(*f_metadata);
	log_info(event_logger,"Creado archivo Metadata.bin");
}

void crear_bitmap(char**dir_metadata){

	bool esNuevoBitmap=false;

	char* bin_bitmap = string_from_format("%s%s", *dir_metadata, "Bitmap.bin");

//	printf("dir bitmap: %s", *bin_bitmap);

	f_bitmap =fopen(bin_bitmap,"rb+");

	// si no existe archivo bitmap
	if(f_bitmap==NULL){
		f_bitmap=fopen(bin_bitmap,"wb+");


		size_t TamanioBloques = tope(config_metadata_get_blocks(),8); //TODO validar
		char* bitarray_temp=malloc(TamanioBloques);
		fwrite((void*)bitarray_temp,TamanioBloques,1,f_bitmap);
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
		uint32_t TamanioBloque = config_metadata_get_block_size();
		for(int i=0; i<TamanioBloque; i++){
			bitarray_clean_bit(bitmap,i);
		}
	}

	log_info(logger, "Creado el archivo Bitmap.bin");

	free(bin_bitmap);
}

void crear_bloques(char** bin_block, char*dir_blocks){
	string_append_with_format(bin_block, "%s/0.bin", dir_blocks);

	FILE* f_block = fopen(*bin_block,"r");

	if(f_block==NULL){
		free(*bin_block);
		uint32_t TamanioBloque = config_metadata_get_block_size();
		int i;
		for(i=0; i<TamanioBloque; i++){

			*bin_block = string_new();
			char* nroBloque = string_itoa(i);

			string_append_with_format(bin_block, "%s%s", dir_blocks, nroBloque, ".bin");

			f_block=fopen(*bin_block,"wb+");

			fclose(f_block);
			free(*bin_block);
			free(nroBloque);
		}

	}else{
		free(*bin_block);
		fclose(f_block);
	}

	log_info(event_logger,"Creado los bloques .bin");
}

//**********************

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
