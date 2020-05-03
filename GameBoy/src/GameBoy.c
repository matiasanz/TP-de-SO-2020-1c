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

int main(void) {

	//Me traigo la config para conocer la ip puerto del broker
	t_config* config = config_create("./config/gameboy.config");

	// Abro un socket cliente y me conecto al Broker
	int client = socket_crear_client(
			config_get_string_value(config, "IP_BROKER"),
			config_get_string_value(config, "PUERTO_BROKER"));

	// Creo un mensaje de prueba
	t_mensaje_new_pokemon* msj = mensaje_new_pokemon_crear("pickachu", 1, 2, 3);

	//Empaqueto el mensaje de prueba
	t_paquete* pqt = paquete_crear(MENSAJE,
				                   GAMEBOY,
								   NEW_POKEMON,
								   mensaje_new_pokemon_serializar(msj));
	// serializo el paquete
	int bytes;
	void* a_enviar = paquete_serializar(pqt, &bytes);

	//Envio el paquete, utilizo la variable bytes para indicar el tamaño
	socket_send(client, a_enviar, bytes);

	// Recibo el id del mensaje
	int respuesta = socket_recibir_int(client);

	printf("Id obtenido del Broker: %d", respuesta);

	return 1;

}


//void paquete_enviar(int cod_op, t_buffer* buffer, int socket_cliente) {
//	t_paquete* paquete = paquete_crear(cod_op, BROKER, buffer);
//
//	int bytes = 0;
//
//
//	send(socket_cliente, a_enviar, bytes, 0);
//
//	free(a_enviar);
//}
