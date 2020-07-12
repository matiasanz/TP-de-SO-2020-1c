#include "gamecard.h"

void gamecard_Catch_Pokemon(t_mensaje_appeared_catch_pokemon* unMsjCatchPoke){

	uint32_t pokemonAtrapado;//0 o 1

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

		pthread_mutex_lock(&mutexLogger);

		log_error(logger,"No existe el Pokemon: %s",unMsjCatchPoke->pokemon.especie);

		pthread_mutex_unlock(&mutexLogger);

	}else{

		pthread_mutex_lock(&mutexEventLogger);
		log_info(event_logger,"Si existe el Pokemon: %s",unMsjCatchPoke->pokemon.especie);
		pthread_mutex_unlock(&mutexEventLogger);
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

			pthread_mutex_lock(&mutexLogger);
			log_error(logger,"El archivo pokemon esta abierto, esta operacion se reintentara luego: Catch_Pokemon ::%s ::pos (%i,%i)"
									,unMsjCatchPoke->pokemon.especie
									,unMsjCatchPoke->pokemon.posicion.pos_x
									,unMsjCatchPoke->pokemon.posicion.pos_y);

			pthread_mutex_unlock(&mutexLogger);

			pthread_t unHilo;
			pthread_create(&unHilo, NULL,(void*) gamecard_Catch_Pokemon_ReIntento, unMsjCatchPoke);
			pthread_detach(unHilo);


			//y finalizo este hilo
			pthread_cancel(pthread_self());
			log_info(event_logger,"no se corto el hilo");

		}

		//----------------------
		pthread_mutex_lock(&mutDiccionarioSemaforos);

		pthread_mutex_t* pokeMutAux1=dictionary_get(semaforosDePokemons,unMsjCatchPoke->pokemon.especie);

		pthread_mutex_unlock(&mutDiccionarioSemaforos);

		pthread_mutex_lock(pokeMutAux1);
		//------------aqui marca condicion de carrera, aunque no deberia--------------
		char** bloquesDelPokemon=config_get_array_value(config_metadata_pokemon,"BLOCKS");

		pthread_mutex_lock(&mutDiccionarioSemaforos);

		pthread_mutex_t* pokeMutAux2=dictionary_get(semaforosDePokemons,unMsjCatchPoke->pokemon.especie);

		pthread_mutex_unlock(&mutDiccionarioSemaforos);

		pthread_mutex_unlock(pokeMutAux2);


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

			pthread_mutex_lock(&mutexLogger);
			log_info(logger,"Un %s fue atrapado en la posicion: (%i,%i)",unMsjCatchPoke->pokemon.especie,unMsjCatchPoke->pokemon.posicion.pos_x,unMsjCatchPoke->pokemon.posicion.pos_y);
			pthread_mutex_unlock(&mutexLogger);


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


			pthread_mutex_lock(&mutexLogger);
			log_error(logger,"No se encuentra la posicion: (%i,%i), para el Pokemon: %s",unMsjCatchPoke->pokemon.posicion.pos_x,unMsjCatchPoke->pokemon.posicion.pos_y,unMsjCatchPoke->pokemon.especie);
			pthread_mutex_unlock(&mutexLogger);
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

	t_buffer* bufferDepaquete=mensaje_caught_pokemon_serializar(mensajeAEnviar);
	t_paquete* paqueteAEnviar=paquete_crear(MENSAJE,CAUGHT_POKEMON,bufferDepaquete);


	pthread_mutex_lock(&envioPaquete);
	/*t_conexion_server* unaConexion=conexion_server_crear(
						config_get_string_value(config, "IP_BROKER"),
						config_get_string_value(config, "PUERTO_BROKER"), GAMECARD);

	*/

	int resultadoEnvio = enviar(conexion_broker,paqueteAEnviar);
	if(error_conexion(resultadoEnvio)){
		log_warning(logger,"NO se puede realizar la conexion con el BROKER");
	}

	pthread_mutex_unlock(&envioPaquete);

	//----------------

	free(bin_metadata);
	mensaje_appeared_catch_pokemon_destruir(unMsjCatchPoke);
	mensaje_caught_pokemon_destruir(mensajeAEnviar);
	//conexion_server_destruir(unaConexion);
	paquete_destruir(paqueteAEnviar);

}
