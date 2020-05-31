/*
 * conexiones.h
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_
#define SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_

#include "conexiones-utils.h"

/*Funcion utilizada por los procesos GAME_BOY, GAME_CARD y GAME_BOY para
 * enviar mensajes a un servidor dados un IP y un PUERTO.
 * La funcion crea un socket cliente, envia el paquete y luego cierra el socket.
 * Esta funcion debe usarse desde GameBoyTeam, GameCard y Team,
 */
int enviar(t_conexion_server* server, t_paquete* pqt);

/*
 * Envia un paquete al socket que recibe por parámetro.
 * IMPORTANTE: Esta funcion debe usarse unicamente desde Broker que es el único
 * proceso que mantiene el socket abierto luego del envío.
 * Los demás procesos deben usar la funcion enviar(..) definida arriba.
 */
int enviar_paquete(t_paquete* pqt, int socket);

/*
 * Funcion utilizada por los procesos GAME_BOY, GAME_CARD y GAME_BOY para
 * subscribirse a una cola de mensajes.
 * Luego de subscribir al proceso, se queda esuchando mensajes en dicha cola 
 * (escucha activa).
 * Al recibir un mensaje lo reenvia a travpés de la funcion de escucha que 
 * recibe por parámeetro.
 * La nomenclatura de las funciones de escucha es [tipo_mensaje]_recibido
 * Ejemplo: new_pokemon_recibido(...).
 * Los procesos que usen estsa funcion deben definir la función de escucha.
 */
void subscribir_y_escuchar_cola(t_conexion* args);

#endif /* SRC_CRENITO_COMMONS_CONEXIONES_CONEXIONES_H_ */
