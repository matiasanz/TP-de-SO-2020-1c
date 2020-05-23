#include "validaciones_gameboy.h"
#include <crenito-commons/utils.h>
#include "gameboy.h"
#include <stdlib.h>

void validar_proceso(char* proceso){

	if(strcmp(proceso,BROKER_STRING) && strcmp(proceso,TEAM_STRING) && strcmp(proceso,GAMECARD_STRING)&& strcmp(proceso,"SUSCRIPTOR")){
		log_error(event_logger,"el id proceso es incorrecto, el id recibido es %s: ",proceso);
		abort();
	}


}

void validar_mensaje(char* mensaje){
	if(strcmp(mensaje,NEW_POKEMON_STRING) && strcmp(mensaje,APPEARED_POKEMON_STRING) && strcmp(mensaje,CATCH_POKEMON_STRING) && strcmp(mensaje,CAUGHT_POKEMON_STRING) && strcmp(mensaje,GET_POKEMON_STRING)){
		log_error(event_logger,"el mensaje no existe, el mensaje recibido es %s: ",mensaje);
		abort();
	}

}

void obtener_ip_puerto(char* proceso,char **ip,char **puerto){

	t_config* config = config_create("./config/gameboy.config");
	if(strcmp(proceso,BROKER_STRING)==0){
		*ip=config_get_string_value(config, "IP_BROKER");
		*puerto=config_get_string_value(config, "PUERTO_BROKER");

	}
	if(strcmp(proceso,TEAM_STRING)==0){
		*ip=config_get_string_value(config, "IP_TEAM");
		*puerto=config_get_string_value(config, "PUERTO_TEAM");

	}
	if(strcmp(proceso,GAMECARD_STRING)==0){
		*ip=config_get_string_value(config, "IP_GAMECARD");
		*puerto=config_get_string_value(config, "PUERTO_GAMECARD");

	}

}

void validar_cantidad_argumentos(int cantidad,int numero){
	if(cantidad!=numero){
		log_error(event_logger,"la cantidad de argumentos recibidos es incorrecto, la cantidad que ingreso es %i: ",cantidad);
		abort();
	}
}




void validar_quien_conoce_newpokemon(char* proceso){
	if(strcmp(proceso,BROKER_STRING) && strcmp(proceso,GAMECARD_STRING)){
		log_error(event_logger,"este mensaje no lo entiende el  proceso %s",proceso);
		abort();
	}
}
void validar_mayor_a_cero(int* cantidad,char* num){
	*cantidad=atoi(num);
	puts(cantidad);
	if(cantidad<=0){
		log_error(event_logger,"la cantidad no puede ser menor o igual a cero, el numero que ingreso es %i",cantidad);
		abort();
	}
}
void validar_mayor_igual_a_cero(int* pos,char* num){
	*pos=atoi(num);
	if(pos<0){
		log_error(event_logger,"la cantidad no puede ser menor  a cero, el numero que ingreso es %i",pos);
		abort();
	}

}

