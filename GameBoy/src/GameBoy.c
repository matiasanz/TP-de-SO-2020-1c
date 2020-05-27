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

	conexion_broker = conexion_server_crear(
				config_get_string_value(config, "IP_BROKER"),
				config_get_string_value(config, "PUERTO_BROKER"), GAMEBOY,
				0);

		// Creo un mensaje de prueba
	t_mensaje_new_pokemon* msj = mensaje_new_pokemon_crear("pickachu", 1, 2, 3);

	//Empaqueto el mensaje de prueba
	t_buffer* bfr = mensaje_new_pokemon_serializar(msj);
	t_paquete* pqt = paquete_crear(MENSAJE,
				                   GAMEBOY,
								   NEW_POKEMON,
								   bfr);

	//envio el mensaje
	int respuesta = enviar(conexion_broker, pqt);

	printf("Id obtenido del Broker: %d \n", respuesta);

	config_destroy(config);
	mensaje_new_pokemon_destruir(msj);
	paquete_destruir(pqt);

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
