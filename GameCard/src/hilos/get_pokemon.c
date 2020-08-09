#include "mensajesGamecard.h"

void gamecard_procesar_Get_Pokemon(t_mensaje_get_pokemon* mensajeGet){

	char*especie = mensajeGet->especie;

	t_list* posicionesEncontradas = localizar_pokemon(especie);

	while(acceso_fallido(posicionesEncontradas)){
		log_enunciado_intento_interrumpido_de_get(mensajeGet);
		posicionesEncontradas = reintentar_localizar_pokemon(especie);
	}

	log_enunciado_posiciones_encontradas(especie, posicionesEncontradas);

	gamecard_enviar_localized(mensajeGet, posicionesEncontradas); //creacion del paquete localized_pokemon y envio a Broker

	mensaje_get_pokemon_destruir(mensajeGet);

	list_destroy(posicionesEncontradas);
}

//************************************************************************************

bool acceso_fallido(t_list* posiciones){
	return !posiciones;
}

t_list* reintentar_localizar_pokemon(char* especie){
	sleep(TIEMPO_REINTENTO_OPERACION);
	return localizar_pokemon(especie);
}

void simular_acceso_a_disco(){
	sleep(TIEMPO_RETARDO_OPERACION);
}

//retorna el mutex correspondiente a la especie ingresada. Si no lo encuentra, lo crea y lo agrega
pthread_mutex_t* pokemon_get_mutex(char* especie){

	pthread_mutex_lock(&mutDiccionarioSemaforos);
	pthread_mutex_t* mutex = dictionary_get(semaforosDePokemons, especie);

	if(!mutex){

		mutex = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(mutex, NULL);
		dictionary_put(semaforosDePokemons, especie, mutex);
	}

	pthread_mutex_unlock(&mutDiccionarioSemaforos);

	return mutex;
}

//retorna true si el config no es null. Si fuera null, significaria que no se encontro el archivo
bool archivo_existe(t_config* metadata){
	return metadata;
}

//retorna true si el atributo OPEN del archivo es Y(yes). En ese caso, no se lo podria acceder y se debera reintentar
bool archivo_abierto(t_config* config_metadata_pokemon){

	char* estadoArchivo=config_get_string_value(config_metadata_pokemon,"OPEN");

	if(strcmp(estadoArchivo,"N")==0){
		config_set_value(config_metadata_pokemon,"OPEN","Y");
		config_save(config_metadata_pokemon);

		return false;
	}

	return true;
}

//Tira excepcion en caso de que se encuentre una linea que no coincide con el formato
//Entiendo que esto no deberia ocurrir, pero dudo si es responsabilidad nuestra
void validar_linea(char*lineaDelPokemon){
	//este if es para verificar que no este algo distinto al formato "x-y=cant"
	if(!string_contains(lineaDelPokemon,"-") || !string_contains(lineaDelPokemon,"=")){
		log_error(event_logger, "Se leyo una absurda de un metadata (Capaz flashee)");
		exit(1);
	}
}


t_config* pokemon_get_metadata(char* especie){

	char* bin_metadata=pokemon_find_metadata(especie);

//	pthread_mutex_t* mutexPokemon = pokemon_get_mutex(especie);
//	pthread_mutex_lock(mutexPokemon);
	t_config* config_metadata_pokemon = config_create(bin_metadata);
//	pthread_mutex_unlock(mutexPokemon);

	free(bin_metadata);

	return config_metadata_pokemon;
}

//Retorna las posiciones del pokemon o NULL si no se concreto la operacion
t_list* localizar_pokemon(char* especie){

	t_config* config_metadata_pokemon = pokemon_get_metadata(especie);

	//Valido si no existe el archivo metadata
	if(!archivo_existe(config_metadata_pokemon)){

		//retardo para simular acceso a disco
		simular_acceso_a_disco();
		return list_create();
	}

//Si existe

	pthread_mutex_t* mutexMetadataPokemon = pokemon_get_mutex(especie);

	pthread_mutex_lock(mutexMetadataPokemon);


	//agrego este destroy y create del metadata del pokemon,
	//porque talvez este hilo esperaba en el mutex y puede que algun catch
	//termino antes y hay menos bloques ocupados por el pokemon,
	//tendriamos informacion desactualizada en el config_metadata_pokemon
	config_destroy(config_metadata_pokemon);
	config_metadata_pokemon = pokemon_get_metadata(especie);

	//------Ver si el archivo esta abierto------------
	if(archivo_abierto(config_metadata_pokemon)){
		pthread_mutex_unlock(mutexMetadataPokemon);
		config_destroy(config_metadata_pokemon);

		//y finalizo este hilo
		return NULL;
	}

	//--------comenzar a operar el pokemon-------

	t_list* listaDePosiciones = leer_posiciones_de_disco(config_metadata_pokemon);

	config_set_value(config_metadata_pokemon,"OPEN","N");
	config_save(config_metadata_pokemon);
	pthread_mutex_unlock(mutexMetadataPokemon);

	config_destroy(config_metadata_pokemon);

	return listaDePosiciones;
}

//Lee el metadata y obtiene las posiciones
t_list* leer_posiciones_de_disco(t_config* config_metadata_pokemon){

	simular_acceso_a_disco();

	t_list* listaDePosiciones = list_create();

	char** bloquesDelPokemon=config_get_array_value(config_metadata_pokemon,"BLOCKS");

	//Si hay elementos
	if(bloquesDelPokemon!=NULL){

		char* contenidoBloques = contenido_de_Bloques_con_mmap(bloquesDelPokemon);

		char** lineasDelPokemon=string_split(contenidoBloques,"\n");

		for(int i=0; lineasDelPokemon[i]!=NULL ; i++){

			validar_linea(lineasDelPokemon[i]);

			char** posicionYcantidad=string_split(lineasDelPokemon[i],"=");

			char** posicionString=string_split(posicionYcantidad[0],"-");

			int posX=atoi(posicionString[0]);

			int posY=atoi(posicionString[1]);

			t_posicion* unaPosicion=posicion_crear_ptr(posX,posY);

			list_add(listaDePosiciones,unaPosicion);

			string_array_liberar(posicionString);
			string_array_liberar(posicionYcantidad);

			free(lineasDelPokemon[i]);
		}


		free(lineasDelPokemon);
		free(contenidoBloques);
	}

	string_array_liberar(bloquesDelPokemon);

	return listaDePosiciones;
}
