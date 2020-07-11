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

	sem_init(&objetivos_gamecard, 0, 0); //TODO

	inicializar_config();
	inicializar_logs();
	crearEstructuras();
	inicializar_conexiones();
}

void inicializar_config() {
	config = config_create("./config/gamecard.config");
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

	semaforosDePokemons=dictionary_create();
}

//Crear directorios
void crear_metadata(char**dir_metadata){
	*dir_metadata = string_from_format("%s%s", punto_montaje_tallgrass, "Metadata/");
	mkdir(*dir_metadata,0777);
	log_info(event_logger,"Creada carpeta Metadata");
}

void crear_carpeta_files(char** dir_files){
	*dir_files = string_from_format("%s%s", punto_montaje_tallgrass, "Files/");
	mkdir(*dir_files,0777);
	log_info(event_logger,"Creada carpeta Files");
}

void crear_carpeta_blocks(char** dir_blocks){
	*dir_blocks = string_from_format("%s%s", punto_montaje_tallgrass, "Blocks/");
	mkdir(*dir_blocks,0777);
	log_info(event_logger,"Creada carpeta Blocks");
}

void crear_directorios(char** dir_metadata, char** dir_files, char** dir_blocks){

	crear_metadata(dir_metadata);
	crear_carpeta_files(dir_files);
	crear_carpeta_blocks(dir_blocks);

}

void leer_metadata(char**dir_metadata, char**bin_metadata, FILE**f_metadata){

	//-----Creacion del Metadata, si es que no existe
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

void crear_bit_map(char** bin_bitmap, char**dir_metadata){

	bool esNuevoBitmap=false;

	string_append_with_format(bin_bitmap,"%s%s", *dir_metadata, "Bitmap.bin");

//	printf("dir bitmap: %s", *bin_bitmap);

	f_bitmap =fopen(*bin_bitmap,"rb+");

	// si no existe archivo bitmap
	if(f_bitmap==NULL){
		f_bitmap=fopen(*bin_bitmap,"wb+");

		size_t tamanioBloque = tope(config_get_int_value(config_metadata,"BLOCKS"),8);
		char* bitarray_temp=malloc(tamanioBloque);
		fwrite((void*)bitarray_temp,tamanioBloque,1,f_bitmap);
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
}

void crear_bloques(char** bin_block, char*dir_blocks){
	string_append_with_format(bin_block, "%s%s", dir_blocks, "/0.bin");

	FILE* f_block = fopen(*bin_block,"r");

	if(f_block==NULL){
		free(*bin_block);

		int i, tamanioBloques = config_get_int_value(config_metadata,"BLOCKS");
		for(i=0; i<tamanioBloques; i++){

			*bin_block = string_new();
			char* nroBloque=string_itoa(i);

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

void crearEstructuras(){

	tiempo_de_reintento_operacion=config_get_int_value(config,"TIEMPO_DE_REINTENTO_OPERACION");
	tiempo_retardo_operacion=config_get_int_value(config,"TIEMPO_RETARDO_OPERACION");
	punto_montaje_tallgrass=config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS");
	// Inicializo semaforo
	pthread_mutex_init(&mutBitarray, NULL);
	pthread_mutex_init(&mutDiccionarioSemaforos, NULL);
	pthread_mutex_init(&envioPaquete, NULL);

	pthread_mutex_init(&mutexLogger, NULL);
	pthread_mutex_init(&mutexEventLogger, NULL);


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

	mkdir(punto_montaje_tallgrass,0777);

	//-------Crecion de Directorios
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

void gamecard_New_Pokemon_ReIntento(t_mensaje_new_pokemon* unMsjNewPoke){
	sleep(tiempo_de_reintento_operacion);
	gamecard_New_Pokemon(unMsjNewPoke);
}

void gamecard_Catch_Pokemon_ReIntento(t_mensaje_appeared_catch_pokemon* unMsjCatchPoke){
	sleep(tiempo_de_reintento_operacion);
	gamecard_Catch_Pokemon(unMsjCatchPoke);
}
void gamecard_Get_Pokemon_reintento(t_mensaje_get_pokemon* unMsjGetPoke){
	sleep(tiempo_de_reintento_operacion);
	gamecard_procesar_Get_Pokemon(unMsjGetPoke);
}


int cant_elemetos_array(char** array){
	int i=0;

	while(array[i]!=NULL){
		i++;
	}

	return i;
}
void split_liberar(char** split){
	unsigned int i = 0;
	for(; split[i] != NULL;i++){
		free(split[i]);
	}
	free(split);
}

int bloque_disponible(t_bitarray* bitmap,int totalBloques){

	int i=0;
	while(i<totalBloques && bitarray_test_bit(bitmap,i)){
		i++;
	}

	return i;
}
void guardarLinea(char* path,char* nuevalinea,int len){
	FILE* f_block;

	f_block=fopen(path,"rb+");

	fseek(f_block,0,SEEK_END);

	fwrite(nuevalinea,sizeof(char),len,f_block);

	fclose(f_block);

}
char* crearLinea(t_mensaje_new_pokemon* unMsjNewPoke ){
	    char* unalinea=string_new();
	    char* stringPosX=string_itoa(unMsjNewPoke->pokemon.posicion.pos_x);
	    char* stringPosY=string_itoa(unMsjNewPoke->pokemon.posicion.pos_y);
	    char* stringCant=string_itoa(unMsjNewPoke->cantidad);

	    string_append(&unalinea,stringPosX);
		string_append(&unalinea,"-");
		string_append(&unalinea,stringPosY);
		string_append(&unalinea,"=");
		string_append(&unalinea,stringCant);
		string_append(&unalinea,"\n");

		free(stringPosX);
		free(stringPosY);
		free(stringCant);

		return unalinea;
}
int size_bloque(char* path){
	FILE* f_block;
	f_block=fopen(path,"rb");
	fseek(f_block, 0, SEEK_END);
	int file_size = ftell(f_block);
	fseek(f_block, 0, SEEK_SET);

	fclose(f_block);

	return file_size;
}
//ejemplo ("unaCadena",["1","4","5","8",NULL])
bool contiene_string_en_bloques(char* string, char**bloques){

	char* cadenaCompleta=string_new();
	FILE* archivo;
	int file_size;
	char* cadena;
	bool result;
	char* bin_block;


	int cantbloques=cant_elemetos_array(bloques);

	if(cantbloques>0){
		for(int x=0;x<cantbloques;x++){

				bin_block = string_new();
				string_append(&bin_block,paths_estructuras[BLOCKS]);
				string_append(&bin_block,bloques[x]);
				string_append(&bin_block,".bin");


				archivo=fopen(bin_block,"rb");

				fseek(archivo, 0, SEEK_END);
				file_size = ftell(archivo);
				fseek(archivo, 0, SEEK_SET);

				cadena=malloc(file_size);

				fread(cadena,sizeof(char),file_size,archivo);

				string_append(&cadenaCompleta,cadena);

				fclose(archivo);
				free(bin_block);
				free(cadena);
		}

		result=string_contains(cadenaCompleta,string);

		free(cadenaCompleta);

		return result;

	}

	free(cadenaCompleta);
	return false;
}

int espacioDisponibleEnBloque(char* path){
	return config_get_int_value(config_metadata,"BLOCK_SIZE")-size_bloque(path);
}
int bloquesNecesarios(char* lineaNueva,int maxSizeBloque){
	return (int)((string_length(lineaNueva)+(maxSizeBloque-1))/(maxSizeBloque));
}

//posString:: "x-y", ejem "1-2",["1","4","5","8",NULL]
bool contienePosicionEnBloques(char* string, char**bloques){

	bool result;

	int cantbloques=cant_elemetos_array(bloques);

	if(cantbloques>0){
		char* lineasDeBloques=contenido_de_Bloques_con_mmap(bloques);

		//----------compruebo si esta------
		char** arrayLineasDelPokmeon=string_split(lineasDeBloques,"\n");


		//a la posicion que me pasen le agrego un "=", para facilitar la busqueda
		char* stringPosAdaptado=string_new();
		string_append(&stringPosAdaptado,string);
		string_append(&stringPosAdaptado,"=");

		result=false;

		for(int i=0;arrayLineasDelPokmeon[i]!=NULL;i++){

			if(string_starts_with(arrayLineasDelPokmeon[i],stringPosAdaptado)){
				result=true;
			};
			free(arrayLineasDelPokmeon[i]);
		}


		free(stringPosAdaptado);
		free(arrayLineasDelPokmeon);
		free(lineasDeBloques);

		return result;

	}

	return false;
}

//DEPRECATED
char* contenidoDeBloques(char** bloques){
	char* cadenaCompleta=string_new();
	FILE* archivo;
	int file_size;
	char* cadena;
	char* bin_block;

	int cantbloques=cant_elemetos_array(bloques);

	if(cantbloques>0){
		for(int x=0;x<cantbloques;x++){

				bin_block = string_new();
				string_append(&bin_block,paths_estructuras[BLOCKS]);
				string_append(&bin_block,bloques[x]);
				string_append(&bin_block,".bin");


				archivo=fopen(bin_block,"rb");

				fseek(archivo, 0, SEEK_END);
				file_size = ftell(archivo);
				fseek(archivo, 0, SEEK_SET);

				cadena=malloc(file_size);

				fread(cadena,sizeof(char),file_size,archivo);

				string_append(&cadenaCompleta,cadena);

				fclose(archivo);
				free(bin_block);
				free(cadena);
		}
	}

	return cadenaCompleta;
}
void sobrescribirLineas(char* path,char* nuevalinea,int len){
	FILE* f_block;

	f_block=fopen(path,"wb+");

	fseek(f_block,0,SEEK_SET);

	fwrite(nuevalinea,sizeof(char),len,f_block);

	fclose(f_block);

}

char* contenido_de_Bloques_con_mmap(char** bloques){
	char* cadenaCompleta=string_new();

	FILE* archivo;
	int file_size;
	char* cadena;
	char* bin_block;

	int cantbloques=cant_elemetos_array(bloques);

	if(cantbloques>0){
		for(int x=0;x<cantbloques;x++){

		bin_block = string_new();
		string_append(&bin_block,paths_estructuras[BLOCKS]);
		string_append(&bin_block,bloques[x]);
		string_append(&bin_block,".bin");

		archivo=fopen(bin_block,"rb+");

		fseek(archivo, 0, SEEK_END);
		file_size = ftell(archivo);
		fseek(archivo, 0, SEEK_SET);

		cadena=(char*)mmap(NULL,sizeof(char)*file_size,PROT_READ | PROT_WRITE | PROT_EXEC,MAP_SHARED,fileno(archivo),0);

		string_append(&cadenaCompleta,cadena);


		munmap(cadena,file_size);
		fclose(archivo);
		free(bin_block);

		}
	}

	return cadenaCompleta;
}
