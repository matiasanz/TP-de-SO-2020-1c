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

//Contiene el Id de proceso subscriptor y los datos de la conexion
typedef struct {
	t_id_proceso id;
	t_conexion conexion;
} t_subscriptor;

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
	pthread_mutex_t locker;
} t_cola_container;

//Las 6 colas de mensajes
t_cola_container* cola_new_pokemon;
t_cola_container* cola_appeared_pokemon;
t_cola_container* cola_catch_pokemon;
t_cola_container* cola_caught_pokemon;
t_cola_container* cola_get_pokemon;
t_cola_container* cola_localized_pokemon;

void serve_client(t_conexion* conexion);
void devolver_mensaje(int cod_op, t_buffer* buffer, int socket_cliente);
void handover(int cod_op, int socket_cliente);

// Punto de entrada para cuando se detecta que un proceso se quiere subscribir a una cola
void procesar_subscripcion(t_conexion* conexion, char* nombre_proceso);
// Realiza la subscripcion propiamente dicha
int subscribir_proceso_a_cola(t_conexion* conexion, t_tipo_cola_mensaje cola_a_subscribir);

// Punto de entrada para cuando se detecta que un proceso envia un mensaje
void procesar_mensaje(t_conexion* conexion, char* nombre_proceso);

//Funciones auxiliares:
char* get_nombre_cola_mensaje(t_tipo_cola_mensaje tipo_cola_mensaje);
t_cola_container* inicializar_cola_container();
t_cola_container* get_cola_container(t_tipo_cola_mensaje cola_a_subscribir);
void log_estado_subscripcion(int status, char* nombre_proceso, int id_cola_container);

#endif /* UTILIDADESBROKER_H_ */
