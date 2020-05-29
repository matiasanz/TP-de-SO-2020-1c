/*
 * conexiones.h
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_
#define SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_

#include "paquete.h"
#include "socket.h"

//Mensajes
#include "../mensajes/mensaje_new_pokemon.h"
#include "../mensajes/mensaje_localized_pokemon.h"
#include "../mensajes/mensaje_get_pokemon.h"
#include "../mensajes/mensaje_appeared_catch_pokemon.h"
#include "../mensajes/mensaje_caught_pokemon.h"

typedef struct {
	char* ip;
	char* puerto;
	t_id_proceso id_proceso;
	int segundos_reconexion;
} t_conexion_server;

typedef struct {
	uint32_t id_subcriptor;
	uint32_t socket;
} t_conexion_cliente;

//Funciones de comunicacion entre procesos
t_conexion_cliente subscribir_cola(t_conexion_server server, t_id_cola id_cola);
int enviar(t_conexion_server server, t_paquete* pqt);
void* recibir(t_conexion_cliente cliente);

//Funciones de creacion de conexiones
t_conexion_server conexion_server_crear(char* ip, char* puerto, t_id_proceso id_proceso, int segundos_reconexion);
t_conexion_cliente* conexion_cliente_crear(int socket, int id_subscriptor);

#endif /* SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_ */
