/*
 * paquete.h
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_PAQUETE_H_

#include <stdlib.h>
#include <string.h>

typedef struct {
	int size;
	void* stream;
} t_buffer;

t_buffer* cr_crear_buffer(int size);
void cr_destruir_buffer(t_buffer* buffer);

typedef enum {
	BROKER = 1, GAMEBOY = 2, GAMECARD = 3, TEAM = 4

} op_code;

typedef struct {
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

t_paquete* cr_crear_paquete(int cod_op, t_buffer* );
void cr_destruir_paquete(t_paquete* paquete);
void* cr_serializar_paquete(t_paquete* paquete, int *bytes);

#define SRC_CRENITO_COMMONS_PAQUETE_H_

#endif /* SRC_CRENITO_COMMONS_PAQUETE_H_ */
