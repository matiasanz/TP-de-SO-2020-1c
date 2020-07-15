/*
 * proceso.h
 *
 *  Created on: 12 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_MODELO_PROCESO_H_
#define SRC_CRENITO_COMMONS_MODELO_PROCESO_H_

#include "../utils.h"

typedef enum {
	BROKER = 1,
	GAMEBOY = 2,
	GAMECARD = 3,
	TEAM = 4
} t_tipo_proceso;

typedef struct {
	uint32_t id_proceso; //valor que viene dado por el archivo de config.
	t_tipo_proceso tipo_proceso;
} t_proceso;

t_proceso proceso;

void proceso_inicializar(t_tipo_proceso tipo);
bool proceso_debe_informar_suscripcion();
bool proceso_debe_informar_reconexion();

uint32_t proceso_get_id(t_proceso proceso);
char* proceso_get_nombre(t_proceso proceso);
t_tipo_proceso proceso_obtener_tipo_segun_nombre(char* proceso_string);

#endif /* SRC_CRENITO_COMMONS_MODELO_PROCESO_H_ */
