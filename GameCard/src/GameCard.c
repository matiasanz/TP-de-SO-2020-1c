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
	// Inicializo semaforo
	pthread_mutex_init(&mutBitarray, NULL);
	pthread_mutex_init(&mutDiccionarioSemaforos, NULL);
	pthread_mutex_init(&envioPaquete, NULL);


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

				pthread_mutex_lock(&mutDiccionarioSemaforos);
				//este if es para cuando ya existe el pokemon en disco, pero no su mutex
				if(!dictionary_has_key(semaforosDePokemons,unMsjNewPoke->pokemon.especie)){

					pthread_mutex_t* mutexMetadataPokemon=malloc(sizeof(pthread_mutex_t));
					pthread_mutex_init(mutexMetadataPokemon, NULL);
					dictionary_put(semaforosDePokemons,unMsjNewPoke->pokemon.especie,mutexMetadataPokemon);

				}
				pthread_mutex_unlock(&mutDiccionarioSemaforos);

				pthread_mutex_lock(&mutDiccionarioSemaforos);

				pthread_mutex_t* pokeMut1=dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie);

				pthread_mutex_unlock(&mutDiccionarioSemaforos);

				pthread_mutex_lock(pokeMut1);

				//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

				t_config* metadata_pokemon_default=config_create(bin_metadata);
				config_set_value(metadata_pokemon_default,"DIRECTORY","N");
				config_set_value(metadata_pokemon_default,"SIZE","0");
				config_set_value(metadata_pokemon_default,"BLOCKS","[]");
				config_set_value(metadata_pokemon_default,"OPEN","N");
				config_save(metadata_pokemon_default);
				config_destroy(metadata_pokemon_default);

				pthread_mutex_lock(&mutDiccionarioSemaforos);

				pthread_mutex_t* pokeMut2=dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie);

				pthread_mutex_unlock(&mutDiccionarioSemaforos);


				pthread_mutex_unlock(pokeMut2);

				//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

			}else{
				pthread_mutex_lock(&mutDiccionarioSemaforos);
				//este if es para cuando ya existe el pokemon en disco, pero no su mutex
				if(!dictionary_has_key(semaforosDePokemons,unMsjNewPoke->pokemon.especie)){

					pthread_mutex_t* mutexMetadataPokemon=malloc(sizeof(pthread_mutex_t));
					pthread_mutex_init(mutexMetadataPokemon, NULL);
					dictionary_put(semaforosDePokemons,unMsjNewPoke->pokemon.especie,mutexMetadataPokemon);

				}
				pthread_mutex_unlock(&mutDiccionarioSemaforos);
				/*
				pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

				config_metadata_pokemon=config_create(bin_metadata);

				pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));
				*/

			}
			fclose(f_metadata);

			pthread_mutex_lock(&mutDiccionarioSemaforos);

			pthread_mutex_t* pokeMut3=dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie);

			pthread_mutex_unlock(&mutDiccionarioSemaforos);

			pthread_mutex_lock(pokeMut3);

			//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

			config_metadata_pokemon=config_create(bin_metadata);
			char* estadoArchivo=config_get_string_value(config_metadata_pokemon,"OPEN");
			bool abierto=true;

			if(strcmp(estadoArchivo,"N")==0){
			abierto=false;
			config_set_value(config_metadata_pokemon,"OPEN","Y");
			config_save(config_metadata_pokemon);

			}


			pthread_mutex_lock(&mutDiccionarioSemaforos);

			pthread_mutex_t* pokeMut4=dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie);

			pthread_mutex_unlock(&mutDiccionarioSemaforos);

			pthread_mutex_unlock(pokeMut4);

			//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

			//------Ver si el archivo esta abierto------------
			if(abierto){
				//abro otro hilo con un sleep que volvera a atender al Mensaje

				config_destroy(config_metadata_pokemon);
				free(bin_metadata);
				free(dir_unNuevoPokemon);

				log_info(event_logger,"Esta operacion se reintentara luego: New_Pokemon ::%s ::pos (%i,%i)::cant %i"
										,unMsjNewPoke->pokemon.especie
										,unMsjNewPoke->pokemon.posicion.pos_x
										,unMsjNewPoke->pokemon.posicion.pos_y
										,unMsjNewPoke->cantidad);
				pthread_t unHilo;
				pthread_create(&unHilo, NULL,(void*) gamecard_New_Pokemon_ReIntento, unMsjNewPoke);
				pthread_detach(unHilo);


				//y finalizo este hilo
				pthread_cancel(pthread_self());
				log_info(event_logger,"no se corto el hilo");

			}

			/*
			pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));
			config_set_value(config_metadata_pokemon,"OPEN","Y");
			config_save(config_metadata_pokemon);
			pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));
	*/

			char** bloquesDelPokemon=config_get_array_value(config_metadata_pokemon,"BLOCKS");


		//--------Comienzo a operar con el pokemon------------

		char* cadenaABuscar=string_new();
		char* stringPosX=string_itoa(unMsjNewPoke->pokemon.posicion.pos_x);
		char* stringPosY=string_itoa(unMsjNewPoke->pokemon.posicion.pos_y);

		string_append(&cadenaABuscar,stringPosX);
		string_append(&cadenaABuscar,"-");
		string_append(&cadenaABuscar,stringPosY);


		if(contienePosicionEnBloques(cadenaABuscar,bloquesDelPokemon)){
			//rama el pokemon posee la posicion recibida

			char* listaBloques=string_new();
			int size;

			//modificacion de cantidad de pokemon en posicion existente

			char* contenidoBloques=contenido_de_Bloques_con_mmap(bloquesDelPokemon);

			//log_info(event_logger,"contenido bloques:: %s",contenidoBloques);

			char** lineasDelPokemon=string_split(contenidoBloques,"\n");

			char* stringPosAdaptado=string_new();
			string_append(&stringPosAdaptado,cadenaABuscar);
			string_append(&stringPosAdaptado,"=");

			char* contenidoActualizadoDeBloques=string_new();

			for(int i=0;lineasDelPokemon[i]!=NULL;i++){
				//este if es para verificar que no este algo distinto al formato "x-y=cant"
				if(string_contains(lineasDelPokemon[i],"-") && string_contains(lineasDelPokemon[i],"=")){
				//aqui veo si es, el que hay que modificar
				if(string_starts_with(lineasDelPokemon[i],stringPosAdaptado)){

					char** posYCant=string_split(lineasDelPokemon[i],"=");

					int cantActual=atoi(posYCant[1]);

					int cantidadFinal=cantActual+unMsjNewPoke->cantidad;
					char* stringCantFinal=string_itoa(cantidadFinal);

					string_append(&contenidoActualizadoDeBloques,posYCant[0]);
					string_append(&contenidoActualizadoDeBloques,"=");
					string_append(&contenidoActualizadoDeBloques,stringCantFinal);
					string_append(&contenidoActualizadoDeBloques,"\n");

					free(stringCantFinal);
					split_liberar(posYCant);

				}else{

				string_append(&contenidoActualizadoDeBloques,lineasDelPokemon[i]);
				string_append(&contenidoActualizadoDeBloques,"\n");

				}
				}
				free(lineasDelPokemon[i]);
			}

			free(stringPosAdaptado);
			free(lineasDelPokemon);
			free(contenidoBloques);

			//---------comienzo a guardar los datos actualizados--------

			int cantBloquesNecesarios=bloquesNecesarios(contenidoActualizadoDeBloques,config_get_int_value(config_metadata,"BLOCK_SIZE"));


			if(cantBloquesNecesarios>cant_elemetos_array(bloquesDelPokemon)){
				//caso en el que necesito mas bloques de los que tenia
				int cantBloquesFaltantes=cantBloquesNecesarios-cant_elemetos_array(bloquesDelPokemon);

				char* nroDebloquesActualizado=string_new();

				for(int x=0;x<cant_elemetos_array(bloquesDelPokemon);x++){
					string_append(&nroDebloquesActualizado,bloquesDelPokemon[x]);
					string_append(&nroDebloquesActualizado,",");
				}

				for(int b=0;b<cantBloquesFaltantes;b++){
				pthread_mutex_lock(&mutBitarray);
				int nrobloque=bloque_disponible(bitmap,config_get_int_value(config_metadata,"BLOCKS"));
				bitarray_set_bit(bitmap,nrobloque);
				pthread_mutex_unlock(&mutBitarray);


				char* stringNroBloque=string_itoa(nrobloque);
					if(cantBloquesFaltantes==(b+1)){
						string_append(&nroDebloquesActualizado,stringNroBloque);
					}else{
						string_append(&nroDebloquesActualizado,stringNroBloque);
						string_append(&nroDebloquesActualizado,",");
					}

				free(stringNroBloque);
				}

				char** arrayBloqueActualizado=string_split(nroDebloquesActualizado,",");


				for(int y=0;y<cantBloquesNecesarios;y++){

				char* bin_block = string_new();
				string_append(&bin_block,paths_estructuras[BLOCKS]);
				string_append(&bin_block,arrayBloqueActualizado[y]);
				string_append(&bin_block,".bin");


				if(cantBloquesNecesarios==(y+1)){
					//esto seria el ultimo bloque necesario

					char* ultimoAguardar=string_new();
					char* stringFinal=string_substring_from(contenidoActualizadoDeBloques,y*config_get_int_value(config_metadata,"BLOCK_SIZE"));

					string_append(&ultimoAguardar,stringFinal);
					int longitud=string_length(ultimoAguardar);
					sobrescribirLineas(bin_block,ultimoAguardar,longitud);
					free(ultimoAguardar);
					free(stringFinal);
				}else{

					char* cadenaAguardar=string_new();
					char* stringRecorte=string_substring(contenidoActualizadoDeBloques,y*config_get_int_value(config_metadata,"BLOCK_SIZE"),config_get_int_value(config_metadata,"BLOCK_SIZE"));
					string_append(&cadenaAguardar,stringRecorte);
					int longitud=string_length(cadenaAguardar);
					sobrescribirLineas(bin_block,cadenaAguardar,longitud);

					free(cadenaAguardar);
					free(stringRecorte);
				}

				free(bin_block);
			}

			size=string_length(contenidoActualizadoDeBloques);


			string_append(&listaBloques,"[");
			string_append(&listaBloques,nroDebloquesActualizado);
			string_append(&listaBloques,"]");


			split_liberar(arrayBloqueActualizado);
			free(nroDebloquesActualizado);

			}else{
				//con los bloques asignados que tiene el pokemon bastan

				for(int y=0;y<cantBloquesNecesarios;y++){

					char* bin_block = string_new();
					string_append(&bin_block,paths_estructuras[BLOCKS]);
					string_append(&bin_block,bloquesDelPokemon[y]);
					string_append(&bin_block,".bin");


					if(cantBloquesNecesarios==(y+1)){
						//esto seria el ultimo bloque necesario

						char* ultimoAguardar=string_new();
						char* stringFinal=string_substring_from(contenidoActualizadoDeBloques,y*config_get_int_value(config_metadata,"BLOCK_SIZE"));
						//primer Alternativa
						string_append(&ultimoAguardar,stringFinal);
						int longitud=string_length(ultimoAguardar);

						//segunda alternativa
						//int longitud=string_length(contenidoActualizadoDeBloques)-(y*config_get_int_value(config_metadata,"BLOCK_SIZE"));
						//string_append(&ultimoAguardar,string_substring(contenidoActualizadoDeBloques,y*config_get_int_value(config_metadata,"BLOCK_SIZE"),longitud));

						sobrescribirLineas(bin_block,ultimoAguardar,longitud);
						free(ultimoAguardar);
						free(stringFinal);
					}else{

						char* cadenaAguardar=string_new();
						char* stringRecorte=string_substring(contenidoActualizadoDeBloques,y*config_get_int_value(config_metadata,"BLOCK_SIZE"),config_get_int_value(config_metadata,"BLOCK_SIZE"));
						string_append(&cadenaAguardar,stringRecorte);
						int longitud=string_length(cadenaAguardar);
						sobrescribirLineas(bin_block,cadenaAguardar,longitud);

						free(cadenaAguardar);
						free(stringRecorte);
					}

					free(bin_block);
				}

				size=string_length(contenidoActualizadoDeBloques);
				string_append(&listaBloques,config_get_string_value(config_metadata_pokemon,"BLOCKS"));


			}

			free(contenidoActualizadoDeBloques);

			char* stringSize=string_itoa(size);
			//retardo para simular acceso a disco
			sleep(tiempo_retardo_operacion);

			pthread_mutex_lock(&mutDiccionarioSemaforos);

			pthread_mutex_t* pokeMut5=dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie);

			pthread_mutex_unlock(&mutDiccionarioSemaforos);

			pthread_mutex_lock(pokeMut5);

			//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

			config_set_value(config_metadata_pokemon,"BLOCKS",listaBloques);
			config_set_value(config_metadata_pokemon,"SIZE",stringSize);
			config_set_value(config_metadata_pokemon,"OPEN","N");
			config_save(config_metadata_pokemon);


			pthread_mutex_lock(&mutDiccionarioSemaforos);

			pthread_mutex_t* pokeMut6=dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie);

			pthread_mutex_unlock(&mutDiccionarioSemaforos);

			pthread_mutex_unlock(pokeMut6);

			//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

			free(listaBloques);
			free(stringSize);

			log_info(event_logger,"La posicion ya existe");

		}else{
		//rama en donde el pokemon no esta en la posicion recibida



			if(cant_elemetos_array(bloquesDelPokemon)==0){
				//rama en donde el pokemon no tiene ningun bloque asignado

				char* listaBloques=string_new();
				int size;
				char* nuevalinea=crearLinea(unMsjNewPoke);
				int longitud=string_length(nuevalinea);

				int cantBloquesNecesarios=bloquesNecesarios(nuevalinea,config_get_int_value(config_metadata,"BLOCK_SIZE"));

				string_append(&listaBloques,"[");

				for(int y=0;y<cantBloquesNecesarios;y++){

					pthread_mutex_lock(&mutBitarray);
					int nrobloque=bloque_disponible(bitmap,config_get_int_value(config_metadata,"BLOCKS"));
					bitarray_set_bit(bitmap,nrobloque);
					pthread_mutex_unlock(&mutBitarray);

					char* bin_block = string_new();
					char* stringNrobloque=string_itoa(nrobloque);

					string_append(&bin_block,paths_estructuras[BLOCKS]);
					string_append(&bin_block,stringNrobloque);
					string_append(&bin_block,".bin");


					if(cantBloquesNecesarios==(y+1)){
						//esto seria el ultimo bloque necesario

						char* ultimoAguardar=string_new();
						char* stringFinal=string_substring_from(nuevalinea,y*config_get_int_value(config_metadata,"BLOCK_SIZE"));

						string_append(&ultimoAguardar,stringFinal);
						int longitud=string_length(ultimoAguardar);
						guardarLinea(bin_block,ultimoAguardar,longitud);

						string_append(&listaBloques,stringNrobloque);
						free(ultimoAguardar);
						free(stringFinal);
					}else{

						char* cadenaAguardar=string_new();
						char* stringRecorte=string_substring(nuevalinea,y*config_get_int_value(config_metadata,"BLOCK_SIZE"),config_get_int_value(config_metadata,"BLOCK_SIZE"));

						string_append(&cadenaAguardar,stringRecorte);
						int longitud=string_length(cadenaAguardar);
						guardarLinea(bin_block,cadenaAguardar,longitud);

						string_append(&listaBloques,stringNrobloque);
						string_append(&listaBloques,",");
						free(cadenaAguardar);
						free(stringRecorte);
					}


					free(bin_block);
					free(stringNrobloque);
				}



				/*

				//el bitarray cuenta desde 0
				pthread_mutex_lock(&mutBitarray);
				int nrobloque=bloque_disponible(bitmap,config_get_int_value(config_metadata,"BLOCKS"));
				bitarray_set_bit(bitmap,nrobloque);
				pthread_mutex_unlock(&mutBitarray);

				char* bin_block = string_new();
				string_append(&bin_block,paths_estructuras[BLOCKS]);
				string_append(&bin_block,string_itoa(nrobloque));
				string_append(&bin_block,".bin");





				guardarLinea(bin_block,nuevalinea,longitud);

				char* listaBloques=string_new();
				string_append(&listaBloques,"[");
				string_append(&listaBloques,string_itoa(nrobloque));
				string_append(&listaBloques,"]");*/

				string_append(&listaBloques,"]");
				size=longitud;
				char* stringSize=string_itoa(size);

				//retardo para simular acceso a disco
				sleep(tiempo_retardo_operacion);

				pthread_mutex_lock(&mutDiccionarioSemaforos);

				pthread_mutex_t* pokeMut7=dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie);

				pthread_mutex_unlock(&mutDiccionarioSemaforos);

				pthread_mutex_lock(pokeMut7);

				//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

				config_set_value(config_metadata_pokemon,"BLOCKS",listaBloques);
				config_set_value(config_metadata_pokemon,"SIZE",stringSize);
				config_set_value(config_metadata_pokemon,"OPEN","N");
				config_save(config_metadata_pokemon);


				pthread_mutex_lock(&mutDiccionarioSemaforos);

				pthread_mutex_t* pokeMut8=dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie);

				pthread_mutex_unlock(&mutDiccionarioSemaforos);

				pthread_mutex_unlock(pokeMut8);

				//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));


				log_info(event_logger,"no posee bloques, se usaran los bloques vacios nro:%s",listaBloques);

				free(listaBloques);
				free(nuevalinea);
				free(stringSize);


			}else{
				//rama en donde el pokemon tiene bloques asignados

				char* nuevalinea=crearLinea(unMsjNewPoke);
				int longitudCadenaNueva=string_length(nuevalinea);
				char* pathBloque = string_new();
				char* ultimoBloque=bloquesDelPokemon[cant_elemetos_array(bloquesDelPokemon)-1];

				char* listaBloques=string_new();
				int size;

				string_append(&pathBloque,paths_estructuras[BLOCKS]);
				string_append(&pathBloque,ultimoBloque);
				string_append(&pathBloque,".bin");

						if(espacioDisponibleEnBloque(pathBloque)>=longitudCadenaNueva){
							//rama en donde se guarda la linea en el ultimo bloque que tiene asignado el pokemon,
							//no necesita pedir bloques nuevos

							guardarLinea(pathBloque,nuevalinea,longitudCadenaNueva);

							string_append(&listaBloques,config_get_string_value(config_metadata_pokemon,"BLOCKS"));

						}else{

							//rama en donde no hay espacio suficiente en sus bloques,
							//entonces buscar en el bitmap los bloques libres necesarios, para completar el guardado


							//----preparo el string del array de bloques, que ira en el metadata del pokemon
							int len_array=cant_elemetos_array(bloquesDelPokemon);

							string_append(&listaBloques,"[");
							for(int i=0;i<len_array;i++){
								string_append(&listaBloques,bloquesDelPokemon[i]);
								string_append(&listaBloques,",");
							}

							//-----recorto y guardo, lo que entre de la linea en el ultimo bloque asignado que tiene el pokemon
							int espacioDisponibleEnUltimoBloque=espacioDisponibleEnBloque(pathBloque);
							char* headlineaRecortada=string_new();
							char* recorteHasta=string_substring_until(nuevalinea,espacioDisponibleEnUltimoBloque);

							string_append(&headlineaRecortada,recorteHasta);

							guardarLinea(pathBloque,headlineaRecortada,espacioDisponibleEnUltimoBloque);

							free(headlineaRecortada);
							free(recorteHasta);

							//---------guardo el resto de la linea--------


							char* tailLineaRecortada=string_new();
							char* recorteDesde=string_substring_from(nuevalinea,espacioDisponibleEnUltimoBloque);
							string_append(&tailLineaRecortada,recorteDesde);

							int cantBloquesNecesarios=bloquesNecesarios(tailLineaRecortada,config_get_int_value(config_metadata,"BLOCK_SIZE"));

							for(int y=0;y<cantBloquesNecesarios;y++){

								pthread_mutex_lock(&mutBitarray);
								int nrobloque=bloque_disponible(bitmap,config_get_int_value(config_metadata,"BLOCKS"));
								bitarray_set_bit(bitmap,nrobloque);
								pthread_mutex_unlock(&mutBitarray);

								char* bin_block = string_new();
								char* stringNrobloque=string_itoa(nrobloque);
								string_append(&bin_block,paths_estructuras[BLOCKS]);
								string_append(&bin_block,stringNrobloque);
								string_append(&bin_block,".bin");


								if(cantBloquesNecesarios==(y+1)){
									//esto seria el ultimo bloque necesario

									char* ultimoAguardar=string_new();
									char* stringFinal=string_substring_from(tailLineaRecortada,y*config_get_int_value(config_metadata,"BLOCK_SIZE"));

									string_append(&ultimoAguardar,stringFinal);
									int longitud=string_length(ultimoAguardar);
									guardarLinea(bin_block,ultimoAguardar,longitud);

									string_append(&listaBloques,stringNrobloque);
									free(ultimoAguardar);
									free(stringFinal);
								}else{

									char* cadenaAguardar=string_new();
									char* stringRecorte=string_substring(tailLineaRecortada,y*config_get_int_value(config_metadata,"BLOCK_SIZE"),config_get_int_value(config_metadata,"BLOCK_SIZE"));
									string_append(&cadenaAguardar,stringRecorte);
									int longitud=string_length(cadenaAguardar);
									guardarLinea(bin_block,cadenaAguardar,longitud);

									string_append(&listaBloques,stringNrobloque);
									string_append(&listaBloques,",");
									free(cadenaAguardar);
									free(stringRecorte);
								}


								free(bin_block);
								free(stringNrobloque);
							}

							string_append(&listaBloques,"]");
							free(tailLineaRecortada);
							free(recorteDesde);
						}

				size=longitudCadenaNueva+config_get_int_value(config_metadata_pokemon,"SIZE");
				char* stringSize=string_itoa(size);

				//retardo para simular acceso a disco
				sleep(tiempo_retardo_operacion);

				pthread_mutex_lock(&mutDiccionarioSemaforos);

				pthread_mutex_t* pokeMut9=dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie);

				pthread_mutex_unlock(&mutDiccionarioSemaforos);

				pthread_mutex_lock(pokeMut9);

				//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

				config_set_value(config_metadata_pokemon,"BLOCKS",listaBloques);
				config_set_value(config_metadata_pokemon,"SIZE",stringSize);
				config_set_value(config_metadata_pokemon,"OPEN","N");
				config_save(config_metadata_pokemon);

				pthread_mutex_lock(&mutDiccionarioSemaforos);

				pthread_mutex_t* pokeMut10=dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie);

				pthread_mutex_unlock(&mutDiccionarioSemaforos);

				pthread_mutex_unlock(pokeMut10);

				//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));


				free(listaBloques);
				free(nuevalinea);
				free(stringSize);
				free(pathBloque);

				log_info(event_logger,"ya tiene bloques asignados");
			}

		}

		free(cadenaABuscar);
		free(stringPosX);
		free(stringPosY);


		log_info(event_logger,"pokemon guardado:%s ::pos (%i,%i)::cant %i"
				,unMsjNewPoke->pokemon.especie
				,unMsjNewPoke->pokemon.posicion.pos_x
				,unMsjNewPoke->pokemon.posicion.pos_y
				,unMsjNewPoke->cantidad);


		//creacion de  paquete appeared pokemon y envio a Broker

		t_mensaje_appeared_catch_pokemon* mensajeAEnviar=mensaje_appeared_catch_pokemon_crear(unMsjNewPoke->pokemon.especie,unMsjNewPoke->pokemon.posicion.pos_x,unMsjNewPoke->pokemon.posicion.pos_y);
		mensaje_appeared_catch_pokemon_set_id_correlativo(mensajeAEnviar,mensaje_new_pokemon_get_id(unMsjNewPoke));

		t_paquete_header header=paquete_header_crear(MENSAJE,GAMECARD,APPEARED_POKEMON);
		t_buffer* bufferDepaquete=mensaje_appeared_catch_pokemon_serializar(mensajeAEnviar);
		t_paquete* paqueteAEnviar=paquete_crear(header,bufferDepaquete);

		pthread_mutex_lock(&envioPaquete);
		t_conexion_server* unaConexion=conexion_server_crear(
					config_get_string_value(config, "IP_BROKER"),
					config_get_string_value(config, "PUERTO_BROKER"), GAMECARD);

		if(enviar(unaConexion,paqueteAEnviar)==ERROR_SOCKET){
			log_warning(logger,"NO se puede realizar la conexion con el BROKER");
		}

		pthread_mutex_unlock(&envioPaquete);
		//------------------------
		split_liberar(bloquesDelPokemon);
		config_destroy(config_metadata_pokemon);
		free(bin_metadata);
		free(dir_unNuevoPokemon);
		mensaje_new_pokemon_destruir(unMsjNewPoke);
		mensaje_appeared_catch_pokemon_destruir(mensajeAEnviar);
		conexion_server_destruir(unaConexion);
		paquete_destruir(paqueteAEnviar);

}

void gamecard_Catch_Pokemon(t_mensaje_appeared_catch_pokemon* unMsjCatchPoke){

	int pokemonAtrapado;//0 o 1

	char* bin_metadata = string_new();

	FILE* f_metadata;

	t_config* config_metadata_pokemon;

	string_append(&bin_metadata,paths_estructuras[FILES]);
	string_append(&bin_metadata,unMsjCatchPoke->pokemon.especie);

	string_append(&bin_metadata,"/Metadata.bin");

	if((f_metadata=fopen(bin_metadata,"r"))==NULL){ //si no existe el archivo metadata

		pokemonAtrapado=0;

		//retardo para simular acceso a disco
		sleep(tiempo_retardo_operacion);

		log_error(logger,"No existe el Pokemon: %s",unMsjCatchPoke->pokemon.especie);

	}else{
		log_info(event_logger,"Si existe el Pokemon: %s",unMsjCatchPoke->pokemon.especie);

		//como existe el archivo, debo usar fclose, en caso contrario, no.
		fclose(f_metadata);

		pthread_mutex_lock(&mutDiccionarioSemaforos);
		//este if es para cuando ya existe el pokemon en disco, pero no su mutex
		if(!dictionary_has_key(semaforosDePokemons,unMsjCatchPoke->pokemon.especie)){

			pthread_mutex_t* mutexMetadataPokemon=malloc(sizeof(pthread_mutex_t));
			pthread_mutex_init(mutexMetadataPokemon, NULL);
			dictionary_put(semaforosDePokemons,unMsjCatchPoke->pokemon.especie,mutexMetadataPokemon);

		}
		pthread_mutex_unlock(&mutDiccionarioSemaforos);

		pthread_mutex_lock(&mutDiccionarioSemaforos);

		pthread_mutex_t* pokeMut1=dictionary_get(semaforosDePokemons,unMsjCatchPoke->pokemon.especie);

		pthread_mutex_unlock(&mutDiccionarioSemaforos);

		//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjCatchPoke->pokemon.especie));

		pthread_mutex_lock(pokeMut1);
		config_metadata_pokemon=config_create(bin_metadata);
		char* estadoArchivo=config_get_string_value(config_metadata_pokemon,"OPEN");
		bool abierto=true;
		if(strcmp(estadoArchivo,"N")==0){
			config_set_value(config_metadata_pokemon,"OPEN","Y");
			config_save(config_metadata_pokemon);
			abierto=false;
		}
		pthread_mutex_lock(&mutDiccionarioSemaforos);

		pthread_mutex_t* pokeMut2=dictionary_get(semaforosDePokemons,unMsjCatchPoke->pokemon.especie);

		pthread_mutex_unlock(&mutDiccionarioSemaforos);

		pthread_mutex_unlock(pokeMut2);
		//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjCatchPoke->pokemon.especie));


		//------Ver si el archivo esta abierto------------
		if(abierto){
			//abro otro hilo con un sleep que volvera a atender al Mensaje

			config_destroy(config_metadata_pokemon);
			free(bin_metadata);

			log_info(event_logger,"Esta operacion se reintentara luego: Catch_Pokemon ::%s ::pos (%i,%i)"
									,unMsjCatchPoke->pokemon.especie
									,unMsjCatchPoke->pokemon.posicion.pos_x
									,unMsjCatchPoke->pokemon.posicion.pos_y);
			pthread_t unHilo;
			pthread_create(&unHilo, NULL,(void*) gamecard_Catch_Pokemon_ReIntento, unMsjCatchPoke);
			pthread_detach(unHilo);


			//y finalizo este hilo
			pthread_cancel(pthread_self());
			log_info(event_logger,"no se corto el hilo");

		}

		char** bloquesDelPokemon=config_get_array_value(config_metadata_pokemon,"BLOCKS");


		//--------comenzar a operar el pokemon-------

		char* cadenaABuscar=string_new();
		char* stringPosX=string_itoa(unMsjCatchPoke->pokemon.posicion.pos_x);
		char* stringPosY=string_itoa(unMsjCatchPoke->pokemon.posicion.pos_y);

		string_append(&cadenaABuscar,stringPosX);
		string_append(&cadenaABuscar,"-");
		string_append(&cadenaABuscar,stringPosY);


		if(contienePosicionEnBloques(cadenaABuscar,bloquesDelPokemon)){
			//rama el pokemon posee la posicion recibida

			char* listaBloques=string_new();
			int size;

			//modificacion de cantidad de pokemon en posicion existente

			char* contenidoBloques=contenido_de_Bloques_con_mmap(bloquesDelPokemon);

			char** lineasDelPokemon=string_split(contenidoBloques,"\n");

			//adapto el string de posicion, para facilitarme la busqueda con el "="
			char* stringPosAdaptado=string_new();
			string_append(&stringPosAdaptado,cadenaABuscar);
			string_append(&stringPosAdaptado,"=");

			char* contenidoActualizadoDeBloques=string_new();

			for(int i=0;lineasDelPokemon[i]!=NULL;i++){
				//este if es para verificar que no este algo distinto al formato "x-y=cant"
				if(string_contains(lineasDelPokemon[i],"-") && string_contains(lineasDelPokemon[i],"=")){
				//aqui veo si es, el que hay que modificar
				if(string_starts_with(lineasDelPokemon[i],stringPosAdaptado)){

					pokemonAtrapado=1;

					char** posYCant=string_split(lineasDelPokemon[i],"=");

					int cantActual=atoi(posYCant[1]);

					//hago la resta, porque atrapo al pokemon, hay 1 menos en esa posicion
					int cantidadFinal=cantActual - 1;
					if(cantidadFinal>0){
					char* stringCantidadFinal=string_itoa(cantidadFinal);
					string_append(&contenidoActualizadoDeBloques,posYCant[0]);
					string_append(&contenidoActualizadoDeBloques,"=");
					string_append(&contenidoActualizadoDeBloques,stringCantidadFinal);
					string_append(&contenidoActualizadoDeBloques,"\n");
					free(stringCantidadFinal);
					}

					split_liberar(posYCant);

				}else{

				string_append(&contenidoActualizadoDeBloques,lineasDelPokemon[i]);
				string_append(&contenidoActualizadoDeBloques,"\n");

				}
				}
				free(lineasDelPokemon[i]);
			}

			free(stringPosAdaptado);
			free(lineasDelPokemon);
			free(contenidoBloques);


			//---------comienzo a guardar los datos actualizados--------

			int cantBloquesNecesarios=bloquesNecesarios(contenidoActualizadoDeBloques,config_get_int_value(config_metadata,"BLOCK_SIZE"));


			if(cantBloquesNecesarios<cant_elemetos_array(bloquesDelPokemon)){

				//caso en el que necesito menos bloques de los que tenia
				//int cantBloquesSobrantes=cant_elemetos_array(bloquesDelPokemon)-cantBloquesNecesarios;

				char* nroDebloquesActualizado=string_new();

				//armo el string de los bloques que usara
				for(int x=0;x<cantBloquesNecesarios;x++){
					if(cantBloquesNecesarios==(x+1)){
						string_append(&nroDebloquesActualizado,bloquesDelPokemon[x]);
					}else{
						string_append(&nroDebloquesActualizado,bloquesDelPokemon[x]);
						string_append(&nroDebloquesActualizado,",");
					}
				}

				//limpieza de bitarray y de los bloques sobrantes
				for(int j=cantBloquesNecesarios;j<cant_elemetos_array(bloquesDelPokemon);j++){
					pthread_mutex_lock(&mutBitarray);
					int nrobloque=atoi(bloquesDelPokemon[j]);

					bitarray_clean_bit(bitmap,nrobloque);
					//limpiar el bloque.bin correspondiente

					char* clean_block = string_new();
					string_append(&clean_block,paths_estructuras[BLOCKS]);
					string_append(&clean_block,bloquesDelPokemon[j]);
					string_append(&clean_block,".bin");
					sobrescribirLineas(clean_block,"",0);
					free(clean_block);
					pthread_mutex_unlock(&mutBitarray);
				}

				char** arrayBloqueActualizado=string_split(nroDebloquesActualizado,",");

				for(int y=0;y<cantBloquesNecesarios;y++){

					char* bin_block = string_new();
					string_append(&bin_block,paths_estructuras[BLOCKS]);
					string_append(&bin_block,arrayBloqueActualizado[y]);
					string_append(&bin_block,".bin");


					if(cantBloquesNecesarios==(y+1)){
						//esto seria el ultimo bloque necesario

						char* ultimoAguardar=string_new();
						char* stringFinal=string_substring_from(contenidoActualizadoDeBloques,y*config_get_int_value(config_metadata,"BLOCK_SIZE"));

						string_append(&ultimoAguardar,stringFinal);
						int longitud=string_length(ultimoAguardar);
						sobrescribirLineas(bin_block,ultimoAguardar,longitud);
						free(ultimoAguardar);
						free(stringFinal);
					}else{

						char* cadenaAguardar=string_new();
						char* stringRecorte=string_substring(contenidoActualizadoDeBloques,y*config_get_int_value(config_metadata,"BLOCK_SIZE"),config_get_int_value(config_metadata,"BLOCK_SIZE"));
						string_append(&cadenaAguardar,stringRecorte);
						int longitud=string_length(cadenaAguardar);
						sobrescribirLineas(bin_block,cadenaAguardar,longitud);

						free(cadenaAguardar);
						free(stringRecorte);
					}

					free(bin_block);
				}

				size=string_length(contenidoActualizadoDeBloques);


				string_append(&listaBloques,"[");
				string_append(&listaBloques,nroDebloquesActualizado);
				string_append(&listaBloques,"]");


				split_liberar(arrayBloqueActualizado);
				free(nroDebloquesActualizado);

			}else{

				//con los bloques asignados que tiene el pokemon bastan

				for(int y=0;y<cantBloquesNecesarios;y++){

					char* bin_block = string_new();
					string_append(&bin_block,paths_estructuras[BLOCKS]);
					string_append(&bin_block,bloquesDelPokemon[y]);
					string_append(&bin_block,".bin");


					if(cantBloquesNecesarios==(y+1)){
						//esto seria el ultimo bloque necesario

						char* ultimoAguardar=string_new();

						char* stringFinal=string_substring_from(contenidoActualizadoDeBloques,y*config_get_int_value(config_metadata,"BLOCK_SIZE"));

						//primer Alternativa
						string_append(&ultimoAguardar,stringFinal);
						int longitud=string_length(ultimoAguardar);

						//segunda alternativa
						//int longitud=string_length(contenidoActualizadoDeBloques)-(y*config_get_int_value(config_metadata,"BLOCK_SIZE"));
						//string_append(&ultimoAguardar,string_substring(contenidoActualizadoDeBloques,y*config_get_int_value(config_metadata,"BLOCK_SIZE"),longitud));

						sobrescribirLineas(bin_block,ultimoAguardar,longitud);
						free(ultimoAguardar);
						free(stringFinal);
					}else{

						char* cadenaAguardar=string_new();
						char* stringRecorte=string_substring(contenidoActualizadoDeBloques,y*config_get_int_value(config_metadata,"BLOCK_SIZE"),config_get_int_value(config_metadata,"BLOCK_SIZE"));

						string_append(&cadenaAguardar,stringRecorte);
						int longitud=string_length(cadenaAguardar);
						sobrescribirLineas(bin_block,cadenaAguardar,longitud);

						free(cadenaAguardar);
						free(stringRecorte);
					}

					free(bin_block);
				}

				size=string_length(contenidoActualizadoDeBloques);
				string_append(&listaBloques,config_get_string_value(config_metadata_pokemon,"BLOCKS"));


			}

			free(contenidoActualizadoDeBloques);

			char* stringSize=string_itoa(size);

			//retardo para simular acceso a disco
			sleep(tiempo_retardo_operacion);

			pthread_mutex_lock(&mutDiccionarioSemaforos);

			pthread_mutex_t* pokeMut3=dictionary_get(semaforosDePokemons,unMsjCatchPoke->pokemon.especie);

			pthread_mutex_unlock(&mutDiccionarioSemaforos);

			//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjCatchPoke->pokemon.especie));
			pthread_mutex_lock(pokeMut3);

			config_set_value(config_metadata_pokemon,"BLOCKS",listaBloques);
			config_set_value(config_metadata_pokemon,"SIZE",stringSize);
			config_set_value(config_metadata_pokemon,"OPEN","N");
			config_save(config_metadata_pokemon);


			pthread_mutex_lock(&mutDiccionarioSemaforos);

			pthread_mutex_t* pokeMut4=dictionary_get(semaforosDePokemons,unMsjCatchPoke->pokemon.especie);

			pthread_mutex_unlock(&mutDiccionarioSemaforos);

			pthread_mutex_unlock(pokeMut4);

			//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjCatchPoke->pokemon.especie));

			free(listaBloques);
			free(stringSize);

			log_info(event_logger,"Un %s fue atrapado en la posicion: (%i,%i)",unMsjCatchPoke->pokemon.especie,unMsjCatchPoke->pokemon.posicion.pos_x,unMsjCatchPoke->pokemon.posicion.pos_y);



		}else{
			//rama en donde la posicion recibida no se encuentra el pokemon
			pokemonAtrapado=0;

			//retardo para simular acceso a disco
			sleep(tiempo_retardo_operacion);


			pthread_mutex_lock(&mutDiccionarioSemaforos);

			pthread_mutex_t* pokeMut5=dictionary_get(semaforosDePokemons,unMsjCatchPoke->pokemon.especie);

			pthread_mutex_unlock(&mutDiccionarioSemaforos);

			//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjCatchPoke->pokemon.especie));
			pthread_mutex_lock(pokeMut5);

			config_set_value(config_metadata_pokemon,"OPEN","N");
			config_save(config_metadata_pokemon);


			pthread_mutex_lock(&mutDiccionarioSemaforos);

			pthread_mutex_t* pokeMut6=dictionary_get(semaforosDePokemons,unMsjCatchPoke->pokemon.especie);

			pthread_mutex_unlock(&mutDiccionarioSemaforos);


			pthread_mutex_unlock(pokeMut6);

			//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjCatchPoke->pokemon.especie));

			log_error(logger,"No se encuentra la posicion: (%i,%i), para el Pokemon: %s",unMsjCatchPoke->pokemon.posicion.pos_x,unMsjCatchPoke->pokemon.posicion.pos_y,unMsjCatchPoke->pokemon.especie);

		}

		free(cadenaABuscar);
		free(stringPosX);
		free(stringPosY);
		split_liberar(bloquesDelPokemon);
		config_destroy(config_metadata_pokemon);

	}


	//---------creacion del paquete caught_pokemon y envio a Broker---------

	t_mensaje_caught_pokemon* mensajeAEnviar=mensaje_caught_pokemon_crear(pokemonAtrapado);
	mensaje_caught_pokemon_set_id_correlativo(mensajeAEnviar,mensaje_appeared_catch_pokemon_get_id(unMsjCatchPoke));

	t_paquete_header header=paquete_header_crear(MENSAJE,GAMECARD,CAUGHT_POKEMON);
	t_buffer* bufferDepaquete=mensaje_caught_pokemon_serializar(mensajeAEnviar);
	t_paquete* paqueteAEnviar=paquete_crear(header,bufferDepaquete);


	pthread_mutex_lock(&envioPaquete);
	t_conexion_server* unaConexion=conexion_server_crear(
						config_get_string_value(config, "IP_BROKER"),
						config_get_string_value(config, "PUERTO_BROKER"), GAMECARD);

	if(enviar(unaConexion,paqueteAEnviar)==ERROR_SOCKET){
		log_warning(logger,"NO se puede realizar la conexion con el BROKER");
	}
	pthread_mutex_unlock(&envioPaquete);

	//----------------

	free(bin_metadata);
	mensaje_appeared_catch_pokemon_destruir(unMsjCatchPoke);
	mensaje_caught_pokemon_destruir(mensajeAEnviar);
	conexion_server_destruir(unaConexion);
	paquete_destruir(paqueteAEnviar);

}
void gamecard_Get_Pokemon(t_mensaje_get_pokemon* unMsjGetPoke){

	t_list* listaDePosiciones=list_create();

	char* bin_metadata = string_new();

	FILE* f_metadata;

	t_config* config_metadata_pokemon;

	string_append(&bin_metadata,paths_estructuras[FILES]);
	string_append(&bin_metadata,unMsjGetPoke->especie);

	string_append(&bin_metadata,"/Metadata.bin");

	if((f_metadata=fopen(bin_metadata,"r"))==NULL){ //si no existe el archivo metadata

		//retardo para simular acceso a disco
		sleep(tiempo_retardo_operacion);

		log_info(event_logger,"Mensaje:%s, se localizaron 0 posiciones para %s",GET_POKEMON_STRING,unMsjGetPoke->especie);
	}else{

		fclose(f_metadata);

		pthread_mutex_lock(&mutDiccionarioSemaforos);
		//este if es para cuando ya existe el pokemon en disco, pero no su mutex
		if(!dictionary_has_key(semaforosDePokemons,unMsjGetPoke->especie)){

			pthread_mutex_t* mutexMetadataPokemon=malloc(sizeof(pthread_mutex_t));
			pthread_mutex_init(mutexMetadataPokemon, NULL);
			dictionary_put(semaforosDePokemons,unMsjGetPoke->especie,mutexMetadataPokemon);

		}
		pthread_mutex_unlock(&mutDiccionarioSemaforos);


		pthread_mutex_lock(&mutDiccionarioSemaforos);

		pthread_mutex_t* pokeMut1=dictionary_get(semaforosDePokemons,unMsjGetPoke->especie);

		pthread_mutex_unlock(&mutDiccionarioSemaforos);

		pthread_mutex_lock(pokeMut1);
		//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjGetPoke->especie));

		config_metadata_pokemon=config_create(bin_metadata);
		char* estadoArchivo=config_get_string_value(config_metadata_pokemon,"OPEN");
		bool abierto=true;
		if(strcmp(estadoArchivo,"N")==0){
			config_set_value(config_metadata_pokemon,"OPEN","Y");
			config_save(config_metadata_pokemon);
			abierto=false;
		}


		pthread_mutex_lock(&mutDiccionarioSemaforos);

		pthread_mutex_t* pokeMut2=dictionary_get(semaforosDePokemons,unMsjGetPoke->especie);

		pthread_mutex_unlock(&mutDiccionarioSemaforos);

		pthread_mutex_unlock(pokeMut2);
		//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjGetPoke->especie));


		//------Ver si el archivo esta abierto------------
		if(abierto){
			//abro otro hilo con un sleep que volvera a atender al Mensaje

			config_destroy(config_metadata_pokemon);
			free(bin_metadata);

			log_info(event_logger,"Esta operacion se reintentara luego: GET_POKEMON ::%s",unMsjGetPoke->especie);
			pthread_t unHilo;
			pthread_create(&unHilo, NULL,(void*) gamecard_Get_Pokemon_ReIntento, unMsjGetPoke);
			pthread_detach(unHilo);


			//y finalizo este hilo
			pthread_cancel(pthread_self());
			log_info(event_logger,"no se corto el hilo");

		}

		char** bloquesDelPokemon=config_get_array_value(config_metadata_pokemon,"BLOCKS");

		//--------comenzar a operar el pokemon-------

		if(cant_elemetos_array(bloquesDelPokemon)>0){

			char* contenidoBloques=contenido_de_Bloques_con_mmap(bloquesDelPokemon);

			char** lineasDelPokemon=string_split(contenidoBloques,"\n");

			for(int i=0;lineasDelPokemon[i]!=NULL;i++){
				//este if es para verificar que no este algo distinto al formato "x-y=cant"
				if(string_contains(lineasDelPokemon[i],"-") && string_contains(lineasDelPokemon[i],"=")){

					char** posYCant=string_split(lineasDelPokemon[i],"=");

					char** xy=string_split(posYCant[0],"-");

					int posX=atoi(xy[0]);

					int posY=atoi(xy[1]);

					t_posicion* unaPosicion=posicion_crear_ptr(posX,posY);

					list_add(listaDePosiciones,unaPosicion);

					split_liberar(xy);
					split_liberar(posYCant);
				}
				free(lineasDelPokemon[i]);
			}


			free(lineasDelPokemon);
			free(contenidoBloques);


		}

		//retardo para simular acceso a disco
		sleep(tiempo_retardo_operacion);

		pthread_mutex_lock(&mutDiccionarioSemaforos);

		pthread_mutex_t* pokeMut3=dictionary_get(semaforosDePokemons,unMsjGetPoke->especie);

		pthread_mutex_unlock(&mutDiccionarioSemaforos);

		pthread_mutex_lock(pokeMut3);

		//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjGetPoke->especie));

		config_set_value(config_metadata_pokemon,"OPEN","N");
		config_save(config_metadata_pokemon);


		pthread_mutex_lock(&mutDiccionarioSemaforos);

		pthread_mutex_t* pokeMut4=dictionary_get(semaforosDePokemons,unMsjGetPoke->especie);

		pthread_mutex_unlock(&mutDiccionarioSemaforos);

		pthread_mutex_unlock(pokeMut4);

		//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjGetPoke->especie));

		char* posicionesString=posicion_list_to_string(listaDePosiciones);
		log_info(event_logger,"Mensaje:%s, se localizaron %i posiciones para %s,->>>: %s",GET_POKEMON_STRING,list_size(listaDePosiciones),unMsjGetPoke->especie,posicionesString);

		free(posicionesString);
		split_liberar(bloquesDelPokemon);
		config_destroy(config_metadata_pokemon);
	}



	//---------creacion del paquete localized_pokemon y envio a Broker---------

	t_mensaje_localized_pokemon* mensajeAEnviar=mensaje_localized_pokemon_crear(unMsjGetPoke->especie,listaDePosiciones);
	mensaje_localized_pokemon_set_id_correlativo(mensajeAEnviar,mensaje_get_pokemon_get_id(unMsjGetPoke));

	t_paquete_header header=paquete_header_crear(MENSAJE,GAMECARD,LOCALIZED_POKEMON);
	t_buffer* bufferDepaquete=mensaje_localized_pokemon_serializar(mensajeAEnviar);
	t_paquete* paqueteAEnviar=paquete_crear(header,bufferDepaquete);

	pthread_mutex_lock(&envioPaquete);
	t_conexion_server* unaConexion=conexion_server_crear(
							config_get_string_value(config, "IP_BROKER"),
							config_get_string_value(config, "PUERTO_BROKER"), GAMECARD);

	if(enviar(unaConexion,paqueteAEnviar)==ERROR_SOCKET){
		log_warning(logger,"NO se puede realizar la conexion con el BROKER");
	}
	pthread_mutex_unlock(&envioPaquete);

	//----------------

	free(bin_metadata);
	mensaje_get_pokemon_destruir(unMsjGetPoke);
	mensaje_localized_pokemon_destruir(mensajeAEnviar);
	conexion_server_destruir(unaConexion);
	paquete_destruir(paqueteAEnviar);
	//si uso list_destroy_and_destroy_elements(listaDePosiciones, (void*) posicion_destruir)
	//me dice en valgrind, free invalidos, puede que las posiciones
	//hayan sido libereadas en el mensaje_localized_pokemon_destruir()
	list_destroy(listaDePosiciones);


}

void gamecard_New_Pokemon_ReIntento(t_mensaje_new_pokemon* unMsjNewPoke){
	sleep(tiempo_de_reintento_operacion);
	gamecard_New_Pokemon(unMsjNewPoke);
}

void gamecard_Catch_Pokemon_ReIntento(t_mensaje_appeared_catch_pokemon* unMsjCatchPoke){
	sleep(tiempo_de_reintento_operacion);
	gamecard_Catch_Pokemon(unMsjCatchPoke);
}
void gamecard_Get_Pokemon_ReIntento(t_mensaje_get_pokemon* unMsjGetPoke){
	sleep(tiempo_de_reintento_operacion);
	gamecard_Get_Pokemon(unMsjGetPoke);
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
