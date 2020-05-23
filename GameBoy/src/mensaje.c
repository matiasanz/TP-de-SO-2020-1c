#include "gameboy.h"
#include <crenito-commons/utils.h>
#include "mensaje.h"
#include "validaciones_gameboy.h"



t_paquete* crearMensaje(char* argumentos[],int longitud){

	t_buffer* mensaje_serializado;
	t_tipo_cola_mensaje Tipomensaje;
	char* especie;
	int i,cantidad,posx,posy;
	const char* tiposDeMensajes[] = { NEW_POKEMON_STRING, APPEARED_POKEMON_STRING,
				CATCH_POKEMON_STRING, CAUGHT_POKEMON_STRING, GET_POKEMON_STRING };

	for (i = 0; i < 4; i++) {
		if (!strcmp(argumentos[2], tiposDeMensajes[i]))
			break;
	}

	switch(i+1){

	case NEW_POKEMON:

		Tipomensaje= NEW_POKEMON;
		validar_quien_conoce_newpokemon(argumentos[1]);
		if(strcmp(argumentos[1],BROKER_STRING)==0){

			validar_cantidad_argumentos(longitud,7);
			//la especie no hace falta ser validada, ya que no puede recibir un espacio en blanco
			especie=argumentos[3];
			validar_mayor_igual_a_cero(&posx,argumentos[4]);
			validar_mayor_igual_a_cero(&posy,argumentos[5]);
			validar_mayor_a_cero(&cantidad,argumentos[6]);
			t_mensaje_new_pokemon* msj = mensaje_new_pokemon_crear(especie, posx, posy, cantidad);
			mensaje_serializado=mensaje_new_pokemon_serializar(msj);
		}else{
			//el mensaje serializado del new_pokemon del gamecard
		}



		break;

	case APPEARED_POKEMON:
		Tipomensaje= APPEARED_POKEMON;
		break;
	case CATCH_POKEMON:
		Tipomensaje= CATCH_POKEMON;
		break;
	case CAUGHT_POKEMON:
		Tipomensaje= CAUGHT_POKEMON;
		break;
	case GET_POKEMON:
		Tipomensaje= GET_POKEMON;
		break;

	}

	return paquete_crear(MENSAJE,GAMEBOY,Tipomensaje,mensaje_serializado);

}
