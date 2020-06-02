/*
 * conexiones.h
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_
#define SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_

#include "conexiones-utils.h"

/*
 * Funcion utilizada por los procesos GAME_BOY, GAME_CARD y TEAM para
 * enviar mensajes a un servidor dados un IP y un PUERTO.
 * La funcion crea un socket cliente, envia el paquete y luego cierra el socket.
 * En caso de error devuelve ERROR_SOCKET
 * 
 */
int enviar(t_conexion_server* server, t_paquete* pqt);

/*
 * Envia un paquete al socket que recibe por parámetro.
 * IMPORTANTE: Esta funcion debe usarse unicamente desde el BROKER
 * ya que es el único proceso que mantiene el socket abierto luego del envío.
 * Los demás procesos deben usar la funcion enviar(..) definida arriba.
 */
int enviar_paquete(t_paquete* pqt, int socket);

/*
 * Funcion utilizada por los procesos GAME_CARD y TEAM para
 * subscribirse a una cola de mensajes.
 * Luego de subscribir, se queda esuchando mensajes en dicha cola 
 * (escucha activa).
 * Al recibir un mensaje lo reenvia a través de la funcion de escucha que
 * recibe por parámeetro llamada mensaje_recibido(...)
 */
void subscribir_y_escuchar_cola(t_conexion* args);

/*
 * Funcion utilizada por los procesos GAME_CARD y TEAM para
 * conectarse con el GAME_BOY
 * Crea un socket de escucha usando un ip y puerto que recibe desde
 * el archivo de configuración.
 * Luego realiza escucha activa de mensajes.
 * Al recibir un mensaje lo reenvia a través de la funcion de escucha que
 * recibe por parámeetro llamada mensaje_recibido(...)
 */
void conectar_y_escuchar_gameboy(t_conexion_host* gameboy);

#endif /* SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_ */
