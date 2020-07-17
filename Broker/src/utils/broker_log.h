/*
 * broker_log.h
 *
 *  Created on: 11 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_UTILS_BROKER_LOG_H_
#define SRC_UTILS_BROKER_LOG_H_

#include <crenito-commons/conexiones/conexiones.h>
#include "../memoria/memoria_modelo/particion.h"

#define LOG_HEADER_MENSAJE_ALMACENADO "MENSAJE ALMACENADO"
#define LOG_HEADER_PARTICION_ELIMINADA "PARTICION ELIMINADA"
#define LOG_HEADER_COMPACTACION "ALGORITMO DE COMPACTACIÓN EJECUTADO"
#define LOG_HEADER_CONEXION_PROCESO "NUEVA CONEXION CON PROCESO"
#define LOG_HEADER_NUEVO_SUSCRIPTOR "NUEVO SUSCRIPTOR COLA %s"
#define LOG_HEADER_ENVIO_MSJ_SUSCRIPTOR "MENSAJE ENVIADO"
#define LOG_HEADER_CONFIRMACION_MSJ_SUSCRIPTOR "ACK RECIBIDO EXITOSAMENTE"
#define LOG_HEADER_WARNING_MSJ_SUSCRIPTOR "ERROR AL RECIBIR ACK"
#define LOG_HEADER_ASOCIACION "ALGORITMO DE ASOCIACIÓN DE BLOQUES EJECUTADO"

// Logs obligatorios
void log_conexion_proceso(t_proceso proceso); // 1. Conexión de un proceso al broker.
void log_nuevo_suscriptor(t_proceso proceso, t_id_cola id_cola); // 2. Suscripción de un proceso a una cola de mensajes.
void log_mensaje_recibido(void* msj_recibido, t_id_cola id_cola); // 3.Llegada de un nuevo mensaje a una cola de mensajes.
void log_envio_mensaje_a_suscriptor(uint32_t id_mensaje, uint32_t id_suscriptor); // 4. Envío de un mensaje a un suscriptor específico.
void log_confirmacion_mensaje_suscriptor(uint32_t id_mensaje, uint32_t id_suscriptor); // 5. Confirmación de recepción de un suscripción al envío de un mensaje previo.
void log_almacenamiento_mensaje_en_memoria(t_particion* particion); // 6. Almacenado de un mensaje dentro de la memoria (indicando posición de inicio de su partición).
void log_eliminacion_particion(t_particion* particion); // 7. Eliminado de una partición de memoria (indicado la posición de inicio de la misma).
void log_ejecucion_compactacion(); // 8_a. Ejecución de compactación (para particiones dinámicas)
void log_asociacion_de_bloques(void* inicio_bloque, void* inicio_buddy); //8_b asociación de bloques (para buddy system).
// Logs adicionales
void log_event_inicio_consolidacion_colas(t_id_cola id_cola, int cantidad_mensajes);
void log_event_consolidacion_cola_eliminacion(uint32_t id_mensaje, t_id_cola id_cola);
void log_event_memoria_cola_eliminacion(uint32_t id_mensaje, t_id_cola id_cola);
void log_event_inicio_proceso();
void log_event_reenvio_exitoso(t_suscriptor* un_suscriptor, uint32_t id_mensaje);

// Logs Errores
void log_error_atender_cliente(int* socket, t_paquete_header header);
void log_error_conexion_proceso();
void log_error_inicio_proceso();
void log_error_get_suscriptores(uint32_t id_mensaje);
void log_warning_envio_mensaje(uint32_t id_mensaje, uint32_t id_suscriptor);
void log_warning_enviar_id_mensaje(uint32_t id_mensaje, t_proceso proceso);

#endif /* SRC_UTILS_BROKER_LOG_H_ */
