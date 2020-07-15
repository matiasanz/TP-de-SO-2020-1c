/*
 * proceso.c
 *
 *  Created on: 12 jul. 2020
 *      Author: utnso
 */

#include "proceso.h"

void proceso_inicializar(t_tipo_proceso tipo) {

	proceso.id_proceso = config_get_int_value(config, "ID_PROCESO");
	proceso.tipo_proceso = tipo;
}

bool proceso_debe_informar_suscripcion() {
	return proceso.tipo_proceso == GAMEBOY;
}

bool proceso_debe_informar_reconexion() {
	return proceso.tipo_proceso == TEAM;
}

uint32_t proceso_get_id(t_proceso proceso) {
	return proceso.id_proceso;
}

char* proceso_get_nombre(t_proceso proceso) {

	switch (proceso.tipo_proceso) {
	case BROKER:
		return BROKER_STRING;
	case GAMEBOY:
		return GAMEBOY_STRING;
	case GAMECARD:
		return GAMECARD_STRING;
	case TEAM:
		return TEAM_STRING;
	default:
		log_error(event_logger, "Tipo de proceso incorrecto: %d", proceso.tipo_proceso);
		return NULL;
	}
}

