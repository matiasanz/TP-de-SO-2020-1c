/*
 * conexiones.c
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#include "conexiones.h"

int subscripcion_cola(t_id_proceso id_proceso, t_tipo_cola_mensaje nombre_cola,
		char* ip, char* puerto) {

	t_paquete_header* pqt = paquete_subscripcion_crear(id_proceso, nombre_cola);
	int socket_ciente = socket_crear_client(ip, puerto);

	//Inicio handshake
	socket_send(socket_ciente, pqt, sizeof(t_paquete_header));
	int status = socket_recibir_int(socket_ciente);
	//Fin handshake

	socket_cerrar(socket_ciente);
	paquete_subscripcion_destruir(pqt);

	return status;
}

int subscripcion_exitosa(int estado) {
	return estado == SUBSCRIPCION_EXITO;
}
