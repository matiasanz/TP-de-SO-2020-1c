/*
 * conexiones-utils.h
 *
 *  Created on: 31 may. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_UTILS_H_
#define SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_UTILS_H_

//Mensajes
#include "../mensajes/mensaje_new_pokemon.h"
#include "../mensajes/mensaje_localized_pokemon.h"
#include "../mensajes/mensaje_get_pokemon.h"
#include "../mensajes/mensaje_appeared_catch_pokemon.h"
#include "../mensajes/mensaje_caught_pokemon.h"

#include "paquete.h"
#include "socket.h"
#include "../modelo/suscriptor.h"
#include "../modelo/proceso.h"
#include "../crenito-commons_log.h"

/* 
 * Contiene la información necesaria para poder enviarle mensajes 
 * a un proceso servidor.
 * Ejemplo: cuando el TEAM o GAME_CARD nececistan enviarle información al BROKER.
 */
typedef struct {
	char* ip;
	char* puerto;
} t_conexion_server;

/* 
 * Contiene la información necesaria para que un proceso actúe como cliente 
 * Ejemplo: cuando el TEAM o GAME_CARD nececistan subscribirse a una cola
 * de mensajes y realizar escucha activa.
 */
typedef struct {
	t_id_cola id_cola; //cola a la que el cliente se subscribe
	int segundos_reconexion;
	void (*callback)(t_id_cola, void*); //funcion a ejecutar cuando se recibe un mensaje (funcion escucha)
} t_conexion_cliente;

/* 
 * Contiene la información necesaria para que un proceso actúe como host 
 * Ejemplo: cuando el TEAM o GAME_CARD nececsitan escuchar mensajes del GAME_BOY
 */
typedef struct {
	char* ip;
	char* puerto;
	void (*callback)(t_id_cola, void*); //funcion a ejecutar cuando se recibe un mensaje (funcion escucha)
} t_conexion_host;

typedef struct {
	t_conexion_cliente* cliente;
	t_conexion_server* server;
} t_conexion;

t_conexion_server* conexion_broker;
t_conexion_host* conexion_gameboy;

pthread_mutex_t mutex_subscripcion;

//Funciones auxiliares utilizadas por conexiones.h
t_conexion_host* conexion_host_crear(char* ip, char* puerto, void (*callback)(t_id_cola, void*));

t_conexion_server* conexion_server_crear(char* ip, char* puerto);
void conexion_server_destruir(t_conexion_server* server);

t_conexion_cliente* conexion_cliente_crear(t_id_cola id_cola, int segundos_reconexion, void (*callback)(t_id_cola, void*));
void conexion_cliente_destruir(t_conexion_cliente* cliente);

t_conexion* conexion_crear(t_conexion_server* server, t_conexion_cliente* cliente);
void conexion_destruir(t_conexion* conexion);

t_buffer* serializar(void* msj, t_id_cola id_cola);
void* deserializar(void* msj, t_id_cola id_cola);

bool debe_reconectar(t_conexion_cliente* cliente);

#endif /* SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_UTILS_H_ */
