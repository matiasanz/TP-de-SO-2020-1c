/*
 * hilo_gameboy.c
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#include "../hilos_comunicacion/hilo_gameboy.h"

void conectar_gameboy(void (*callback)(t_id_cola, void*)) {

	conexion_gameboy = conexion_host_crear(
			config_get_string_value(config, "IP_TEAM"),
			config_get_string_value(config, "PUERTO_TEAM"),
			callback);

	pthread_create(&hilo_gameboy, NULL,
			(void*) conectar_y_escuchar_gameboy, conexion_gameboy);

	pthread_detach(hilo_gameboy);
}


