/*
 * hilo_gameboy.c
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#include "hilo_gameboy.h"

void conectar_gameboy(void(*callback)(void*)) {

	conexion_gameboy = conexion_host_crear(
			config_get_string_value(config, "IP_GAMECARD"),
			config_get_string_value(config, "PUERTO_GAMECARD"),
			callback);

	pthread_create(&hilo_gameboy, NULL,
			(void*) conectar_y_escuchar_gameboy, conexion_gameboy);

	pthread_detach(hilo_gameboy);
}


