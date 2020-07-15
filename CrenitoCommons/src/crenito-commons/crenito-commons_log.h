/*
 * log.h
 *
 *  Created on: 15 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_CRENITO_COMMONS_LOG_H_
#define SRC_CRENITO_COMMONS_CRENITO_COMMONS_LOG_H_

#include"utils.h"
#include "modelo/proceso.h"

void log_debug_and_destroy(t_log* un_logger, char* string);
void log_info_and_destroy(t_log* un_logger, char* string);
void log_warning_and_destroy(t_log* un_logger, char* string);

// Logs Obligatorios
//Team
void log_inicio_proceso_reconexion(t_id_cola id_cola, int segundos);  //10. Inicio de proceso de reintento de comunicación con el Broker.
void log_resultado_proceso_reconexion(t_id_cola id_cola, int un_socket); //11. Resultado de proceso de reintento de comunicación con el Broker.

//GameBoy
void log_suscripcion(t_id_cola id_cola); //2. Suscripción a una cola de mensajes.

//Broker
void log_proceso(t_log* un_logger, char* header_string, t_proceso proceso);

//Logs Adicionales
void log_event_parametro_incorrecto(char* tipo_error, char* contenido);

// Logs Errores
void log_warning_socket(int socket, char* operacion);
void log_warning_cola(int id_cola, char* funcion);


#endif /* SRC_CRENITO_COMMONS_CRENITO_COMMONS_LOG_H_ */
