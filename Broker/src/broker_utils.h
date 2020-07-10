/*
 * broker_utils.h
 *
 *  Created on: 16 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_BROKER_UTILS_H_
#define SRC_BROKER_UTILS_H_

#include <crenito-commons/conexiones/conexiones.h>

#define LOG_HEADER_MENSAJE_ALMACENADO "MENSAJE ALMACENADO"
#define LOG_HEADER_PARTICION_ELIMINADA "PARTICION ELIMINADA"
#define LOG_HEADER_COMPACTACION "ALGORITMO DE COMPACTACIÓN EJECUTADO"
#define LOG_HEADER_CONEXION_PROCESO "NUEVA CONEXION CON PROCESO"
#define LOG_HEADER_NUEVO_SUSCRIPTOR "NUEVO SUSCRIPTOR COLA %s"
#define LOG_HEADER_ENVIO_MSJ_SUSCRIPTOR "MENSAJE ENVIADO"
#define LOG_HEADER_CONFIRMACION_MSJ_SUSCRIPTOR "ACK RECIBIDO EXITOSAMENTE"
#define LOG_HEADER_WARNING_MSJ_SUSCRIPTOR "ERROR AL RECIBIR ACK"

pthread_mutex_t mutex_id_univoco;
uint32_t id_univoco;
uint32_t generar_id_univoco();

long get_fecha_en_microsegundos(struct timeval fecha);

// Logs obligatorios
void log_conexion_proceso(t_paquete_header header); // 1. Conexión de un proceso al broker.
void log_nuevo_suscriptor(t_paquete_header header); // 2. Suscripción de un proceso a una cola de mensajes.
void log_mensaje_recibido(t_id_cola id_cola, void* msj); // 3.Llegada de un nuevo mensaje a una cola de mensajes.
void log_envio_mensaje_a_suscriptor(uint32_t id_mensaje, uint32_t id_suscriptor); // 4. Envío de un mensaje a un suscriptor específico.
void log_confirmacion_mensaje_suscriptor(uint32_t id_mensaje, uint32_t id_suscriptor); // 5. Confirmación de recepción de un suscripción al envío de un mensaje previo.
void log_ejecucion_compactacion(); // 8. Ejecución de compactación (para particiones dinámicas)

// Logs adicionales
void log_inicio_consolidacion_colas(char* nombre_cola, int cantidad_mensajes);
void log_consolidacion_cola_eliminacion(uint32_t id_mensaje, t_id_cola id_cola);
void log_inicio_proceso();

// Logs Errores
void log_error_atender_cliente(int socket, t_paquete_header header);
void log_error_conexion_proceso();
void log_error_inicio_proceso();
void log_error_cola(int id_cola);
void log_error_get_suscriptores(uint32_t id_mensaje);
void log_warning_envio_mensaje(uint32_t id_mensaje, uint32_t id_suscriptor);

#endif /* SRC_BROKER_UTILS_H_ */
