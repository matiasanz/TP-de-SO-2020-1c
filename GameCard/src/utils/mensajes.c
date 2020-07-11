#include "mensajes-utils.h"

//Crea el mensaje localized con las posiciones y lo envia al broker
void gamecard_enviar_localized(t_mensaje_get_pokemon* mensajeGet, t_list*posiciones){

	t_mensaje_localized_pokemon* mensajeAEnviar = mensaje_localized_pokemon_crear(mensajeGet->especie
			  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  , posiciones);

	mensaje_localized_pokemon_set_id_correlativo(mensajeAEnviar ,mensaje_get_pokemon_get_id(mensajeGet));

	t_paquete_header header = paquete_header_crear(MENSAJE,GAMECARD,LOCALIZED_POKEMON, id_proceso);

	t_buffer* bufferDepaquete=mensaje_localized_pokemon_serializar(mensajeAEnviar);

	t_paquete* paqueteAEnviar=paquete_crear(header,bufferDepaquete);

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

void gamecard_enviar_appeared(t_mensaje_new_pokemon* mensajeNew){
	t_mensaje_appeared_catch_pokemon* mensajeAEnviar=mensaje_appeared_catch_pokemon_crear(mensajeNew->pokemon.especie,mensajeNew->pokemon.posicion.pos_x,mensajeNew->pokemon.posicion.pos_y);
	mensaje_appeared_catch_pokemon_set_id_correlativo(mensajeAEnviar,mensaje_new_pokemon_get_id(mensajeNew));

	t_paquete_header header=paquete_header_crear(MENSAJE,GAMECARD,APPEARED_POKEMON, id_proceso);
	t_buffer* bufferDepaquete=mensaje_appeared_catch_pokemon_serializar(mensajeAEnviar);
	t_paquete* paqueteAEnviar=paquete_crear(header,bufferDepaquete);

	pthread_mutex_lock(&envioPaquete);
	int resultadoEnvio = enviar(conexion_broker, paqueteAEnviar);
	pthread_mutex_unlock(&envioPaquete);

	if(error_conexion(resultadoEnvio)){
		log_warning(logger,"NO se puede realizar la conexion con el BROKER");
	}

	mensaje_appeared_catch_pokemon_destruir(mensajeAEnviar);
	paquete_destruir(paqueteAEnviar);
}
