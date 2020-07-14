# include "gamecard.h"

//*********************** HEADER ****************************************************
char* pokemon_find_metadata(char*especie);
t_list*localizar_pokemon(t_mensaje_get_pokemon*, char* PATH_METADATA);
void gamecard_responder_localized(t_mensaje_get_pokemon* mensajeGet, t_list*posiciones);
void simular_acceso_a_disco();
bool archivo_existe(t_config* metadata);
bool archivo_abierto(t_config* config_metadata_pokemon);
t_list* leer_posiciones_de_disco(t_config* config_metadata_pokemon);
t_list* localizar_pokemon(t_mensaje_get_pokemon* mensajeGet, char*bin_metadata);
//***********************************************************************************

void log_enunciado_posiciones_encontradas(char*especie, t_list*posiciones){
	char* posicionesString=posicion_list_to_string(posiciones);
	pthread_mutex_lock(&mutexLogger);
	log_info(logger,"Mensaje:%s, se localizaron %u posiciones para %s,->>>: %s",GET_POKEMON_STRING,list_size(posiciones),especie,posicionesString);
	pthread_mutex_unlock(&mutexLogger);
	free(posicionesString);
}

bool acceso_fallido(t_list* posiciones){
	return !posiciones; //Si no pudo realizar la operacion retorna NULL
}

void gamecard_procesar_Get_Pokemon(t_mensaje_get_pokemon* mensajeGet){

	char*especie = mensajeGet->especie;

	char* bin_metadata = pokemon_find_metadata(especie);

	t_list* posicionesEncontradas = localizar_pokemon(mensajeGet, bin_metadata);
	free(bin_metadata);

	if(acceso_fallido(posicionesEncontradas)){

		pthread_t hiloReintentoDeOperacion;
		pthread_create(&hiloReintentoDeOperacion, NULL,(void*) gamecard_Get_Pokemon_reintento, mensajeGet);
		pthread_detach(hiloReintentoDeOperacion);

		list_destroy(posicionesEncontradas);
		return;
	}

	log_enunciado_posiciones_encontradas(especie, posicionesEncontradas);

	gamecard_responder_localized(mensajeGet, posicionesEncontradas); //creacion del paquete localized_pokemon y envio a Broker

	mensaje_get_pokemon_destruir(mensajeGet);
	list_destroy(posicionesEncontradas);
		//si uso list_destroy_and_destroy_elements(listaDePosiciones, (void*) posicion_destruir)
		//me dice en valgrind, free invalidos, puede que las posiciones
		//hayan sido libereadas en el mensaje_localized_pokemon_destruir() <-- Confirmo.
}

//************************************************************************************

//Retorna el path teorico del metadata de la especie
char* pokemon_find_metadata(char*especie){
	return string_from_format("%s%s%s", paths_estructuras[FILES], especie, "/Metadata.bin");
}

//Crea el mensaje localized con las posiciones y lo envia al broker
void gamecard_responder_localized(t_mensaje_get_pokemon* mensajeGet, t_list*posiciones){

	t_mensaje_localized_pokemon* mensajeAEnviar = mensaje_localized_pokemon_crear(mensajeGet->especie
			  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  , posiciones);

	mensaje_localized_pokemon_set_id_correlativo(mensajeAEnviar ,mensaje_get_pokemon_get_id(mensajeGet));

	t_buffer* bufferDepaquete=mensaje_localized_pokemon_serializar(mensajeAEnviar);

	t_paquete* paqueteAEnviar=paquete_crear(MENSAJE,LOCALIZED_POKEMON,bufferDepaquete);

	pthread_mutex_lock(&envioPaquete);
	int resultadoEnvio = enviar(conexion_broker,paqueteAEnviar);
	pthread_mutex_unlock(&envioPaquete);

	if(error_conexion(resultadoEnvio)){
		//repetido TODO
		log_warning(logger,"NO se pudo realizar la conexion con el BROKER");
	}

	mensaje_localized_pokemon_destruir(mensajeAEnviar);
	paquete_destruir(paqueteAEnviar);
}

void simular_acceso_a_disco(){
	sleep(tiempo_retardo_operacion);
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

//Retorna las posiciones del pokemon o NULL si no se concreto la operacion
t_list* localizar_pokemon(t_mensaje_get_pokemon* mensajeGet, char*bin_metadata){

	t_config* config_metadata_pokemon =config_create(bin_metadata);

	//Valido si no existe el archivo metadata
	if(!archivo_existe(config_metadata_pokemon)){

		//retardo para simular acceso a disco
		simular_acceso_a_disco();
		return list_create();
	}

//Si existe

	pthread_mutex_t* mutexMetadataPokemon = pokemon_get_mutex(mensajeGet->especie);

	pthread_mutex_lock(mutexMetadataPokemon);

	//------Ver si el archivo esta abierto------------
	if(archivo_abierto(config_metadata_pokemon)){

		config_destroy(config_metadata_pokemon);

//Log enunciado
		pthread_mutex_lock(&mutexLogger);
		log_error(logger,"El archivo pokemon esta abierto, la operacion GET_POKEMON %s se reintentara luego",mensajeGet->especie);
		pthread_mutex_unlock(&mutexLogger);

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

			split_liberar(posicionString);
			split_liberar(posicionYcantidad);

			free(lineasDelPokemon[i]);
		}


		free(lineasDelPokemon);
		free(contenidoBloques);
	}

	split_liberar(bloquesDelPokemon);

	return listaDePosiciones;
}
