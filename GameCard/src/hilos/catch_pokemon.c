#include "mensajesGamecard.h"

typedef enum {FAIL, OK, INTENTO_INTERRUMPIDO} t_resultado_captura;
//void gamecard_Catch_Pokemon_ReIntento(t_mensaje_appeared_catch_pokemon*);
t_resultado_captura pokemon_intento_de_captura(t_pokemon);

t_resultado_captura gamecard_Catch_Pokemon_ReIntento(t_pokemon unPokemon){
	sleep(TIEMPO_REINTENTO_OPERACION);
	return pokemon_intento_de_captura(unPokemon);
}

bool intento_interrumpido(int resultado){
	return resultado == INTENTO_INTERRUMPIDO;
}

void gamecard_Catch_Pokemon(t_mensaje_appeared_catch_pokemon* mensajeCatch){

	t_pokemon unPokemon = mensajeCatch->pokemon;

	t_resultado_captura atrapado = pokemon_intento_de_captura(unPokemon);

	while(intento_interrumpido(atrapado)){
		atrapado = gamecard_Catch_Pokemon_ReIntento(unPokemon);
	}

	//---------creacion del paquete caught_pokemon y envio a Broker---------

	gamecard_enviar_caught(mensajeCatch, atrapado);

	mensaje_appeared_catch_pokemon_destruir(mensajeCatch);
}

//Revisa cada bloque del pokemon, si esta en la posicion x-y=cantidad, decrementa la cantidad y retorna OK
t_resultado_captura pokemon_intento_de_captura(t_pokemon pokemon){

	pthread_mutex_t* mutexPokemon = pokemon_get_mutex(pokemon.especie);
	pthread_mutex_lock(mutexPokemon);
	t_config* config_metadata_pokemon = pokemon_get_metadata(pokemon.especie);
	pthread_mutex_unlock(mutexPokemon);

	if(!archivo_existe(config_metadata_pokemon)){ //si no existe el archivo metadata

		//retardo para simular acceso a disco
		simular_acceso_a_disco();

		log_enunciado_pokemon_no_existe(pokemon);

		return FAIL;
	}

	log_event_pokemon_existe(pokemon);

	pthread_mutex_lock(mutexPokemon);

	//agrego este destroy y create del metadata del pokemon,
	//porque antes del mutex podrian llegar dos mensajes al mismo tiempo con
	//el valor de Open=N, pero el primero que entre lo modificara
	//y el segundo tendra un valor viejo
	config_destroy(config_metadata_pokemon);
	char* bin_metadata=pokemon_find_metadata(pokemon.especie);
	config_metadata_pokemon = config_create(bin_metadata);
	free(bin_metadata);

	if(archivo_abierto(config_metadata_pokemon)){
	//------Ver si el archivo esta abierto------------
		//abro otro hilo con un sleep que volvera a atender al Mensaje
		//en lugar de esto, el mismo hilo vuelve a intentar

		config_destroy(config_metadata_pokemon);
		pthread_mutex_unlock(mutexPokemon);

		log_enunciado_intento_interrumpido_de_catch(pokemon);

		//y finalizo este hilo
		return INTENTO_INTERRUMPIDO;
	}

	char** bloquesDelPokemon=config_get_array_value(config_metadata_pokemon,"BLOCKS");
	pthread_mutex_unlock(mutexPokemon);


	//--------comenzar a operar el pokemon-------

	char* cadenaABuscar = posicion_dar_formato(pokemon.posicion);

	t_resultado_captura atrapado;

	if(contienePosicionEnBloques(cadenaABuscar,bloquesDelPokemon)){

		//modificacion de cantidad de pokemon en posicion existente

		char* contenidoBloques=contenido_de_Bloques_con_mmap(bloquesDelPokemon);

		char** lineasDelPokemon=string_split(contenidoBloques,"\n");

		//adapto el string de posicion, para facilitarme la busqueda con el "="
		char* stringPosAdaptado = string_from_format("%s=", cadenaABuscar);

		free(cadenaABuscar);

		char* listaBloques=string_new();
				int size;

		char* contenidoActualizadoDeBloques=string_new();

		for(int i=0;lineasDelPokemon[i]!=NULL;i++){
			//este if es para verificar que no este algo distinto al formato "x-y=cant"
			if(string_contains(lineasDelPokemon[i],"-") && string_contains(lineasDelPokemon[i],"=")){
			//aqui veo si es, el que hay que modificar
				if(string_starts_with(lineasDelPokemon[i],stringPosAdaptado)){

					atrapado=OK;

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

					string_array_liberar(posYCant);

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

				log_liberacion_de_bloque(nrobloque);

				pthread_mutex_unlock(&mutBitarray);
			}

			char** arrayBloqueActualizado=string_split(nroDebloquesActualizado,",");

			int i;
			for(i=0;i<cantBloquesNecesarios;i++){

				char* bin_block = string_new();
				string_append(&bin_block,paths_estructuras[BLOCKS]);
				string_append(&bin_block,arrayBloqueActualizado[i]);
				string_append(&bin_block,".bin");


				if(cantBloquesNecesarios==(i+1)){
					//esto seria el ultimo bloque necesario

					char* ultimoAguardar=string_new();
					char* stringFinal=string_substring_from(contenidoActualizadoDeBloques,i*config_get_int_value(config_metadata,"BLOCK_SIZE"));

					string_append(&ultimoAguardar,stringFinal);
					int longitud=string_length(ultimoAguardar);
					sobrescribirLineas(bin_block,ultimoAguardar,longitud);
					free(ultimoAguardar);
					free(stringFinal);
				}else{

					char* cadenaAguardar=string_new();
					char* stringRecorte=string_substring(contenidoActualizadoDeBloques,i*config_get_int_value(config_metadata,"BLOCK_SIZE"),config_get_int_value(config_metadata,"BLOCK_SIZE"));
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


			string_array_liberar(arrayBloqueActualizado);
			free(nroDebloquesActualizado);

		}else{

			//con los bloques asignados que tiene el pokemon bastan

			int i;
			for(i=0;i<cantBloquesNecesarios;i++){

				char* bin_block = string_new();
				string_append(&bin_block,paths_estructuras[BLOCKS]);
				string_append(&bin_block,bloquesDelPokemon[i]);
				string_append(&bin_block,".bin");


				if(cantBloquesNecesarios==(i+1)){
					//esto seria el ultimo bloque necesario

					char* ultimoAguardar=string_new();

					char* stringFinal=string_substring_from(contenidoActualizadoDeBloques,i*config_get_int_value(config_metadata,"BLOCK_SIZE"));

					//primer Alternativa
					string_append(&ultimoAguardar,stringFinal);
					int longitud=string_length(ultimoAguardar);

					//segunda alternativa
					//int longitud=string_length(contenidoActualizadoDeBloques)-(i*config_get_int_value(config_metadata,"BLOCK_SIZE"));
					//string_append(&ultimoAguardar,string_substring(contenidoActualizadoDeBloques,i*config_get_int_value(config_metadata,"BLOCK_SIZE"),longitud));

					sobrescribirLineas(bin_block,ultimoAguardar,longitud);
					free(ultimoAguardar);
					free(stringFinal);
				}else{

					char* cadenaAguardar=string_new();
					char* stringRecorte=string_substring(contenidoActualizadoDeBloques,i*config_get_int_value(config_metadata,"BLOCK_SIZE"),config_get_int_value(config_metadata,"BLOCK_SIZE"));

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

		pthread_mutex_lock(mutexPokemon);

		config_set_value(config_metadata_pokemon,"BLOCKS",listaBloques);
		config_set_value(config_metadata_pokemon,"SIZE",stringSize);
		config_set_value(config_metadata_pokemon,"OPEN","N");
		config_save(config_metadata_pokemon);

		pthread_mutex_unlock(mutexPokemon);

		free(listaBloques);
		free(stringSize);

		log_enunciado_pokemon_atrapado(pokemon);

		if(size==0)
			log_archivo_pokemon_se_vacio(pokemon.especie);


	}else{
		//rama en donde la posicion recibida no se encuentra el pokemon
		atrapado=FAIL;

		//retardo para simular acceso a disco
		simular_acceso_a_disco();

		//pthread_mutex_lock(dictionary_get(semaforosDePokemons,pokemon.especie));
		pthread_mutex_lock(mutexPokemon);
		config_set_value(config_metadata_pokemon,"OPEN","N");
		config_save(config_metadata_pokemon);
		pthread_mutex_unlock(mutexPokemon);

		log_enunciado_posicion_no_encontrada(pokemon);
	}

	string_array_liberar(bloquesDelPokemon);
	config_destroy(config_metadata_pokemon);

	return atrapado;
}

