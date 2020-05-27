#ifndef UTILIDADESBROKER_H_
#define UTILIDADESBROKER_H_

#include<commons/log.h>
#include<pthread.h>
#include <crenito-commons/conexiones/conexiones.h>
#include <commons/collections/queue.h>
#include <commons/collections/list.h>

pthread_t thread;

//Logger para uso exclusivo de informacion pedida por el tp
t_log* logger;

//Logger para grabar informacion adicional/errores
t_log* event_logger;

//Estructura con la información solicitada por el tp en la parte "Administración de Mensajes"
typedef struct {
	int id;
	int id_correlativo;
//	int tipo; --> ver si es necesario
	t_list* subscriptores_enviados;
	t_list* subscriptores_confirmados;
} t_mensaje_metadata;

// Estructura con el contenido a poner en la cola
typedef struct {
	void* contenido; //t_mensaje_new_pokemon_mensaje, t_mensaje_appeared_pokemon, etc.
	t_mensaje_metadata* metadata;
} t_mensaje;

// TAD cola
typedef struct {
	t_queue* cola;
	t_list* subscriptores;
	pthread_mutex_t mutex;
} t_cola_container;

//Las 6 colas de mensajes
t_cola_container* cola_new_pokemon;
t_cola_container* cola_appeared_pokemon;
t_cola_container* cola_catch_pokemon;
t_cola_container* cola_caught_pokemon;
t_cola_container* cola_get_pokemon;
t_cola_container* cola_localized_pokemon;

// Punto de entrada para cuando se detecta que un proceso se quiere subscribir a una cola
void procesar_subscripcion(int socket, t_paquete_header header);
// Punto de entrada para cuando se detecta que un proceso envia un mensaje
void procesar_mensaje(int socket, t_paquete_header header);
t_cola_container* cola_crear();

#endif /* UTILIDADESBROKER_H_ */
