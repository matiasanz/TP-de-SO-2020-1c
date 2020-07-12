#include "gamecard.h"

void gamecard_New_Pokemon(t_mensaje_new_pokemon* mensajeNew){
	    char* dir_unNuevoPokemon = string_new();
		char* bin_metadata = string_new();

		FILE* f_metadata;

		t_config* config_metadata_pokemon;

		string_append(&dir_unNuevoPokemon,paths_estructuras[FILES]);
		string_append(&dir_unNuevoPokemon,mensajeNew->pokemon.especie);
		mkdir(dir_unNuevoPokemon,0777);

		string_append(&bin_metadata,dir_unNuevoPokemon);
		string_append(&bin_metadata,"/Metadata.bin");

		if((f_metadata=fopen(bin_metadata,"r"))==NULL){ //si no existe el archivo metadata
				f_metadata=fopen(bin_metadata,"wb+");

				pthread_mutex_lock(&mutDiccionarioSemaforos);
				//este if es para cuando ya existe el pokemon en disco, pero no su mutex
				if(!dictionary_has_key(semaforosDePokemons,mensajeNew->pokemon.especie)){

					pthread_mutex_t* mutexMetadataPokemon=malloc(sizeof(pthread_mutex_t));
					pthread_mutex_init(mutexMetadataPokemon, NULL);
					dictionary_put(semaforosDePokemons,mensajeNew->pokemon.especie,mutexMetadataPokemon);

				}
				pthread_mutex_unlock(&mutDiccionarioSemaforos);

				pthread_mutex_lock(&mutDiccionarioSemaforos);

				pthread_mutex_t* pokeMut1=dictionary_get(semaforosDePokemons,mensajeNew->pokemon.especie);

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

				pthread_mutex_t* pokeMut2=dictionary_get(semaforosDePokemons,mensajeNew->pokemon.especie);

				pthread_mutex_unlock(&mutDiccionarioSemaforos);


				pthread_mutex_unlock(pokeMut2);

				//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

			}else{
				pthread_mutex_lock(&mutDiccionarioSemaforos);
				//este if es para cuando ya existe el pokemon en disco, pero no su mutex
				if(!dictionary_has_key(semaforosDePokemons,mensajeNew->pokemon.especie)){

					pthread_mutex_t* mutexMetadataPokemon=malloc(sizeof(pthread_mutex_t));
					pthread_mutex_init(mutexMetadataPokemon, NULL);
					dictionary_put(semaforosDePokemons,mensajeNew->pokemon.especie,mutexMetadataPokemon);

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

			pthread_mutex_t* pokeMut3=dictionary_get(semaforosDePokemons,mensajeNew->pokemon.especie);

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

			pthread_mutex_t* pokeMut4=dictionary_get(semaforosDePokemons,mensajeNew->pokemon.especie);

			pthread_mutex_unlock(&mutDiccionarioSemaforos);

			pthread_mutex_unlock(pokeMut4);

			//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

			//------Ver si el archivo esta abierto------------
			if(abierto){
				//abro otro hilo con un sleep que volvera a atender al Mensaje

				config_destroy(config_metadata_pokemon);
				free(bin_metadata);
				free(dir_unNuevoPokemon);

				pthread_mutex_lock(&mutexLogger);
				log_error(logger,"El archivo pokemon esta abierto, esta operacion se reintentara luego: New_Pokemon ::%s ::pos (%i,%i)::cant %i"
										,mensajeNew->pokemon.especie
										,mensajeNew->pokemon.posicion.pos_x
										,mensajeNew->pokemon.posicion.pos_y
										,mensajeNew->cantidad);

				pthread_mutex_unlock(&mutexLogger);

				pthread_t unHilo;
				pthread_create(&unHilo, NULL,(void*) gamecard_New_Pokemon_ReIntento, mensajeNew);
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
		char* stringPosX=string_itoa(mensajeNew->pokemon.posicion.pos_x);
		char* stringPosY=string_itoa(mensajeNew->pokemon.posicion.pos_y);

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

					int cantidadFinal=cantActual+mensajeNew->cantidad;
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

			pthread_mutex_t* pokeMut5=dictionary_get(semaforosDePokemons,mensajeNew->pokemon.especie);

			pthread_mutex_unlock(&mutDiccionarioSemaforos);

			pthread_mutex_lock(pokeMut5);

			//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

			config_set_value(config_metadata_pokemon,"BLOCKS",listaBloques);
			config_set_value(config_metadata_pokemon,"SIZE",stringSize);
			config_set_value(config_metadata_pokemon,"OPEN","N");
			config_save(config_metadata_pokemon);


			pthread_mutex_lock(&mutDiccionarioSemaforos);

			pthread_mutex_t* pokeMut6=dictionary_get(semaforosDePokemons,mensajeNew->pokemon.especie);

			pthread_mutex_unlock(&mutDiccionarioSemaforos);

			pthread_mutex_unlock(pokeMut6);

			//pthread_mutex_unlock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

			free(listaBloques);
			free(stringSize);

			pthread_mutex_lock(&mutexEventLogger);
			log_info(event_logger,"La posicion ya existe");
			pthread_mutex_unlock(&mutexEventLogger);

		}else{
		//rama en donde el pokemon no esta en la posicion recibida



			if(cant_elemetos_array(bloquesDelPokemon)==0){
				//rama en donde el pokemon no tiene ningun bloque asignado

				char* listaBloques=string_new();
				int size;
				char* nuevalinea=crearLinea(mensajeNew);
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

				pthread_mutex_t* pokeMut7=dictionary_get(semaforosDePokemons,mensajeNew->pokemon.especie);

				pthread_mutex_unlock(&mutDiccionarioSemaforos);

				pthread_mutex_lock(pokeMut7);

				//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

				config_set_value(config_metadata_pokemon,"BLOCKS",listaBloques);
				config_set_value(config_metadata_pokemon,"SIZE",stringSize);
				config_set_value(config_metadata_pokemon,"OPEN","N");
				config_save(config_metadata_pokemon);


				pthread_mutex_lock(&mutDiccionarioSemaforos);

				pthread_mutex_t* pokeMut8=dictionary_get(semaforosDePokemons,mensajeNew->pokemon.especie);

				pthread_mutex_unlock(&mutDiccionarioSemaforos);

				pthread_mutex_unlock(pokeMut8);

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

				pthread_mutex_t* pokeMut9=dictionary_get(semaforosDePokemons,mensajeNew->pokemon.especie);

				pthread_mutex_unlock(&mutDiccionarioSemaforos);

				pthread_mutex_lock(pokeMut9);

				//pthread_mutex_lock(dictionary_get(semaforosDePokemons,unMsjNewPoke->pokemon.especie));

				config_set_value(config_metadata_pokemon,"BLOCKS",listaBloques);
				config_set_value(config_metadata_pokemon,"SIZE",stringSize);
				config_set_value(config_metadata_pokemon,"OPEN","N");
				config_save(config_metadata_pokemon);

				pthread_mutex_lock(&mutDiccionarioSemaforos);

				pthread_mutex_t* pokeMut10=dictionary_get(semaforosDePokemons,mensajeNew->pokemon.especie);

				pthread_mutex_unlock(&mutDiccionarioSemaforos);

				pthread_mutex_unlock(pokeMut10);

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

		free(cadenaABuscar);
		free(stringPosX);
		free(stringPosY);

		pthread_mutex_lock(&mutexLogger);
		log_info(logger,"pokemon guardado:%s ::pos (%i,%i)::cant %i"
				,mensajeNew->pokemon.especie
				,mensajeNew->pokemon.posicion.pos_x
				,mensajeNew->pokemon.posicion.pos_y
				,mensajeNew->cantidad);

		pthread_mutex_unlock(&mutexLogger);
		//creacion de  paquete appeared pokemon y envio a Broker

		t_mensaje_appeared_catch_pokemon* mensajeAEnviar=mensaje_appeared_catch_pokemon_crear(mensajeNew->pokemon.especie,mensajeNew->pokemon.posicion.pos_x,mensajeNew->pokemon.posicion.pos_y);
		mensaje_appeared_catch_pokemon_set_id_correlativo(mensajeAEnviar,mensaje_new_pokemon_get_id(mensajeNew));

		t_paquete_header header=paquete_header_crear(MENSAJE,GAMECARD,APPEARED_POKEMON, id_proceso);
		t_buffer* bufferDepaquete=mensaje_appeared_catch_pokemon_serializar(mensajeAEnviar);
		t_paquete* paqueteAEnviar=paquete_crear(header,bufferDepaquete);

		pthread_mutex_lock(&envioPaquete);
		int resultadoEnvio = enviar(conexion_broker, paqueteAEnviar);

		if(error_conexion(resultadoEnvio)){
			log_warning(logger,"NO se puede realizar la conexion con el BROKER");
		}

		pthread_mutex_unlock(&envioPaquete);
		//------------------------
		split_liberar(bloquesDelPokemon);
		config_destroy(config_metadata_pokemon);
		free(bin_metadata);
		free(dir_unNuevoPokemon);
		mensaje_new_pokemon_destruir(mensajeNew);
		mensaje_appeared_catch_pokemon_destruir(mensajeAEnviar);
		paquete_destruir(paqueteAEnviar);

}
