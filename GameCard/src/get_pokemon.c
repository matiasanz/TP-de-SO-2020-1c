# include "gamecard.h"

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

		pthread_mutex_lock(&mutexLogger);
		log_info(logger,"Mensaje:%s, se localizaron 0 posiciones para %s",GET_POKEMON_STRING,unMsjGetPoke->especie);
		pthread_mutex_unlock(&mutexLogger);

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


			pthread_mutex_lock(&mutexLogger);

			log_error(logger,"El archivo pokemon esta abierto, esta operacion se reintentara luego: GET_POKEMON ::%s",unMsjGetPoke->especie);

			pthread_mutex_unlock(&mutexLogger);

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

		pthread_mutex_lock(&mutexLogger);
		log_info(logger,"Mensaje:%s, se localizaron %i posiciones para %s,->>>: %s",GET_POKEMON_STRING,list_size(listaDePosiciones),unMsjGetPoke->especie,posicionesString);
		pthread_mutex_unlock(&mutexLogger);

		free(posicionesString);
		split_liberar(bloquesDelPokemon);
		config_destroy(config_metadata_pokemon);
	}



	//---------creacion del paquete localized_pokemon y envio a Broker---------

	t_mensaje_localized_pokemon* mensajeAEnviar=mensaje_localized_pokemon_crear(unMsjGetPoke->especie,listaDePosiciones);
	mensaje_localized_pokemon_set_id_correlativo(mensajeAEnviar,mensaje_get_pokemon_get_id(unMsjGetPoke));

	t_paquete_header header=paquete_header_crear(MENSAJE,GAMECARD,LOCALIZED_POKEMON, id_proceso);
	t_buffer* bufferDepaquete=mensaje_localized_pokemon_serializar(mensajeAEnviar);
	t_paquete* paqueteAEnviar=paquete_crear(header,bufferDepaquete);

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
	mensaje_get_pokemon_destruir(unMsjGetPoke);
	mensaje_localized_pokemon_destruir(mensajeAEnviar);
	//conexion_server_destruir(unaConexion);
	paquete_destruir(paqueteAEnviar);
	//si uso list_destroy_and_destroy_elements(listaDePosiciones, (void*) posicion_destruir)
	//me dice en valgrind, free invalidos, puede que las posiciones
	//hayan sido libereadas en el mensaje_localized_pokemon_destruir() <-- Confirmo.
	list_destroy(listaDePosiciones);


}
