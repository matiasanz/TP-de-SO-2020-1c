#include "gameboy_logs.h"
#include "gameboy.h"

// Si se quieren comentar o agregar nuevos logs, centralizar en este archivo, no modificar en el codigo.
// Aplica para puts y printf

//**********************************   Enunciado    **************************************************

void log_enunciado_conexion_a_proceso(char* proceso, int respuesta){
	log_info(logger, "Se conecto con el proceso %s, obteniendo la respuesta: %d\n", proceso
																				  , respuesta);
}

void log_enunciado_suscripcion_a_cola_de_mensajes(char* cola_mensaje_string){
	log_info(logger, "El Proceso %s se suscribio a la cola %s del %s por %s segundos"
				, GAMEBOY_STRING
				, cola_mensaje_string
				, BROKER_STRING
				, TIEMPO_CONEXION_STRING);
}

//**********************************   Eventos     **************************************************

void log_event_fin_de_suscripcion_a_cola(int tiempo_conexion,	char* cola_mensaje_string){
	log_info(event_logger, "El tiempo de suscripción a la cola %s (%d segundos) ha finalizado\n"
			, cola_mensaje_string
			, tiempo_conexion);
}

void log_event_intento_de_envio(char* proceso, char* mensaje){
	log_info(event_logger, "************************\n"
						    "    Se intentara enviar un mensaje %s al proceso %s\n", mensaje, proceso);
}

void log_event_intento_de_suscripcion_a_cola(char* cola_mensaje_string, char* tiempo_conexion_string){
	log_info(event_logger, "************************\n"
			"    Se intentara suscribir a la cola %s del proceso %s por %s segundos\n"
			, cola_mensaje_string
			, BROKER_STRING
			, tiempo_conexion_string);
}

void log_event_error_de_conexion(char* nombre_cola){
	log_warning(event_logger,	"No se pudo conectar al proceso %s, cancelando suscripción a %s\n"
							, BROKER_STRING
							, nombre_cola);
}
