/*
 ============================================================================
 Name        : GameBoy.c
 Author      : 
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "gameboy.h"
#include <commons/string.h>
#include <string.h>
#include <stdarg.h>//para el va_list

int main(int argc, char*argv[]) {

	const char* mensajebroker[] = { "NEW_POKEMON", "APPEARED_POKEMON",
			"CATCH_POKEMON", "CAUGHT_POKEMON", "GET_POKEMON" };
	const char* mensajegamecard[] = {"NEW_POKEMON", "CATCH_POKEMON",
			"GET_POKEMON" };
	const char* proc_o_sub[] = { "BROKER", "GAMEBOY", "GAMECARD", "TEAM",
			"SUSCRIPTOR" };
	int i, j, k;
	//va_list p;
	if (argc <= 3) {
		puts("Error!.Faltan mas argumentos..");
	}
	for (i = 0; i < 4; i++) {
		if (!strcmp(argv[1], proc_o_sub[i]))
			break;
	}

	switch (i + 1) {
	case BROKER:
		puts("Proceso broker...");
		for (j = 0; j < 5; j++) {
			if (!strcmp(argv[2], mensajebroker[j]))
				break;
		}
		switch (j+1) { //va_list para pasar varios parametros
		case NEW_POKEMON:
			//validar parametros,enviarlos....
			if (argc != 7) {
				puts("Error!.En la cantidad de argumentos ingresados ..");
				break;
			}

			//Me traigo la config para conocer la ip puerto del broker
			t_config* config = config_create("./config/gameboy.config");

			/* Abro un socket cliente y me conecto al Broker
			 int client = socket_crear_client(
			 config_get_string_value(config, "IP_BROKER"),
			 config_get_string_value(config, "PUERTO_BROKER"));*/

			/*Creo un mensaje de prueba
			 t_mensaje_new_pokemon* msj = mensaje_new_pokemon_crear("pickachu", 1, 2, 3);

			 //Empaqueto el mensaje de prueba
			 t_paquete* pqt = paquete_crear(MENSAJE,GAMEBOY,NEW_POKEMON,mensaje_new_pokemon_serializar(msj));*/

			break;
		case APPEARED_POKEMON:
			if (argc != 7) {
				puts("Error!.En la cantidad de argumentos ingresados ..");
				break;
			}
			break;
		case CATCH_POKEMON:
			if (argc != 6) {
				puts("Error!.En la cantidad de argumentos ingresados ..");
				break;
			}
			break;
		case CAUGHT_POKEMON:
			if (argc != 5) {
				puts("Error!.En la cantidad de argumentos ingresados ..");
				break;
			}
			break;
		case GET_POKEMON:
			if (argc != 4) {
				puts("Error!.En la cantidad de argumentos ingresados ..");
				break;
			}
			break;
		default:
			puts("Error el mensaje que ingreso no existe!");
			break;
		}
		break;
	case GAMEBOY:
		puts("Este proceso no es valido como argumento...");
		break;
	case GAMECARD:
		puts("Proceso gamecard...");
		for (k = 0; k < 5; k++) {
			if (!strcmp(argv[2], mensajegamecard[k]))
				break;
		}

		switch (k) {
		case NEW_POKEMON:
			if (argc != 8) {
				puts("Error!.En la cantidad de argumentos ingresados ..");
				break;
			}

			break;
		case CATCH_POKEMON:
			if (argc != 7) {
				puts("Error!.En la cantidad de argumentos ingresados ..");
				break;
			}
			break;
		case GET_POKEMON:
			if (argc != 4) {
				puts("Error!.En la cantidad de argumentos ingresados ..");
				break;
			}
			break;
		default:
			puts("Error!Mensaje incorrecto");
			break;
		}
		break;
	case TEAM:
		puts("Proceso team...");
		if (argv[2] != "APPEARED_POKEMON") {
			puts("Error el mensaje no existe");
			break;
		}
		if (argc != 6) {
			puts("Error!.En la cantidad de argumentos ingresados ..");
			break;
		}
		//.........

		break;

	case SUSCRIPTOR:
		puts("Suscribiendose...");
		break;

	default:
		puts("Error, al escribir el proceso");
		break;

	}

	return EXIT_SUCCESS;
}

