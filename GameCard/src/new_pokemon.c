#include "gamecard.h"
#include "mensajesGamecard.h"
#include <crenito-commons/mensajes/mensaje_new_pokemon.h>

char*posicion_dar_formato(t_posicion posicion){
	return string_from_format("%u-%u", posicion.pos_x, posicion.pos_y);
}

void cerrar_archivo(t_config* metadata, char*metadata_bin){
	config_set_value("OPEN", "N");
	config_save_in_file(metadata, metadata_bin);
}

void gamecard_New_Pokemon(t_mensaje_new_pokemon* mensajeNew){

	char* especie = mensajeNew->pokemon.especie;
	pthread_mutex_t* mutexPokemon = pokemon_get_mutex(especie);

	char* dir_metadata = pokemon_find_metadata(especie);

	pthread_mutex_lock(mutexPokemon);
	t_config* config_metadata_pokemon = config_create(dir_metadata);

	if(!archivo_existe(config_metadata_pokemon)){
		config_metadata_pokemon = metadata_create_default(especie, dir_metadata);
	}

	while(archivo_abierto(config_metadata_pokemon)){ //Si no esta abierto, la misma funcion lo abre
		pthread_mutex_unlock(mutexPokemon);

		log_enunciado_intento_interrumpido_de_new_pokemon(mensajeNew);

		sleep(TIEMPO_REINTENTO_OPERACION);
		pthread_mutex_lock(mutexPokemon);
	}

	char** bloquesDelPokemon=config_get_array_value(config_metadata_pokemon,"BLOCKS");

	pthread_mutex_unlock(mutexPokemon);


	//--------Comienzo a operar con el pokemon------------

	char* stringDePosicion = posicion_dar_formato(mensajeNew->pokemon.posicion);

	if(contienePosicionEnBloques(stringDePosicion,bloquesDelPokemon)){

		actualizar_datos(stringDePosicion, bloquesDelPokemon, config_metadata_pokemon, mensajeNew);

	}else{	//rama en donde el pokemon no esta en la posicion recibida

		agregar_nueva_linea(config_metadata_pokemon, bloquesDelPokemon, mensajeNew);
	}

	free(stringDePosicion);

	log_enunciado_pokemon_guardado(mensajeNew);

	free(dir_metadata);
	//creacion de  paquete appeared pokemon y envio a Broker
	gamecard_enviar_appeared(mensajeNew);
	//------------------------
	string_array_liberar(bloquesDelPokemon);
	config_destroy(config_metadata_pokemon);

	mensaje_new_pokemon_destruir(mensajeNew);
}

//**************************************************************************************************************

void agregar_nueva_linea(t_config* config_metadata_pokemon, char**bloquesDelPokemon, t_mensaje_new_pokemon* mensajeNew){
	if(cant_elemetos_array(bloquesDelPokemon)==0){
		//rama en donde el pokemon no tiene ningun bloque asignado

		char* nuevalinea = crearLinea(mensajeNew);


		uint32_t BLOCK_SIZE = config_metadata_get_block_size();
		int cantBloquesNecesarios = bloquesNecesarios(nuevalinea, BLOCK_SIZE);

		int size; //de la nueva linea; TODO

		char* listaBloques=string_from_format("[");

		for(int i=0;i<cantBloquesNecesarios;i++){
			pthread_mutex_lock(&mutBitarray);

			uint32_t CANTIDAD_BLOQUES = config_metadata_get_blocks();
			int nrobloque=bloque_disponible(bitmap,CANTIDAD_BLOQUES);
			bitarray_set_bit(bitmap,nrobloque);
			pthread_mutex_unlock(&mutBitarray);

			char* bin_block = string_from_format("%s%u.bin", paths_estructuras[BLOCKS],nrobloque);

			if(cantBloquesNecesarios==(i+1)){
				//esto seria el ultimo bloque necesario

				char* ultimoAguardar=string_new();
				char* stringFinal=string_substring_from(nuevalinea,i*BLOCK_SIZE);

				string_append(&ultimoAguardar,stringFinal);
				int longitud=string_length(ultimoAguardar);
				guardarLinea(bin_block,ultimoAguardar,longitud);

				string_append_with_format(&listaBloques,"%u", nrobloque);
				free(ultimoAguardar);
				free(stringFinal);
			}else{

				char* cadenaAguardar=string_new();
				char* stringRecorte=string_substring(nuevalinea,i*BLOCK_SIZE, BLOCK_SIZE);

				string_append(&cadenaAguardar,stringRecorte);
				int longitud=string_length(cadenaAguardar);
				guardarLinea(bin_block,cadenaAguardar,longitud);

				string_append_with_format(&listaBloques,"%u%s",nrobloque, ",");
				free(cadenaAguardar);
				free(stringRecorte);
			}

			free(bin_block);

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
		size = string_length(nuevalinea);
;
		char* stringSize=string_itoa(size);

		//retardo para simular acceso a disco
		sleep(TIEMPO_RETARDO_OPERACION);

		pthread_mutex_t* mutexPokemon=pokemon_get_mutex(mensajeNew->pokemon.especie);

		pthread_mutex_lock(mutexPokemon);

		//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

		config_set_value(config_metadata_pokemon,"BLOCKS",listaBloques);
		config_set_value(config_metadata_pokemon,"SIZE",stringSize);
		config_set_value(config_metadata_pokemon,"OPEN","N");
		config_save(config_metadata_pokemon);

		pthread_mutex_unlock(mutexPokemon);

		//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

		pthread_mutex_lock(&mutexEventLogger);
		log_info(event_logger,"no posee bloques, se usaran los bloques vacios nro:%s",listaBloques);
		pthread_mutex_unlock(&mutexEventLogger);

		free(listaBloques);
		free(nuevalinea);
		free(stringSize);


	}else{
		//rama en donde el pokemon tiene bloques asignados

		char* nuevalinea=crearLinea(mensajeNew);
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

					int i;
					for(i=0; i<cantBloquesNecesarios; i++){

						pthread_mutex_lock(&mutBitarray);
						int nrobloque=bloque_disponible(bitmap,config_get_int_value(config_metadata,"BLOCKS"));
						bitarray_set_bit(bitmap,nrobloque);
						pthread_mutex_unlock(&mutBitarray);

						char* bin_block = string_from_format("%s%u.bin", paths_estructuras[BLOCKS], nrobloque);

						if(cantBloquesNecesarios==(i+1)){
							//esto seria el ultimo bloque necesario

							char* ultimoAguardar=string_new();
							char* stringFinal=string_substring_from(tailLineaRecortada,i*config_get_int_value(config_metadata,"BLOCK_SIZE"));

							string_append(&ultimoAguardar,stringFinal);
							int longitud=string_length(ultimoAguardar);
							guardarLinea(bin_block,ultimoAguardar,longitud);

							string_append_with_format(&listaBloques, "%u", nrobloque);
							free(ultimoAguardar);
							free(stringFinal);
						}else{

							char* cadenaAguardar=string_new();
							char* stringRecorte=string_substring(tailLineaRecortada,i*config_get_int_value(config_metadata,"BLOCK_SIZE"),config_get_int_value(config_metadata,"BLOCK_SIZE"));
							string_append(&cadenaAguardar,stringRecorte);
							int longitud=string_length(cadenaAguardar);
							guardarLinea(bin_block,cadenaAguardar,longitud);

							string_append_with_format(&listaBloques,"%u",nrobloque);
							string_append(&listaBloques,",");
							free(cadenaAguardar);
							free(stringRecorte);
						}


						free(bin_block);
					}

					string_append(&listaBloques,"]");
					free(tailLineaRecortada);
					free(recorteDesde);
				}

		size=longitudCadenaNueva+config_get_int_value(config_metadata_pokemon,"SIZE");
		char* stringSize=string_itoa(size);

		//retardo para simular acceso a disco
		simular_acceso_a_disco();

		pthread_mutex_lock(&mutDiccionarioSemaforos);
		pthread_mutex_t* mutexPokemon=dictionary_get(semaforosDePokemons,mensajeNew->pokemon.especie);
		pthread_mutex_unlock(&mutDiccionarioSemaforos);

		pthread_mutex_lock(mutexPokemon);

		//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

		config_set_value(config_metadata_pokemon,"BLOCKS",listaBloques);
		config_set_value(config_metadata_pokemon,"SIZE",stringSize);
		config_set_value(config_metadata_pokemon,"OPEN","N");
		config_save(config_metadata_pokemon);

		pthread_mutex_unlock(mutexPokemon);

		//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));


		free(listaBloques);
		free(nuevalinea);
		free(stringSize);
		free(pathBloque);

		pthread_mutex_lock(&mutexEventLogger);
		log_info(event_logger,"ya tiene bloques asignados");
		pthread_mutex_unlock(&mutexEventLogger);
	}

}

//TODO reducir y ver si se pueden sacar argumentos
void actualizar_datos(char* cadenaABuscar, char**bloquesDelPokemon, t_config*config_metadata_pokemon, t_mensaje_new_pokemon* mensajeNew){

	//rama el pokemon posee la posicion recibida

				char* listaBloques=string_new();

				//modificacion de cantidad de pokemon en posicion existente

				char* contenidoBloques=contenido_de_Bloques_con_mmap(bloquesDelPokemon);

				log_info(event_logger,"contenido bloques:: %s",contenidoBloques); //TODO antes estaba comentado

				char** lineasDelPokemon=string_split(contenidoBloques,"\n");

				char* stringPosAdaptado=string_new();
				string_append(&stringPosAdaptado,cadenaABuscar);
				string_append(&stringPosAdaptado,"=");

				char* contenidoActualizadoDeBloques=get_contenido_actualizado(lineasDelPokemon, stringPosAdaptado, mensajeNew->cantidad);

				free(stringPosAdaptado);
				free(lineasDelPokemon);
				free(contenidoBloques);

//---------comienzo a guardar los datos actualizados--------

			int cantBloquesNecesarios=bloquesNecesarios(contenidoActualizadoDeBloques,config_get_int_value(config_metadata,"BLOCK_SIZE"));

			int size;

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


			string_append_with_format(&listaBloques,"%s%s%s","[",nroDebloquesActualizado,"]");

			string_array_liberar(arrayBloqueActualizado);
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
			simular_acceso_a_disco();

			pthread_mutex_t* mutexPokemon = pokemon_get_mutex(mensajeNew->pokemon.especie);

			pthread_mutex_lock(mutexPokemon);

			//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

			config_set_value(config_metadata_pokemon,"BLOCKS",listaBloques);
			config_set_value(config_metadata_pokemon,"SIZE",stringSize);
			config_set_value(config_metadata_pokemon,"OPEN","N");
			config_save(config_metadata_pokemon);

			pthread_mutex_unlock(mutexPokemon);

			//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

			free(listaBloques);
			free(stringSize);

			pthread_mutex_lock(&mutexEventLogger);
			log_info(event_logger,"La posicion ya existe");
			pthread_mutex_unlock(&mutexEventLogger);
}


//**************************************************************************************


char*pokemon_find_carpeta(char*especie){
	return string_from_format("%s%s", paths_estructuras[FILES], especie);
}

//Retorna el path teorico del metadata de la especie
char* pokemon_find_metadata(char*especie){

	char*carpeta = pokemon_find_carpeta(especie);
	printf("carpeta: %s\n", carpeta);
	char*metadata = string_from_format("%s/Metadata.bin", carpeta);
	printf("Metadata: %s\n", metadata);
	free(carpeta);

	return metadata;
}

t_config* metadata_create_default(char*especie, char*dir_metadata){

	char*dir_carpeta = pokemon_find_carpeta(especie);

	mkdir(dir_carpeta,0777);

	FILE*f = fopen(dir_metadata, "w+b"); //TODO :/ medio falopa
	fclose(f);

	t_config* metadataDefault = config_create(dir_metadata);

	//setear por defecto
	config_set_value(metadataDefault,"DIRECTORY","N");
	config_set_value(metadataDefault,"SIZE","0");
	config_set_value(metadataDefault,"BLOCKS","[]");
	config_set_value(metadataDefault,"OPEN","N");
	config_save(metadataDefault);

	free(dir_carpeta);

	return metadataDefault;
}

char* get_contenido_actualizado(char**lineasDelPokemon, char*stringPosAdaptado, uint32_t cantidad){
	char* contenidoActualizadoDeBloques = string_new();


	for(int i=0;lineasDelPokemon[i]!=NULL;i++){
		//este if es para verificar que no este algo distinto al formato "x-y=cant"
		if(string_contains(lineasDelPokemon[i],"-") && string_contains(lineasDelPokemon[i],"=")){
			//aqui veo si es, el que hay que modificar
			if(string_starts_with(lineasDelPokemon[i],stringPosAdaptado)){

				char** posYCant=string_split(lineasDelPokemon[i],"=");

				int cantActual=atoi(posYCant[1]);

				int cantidadFinal=cantActual+cantidad;
				char* stringCantFinal=string_itoa(cantidadFinal);

				string_append(&contenidoActualizadoDeBloques,posYCant[0]);
				string_append(&contenidoActualizadoDeBloques,"=");
				string_append(&contenidoActualizadoDeBloques,stringCantFinal);
				string_append(&contenidoActualizadoDeBloques,"\n");

				free(stringCantFinal);
				string_array_liberar(posYCant);

			}else{

			string_append(&contenidoActualizadoDeBloques,lineasDelPokemon[i]);
			string_append(&contenidoActualizadoDeBloques,"\n");

			}
		}
		free(lineasDelPokemon[i]);
	}

	return contenidoActualizadoDeBloques;
}
