/*
 * test_conexiones.c
 *
 *  Created on: 3 jun. 2020
 *      Author: utnso
 */

#include "test_utils.h"

context (test_conexiones) {

	t_id_cola cola_esperada;
	t_paquete* pqt_test;
	void* mensaje_esperado;

	describe("tests de conexiones") {

		before {

			inicializar_logs();
			conexion_broker = conexion_server_crear("127.0.0.1", "3999", GAMEBOY);

		}end

		after {

			log_destroy(logger);
			log_destroy(event_logger);

		}end

		describe ("Envío y recepción de mensajes") {

			before {

				cola_esperada = NEW_POKEMON;
				mensaje_esperado = mensaje_new_pokemon_test();
				pqt_test = paquete_new_pokemon_test();

			}end

			after {

				mensaje_new_pokemon_destruir(mensaje_esperado);
				conexion_server_destruir(conexion_broker);
				paquete_destruir(pqt_test);

			}end

			it("Un cliente envia un paquete y un servidor lo recibe") {

				//ARRANGE
				//Socket server que escucha mensajes
				int socket_server = socket_crear_listener(conexion_broker -> ip, conexion_broker -> puerto);
				//Conexion al socket_server desde un cliente
				int socket_enviar = socket_crear_client(conexion_broker -> ip, conexion_broker -> puerto);
				//El server acepta la conexion y genera un socket con la referencia del cliente
				int socket_cliente = socket_aceptar_conexion(socket_server);
				/*simulamos que el envio que vamos a realizar va a tener este ACK
				 para evitar que el test se quede clavado durante el handshake*/
				int ack = 1;
				socket_send(socket_cliente, &ack, sizeof(int));

				//ACTION
				// Enviamos el paquete desde el cliente
				int respuesta_enviar = enviar_paquete(pqt_test, socket_enviar);
				//recibimos el paquete desde la referencia del cliente que tiene el server
				int respuesta_recibir = recibir(socket_cliente, (void*) assert_mensaje_recibido);

				should_int(1) be equal to (respuesta_enviar);
				should_bool(conexion_exitosa(respuesta_recibir)) be truthy;

				//Cierre de sockets
				socket_cerrar(socket_server);
				socket_cerrar(socket_enviar);
				socket_cerrar(socket_cliente);
			}end

			it("Si el BROKER está caido la función enviar devuelve el error esperado") {

				//ACTION
				// Enviamos el paquete desde el cliente
				int respuesta_enviar = enviar(conexion_broker, pqt_test);

				should_bool(error_conexion(respuesta_enviar)) be truthy;

			}end
		}end

		describe ("Subscripciones") {

			int socket_server;
			int socket_cliente;

			before {

				socket_server = socket_crear_listener(conexion_broker -> ip, conexion_broker -> puerto);
			}end

			after {

				socket_cerrar(socket_server);
				socket_cerrar(socket_cliente);
				paquete_destruir(pqt_test);
			}end

			it("El GAME_CARD se subscribe a la cola CATCH_POKEMON y recibe un mensaje") {

				pqt_test = paquete_appeared_catch_pokemon_test(CATCH_POKEMON);
				//ARRANGE
				cola_esperada = CATCH_POKEMON;

				conexion_catch_pokemon = conexion_cliente_crear(cola_esperada, 1,
						0,
						(void*)assert_mensaje_recibido_thread);

				t_conexion* args = conexion_crear(conexion_broker,
						conexion_catch_pokemon);
				//ACTION:
				//Subscribimos al proceso
				pthread_create(&hilo_catch_pokemon, NULL,
						(void*) suscribir_y_escuchar_cola, args);

				pthread_detach(hilo_catch_pokemon);
				/*simulamos que el envio que vamos a realizar va a tener este ACK
				 para evitar que el test se quede clavado durante el handshake*/
				socket_cliente = socket_aceptar_conexion(socket_server);
				int ack = 1;
				socket_send(socket_cliente, &ack, sizeof(int));

				int respuesta_enviar = enviar_paquete(pqt_test, socket_cliente);

				should_bool(conexion_exitosa(respuesta_enviar)) be truthy;

			}end

			it("El GAME_CARD se subscribe a la cola GET_POKEMON y recibe un mensaje") {

				pqt_test = paquete_get_pokemon_test();
				//ARRANGE
				cola_esperada = GET_POKEMON;

				conexion_get_pokemon = conexion_cliente_crear(cola_esperada, 1,
						0,
						(void*)assert_mensaje_recibido_thread);

				t_conexion* args = conexion_crear(conexion_broker,
						conexion_get_pokemon);

				//ACTION:
				//Subscribimos al proceso
				pthread_create(&hilo_get_pokemon, NULL,
						(void*) suscribir_y_escuchar_cola, args);

				pthread_detach(hilo_get_pokemon);
				/*simulamos que el envio que vamos a realizar va a tener este ACK
				 para evitar que el test se quede clavado durante el handshake*/
				socket_cliente = socket_aceptar_conexion(socket_server);
				int ack = 1;
				socket_send(socket_cliente, &ack, sizeof(int));

				int respuesta_enviar = enviar_paquete(pqt_test, socket_cliente);

				should_bool(conexion_exitosa(respuesta_enviar)) be truthy;

			}end

			it("El GAME_CARD se subscribe a la cola NEW_POKEMON y recibe un mensaje") {

				pqt_test = paquete_new_pokemon_test();
				//ARRANGE
				cola_esperada = NEW_POKEMON;

				conexion_new_pokemon = conexion_cliente_crear(cola_esperada,1,
						0,
						(void*)assert_mensaje_recibido_thread);

				t_conexion* args = conexion_crear(conexion_broker,
						conexion_new_pokemon);

				//ACTION:
				//Subscribimos al proceso
				pthread_create(&hilo_new_pokemon, NULL,
						(void*) suscribir_y_escuchar_cola, args);

				pthread_detach(hilo_new_pokemon);
				/*simulamos que el envio que vamos a realizar va a tener este ACK
				 para evitar que el test se quede clavado durante el handshake*/

				socket_cliente = socket_aceptar_conexion(socket_server);
				int ack = 1;
				socket_send(socket_cliente, &ack, sizeof(int));

				int respuesta_enviar = enviar_paquete(pqt_test, socket_cliente);

				should_bool(conexion_exitosa(respuesta_enviar)) be truthy;

			}end

			it("El TEAM se subscribe a la cola APPEARED_POKEMON y recibe un mensaje") {

				pqt_test = paquete_appeared_catch_pokemon_test(APPEARED_POKEMON);
				//ARRANGE
				cola_esperada = APPEARED_POKEMON;

				conexion_appeared_pokemon = conexion_cliente_crear(cola_esperada,1,
						0,
						(void*)assert_mensaje_recibido_thread);

				t_conexion* args = conexion_crear(conexion_broker,
						conexion_appeared_pokemon);

				//ACTION:
				//Subscribimos al proceso
				pthread_create(&hilo_appeared_pokemon, NULL,
						(void*) suscribir_y_escuchar_cola, args);

				pthread_detach(hilo_appeared_pokemon);
				/*simulamos que el envio que vamos a realizar va a tener este ACK
				 para evitar que el test se quede clavado durante el handshake*/
				socket_cliente = socket_aceptar_conexion(socket_server);
				int ack = 1;
				socket_send(socket_cliente, &ack, sizeof(int));

				int respuesta_enviar = enviar_paquete(pqt_test, socket_cliente);

				should_bool(conexion_exitosa(respuesta_enviar)) be truthy;

			}end

			it("El TEAM se subscribe a la cola LOCALIZED_POKEMON y recibe un mensaje") {

				pqt_test = paquete_localized_pokemon_test();
				//ARRANGE
				cola_esperada = LOCALIZED_POKEMON;

				conexion_localized_pokemon = conexion_cliente_crear(cola_esperada,1,
						0,
						(void*)assert_mensaje_recibido_thread);

				t_conexion* args = conexion_crear(conexion_broker,
						conexion_localized_pokemon);

				//ACTION:
				//Subscribimos al proceso
				pthread_create(&hilo_localized_pokemon, NULL,
						(void*) suscribir_y_escuchar_cola, args);

				pthread_detach(hilo_localized_pokemon);
				/*simulamos que el envio que vamos a realizar va a tener este ACK
				 para evitar que el test se quede clavado durante el handshake*/
				socket_cliente = socket_aceptar_conexion(socket_server);
				int ack = 1;
				socket_send(socket_cliente, &ack, sizeof(int));

				int respuesta_enviar = enviar_paquete(pqt_test, socket_cliente);

				should_bool(conexion_exitosa(respuesta_enviar)) be truthy;

			}end

			it("El TEAM se subscribe a la cola CAUGHT y recibe un mensaje") {

				pqt_test = paquete_caught_pokemon_test();
				//ARRANGE
				cola_esperada = CAUGHT_POKEMON;

				conexion_caught_pokemon = conexion_cliente_crear(cola_esperada,1,
						0,
						(void*)assert_mensaje_recibido_thread);

				t_conexion* args = conexion_crear(conexion_broker,
						conexion_caught_pokemon);

				//ACTION:
				//Subscribimos al proceso
				pthread_create(&hilo_caught_pokemon, NULL,
						(void*) suscribir_y_escuchar_cola, args);

				pthread_detach(hilo_caught_pokemon);
				/*simulamos que el envio que vamos a realizar va a tener este ACK
				 para evitar que el test se quede clavado durante el handshake*/
				socket_cliente = socket_aceptar_conexion(socket_server);
				int ack = 1;
				socket_send(socket_cliente, &ack, sizeof(int));

				int respuesta_enviar = enviar_paquete(pqt_test, socket_cliente);

				should_bool(conexion_exitosa(respuesta_enviar)) be truthy;

			}end

		}end

		describe ("Conexion con GAME_BOY") {

			it("El GAME_CARD y TEAM rciben mensajes del GAME_BOY") {

				//ARRANGE
				conexion_gameboy = conexion_host_crear("127.0.0.1", "3997", (void*)assert_mensaje_recibido_thread);

				pqt_test = paquete_localized_pokemon_test(LOCALIZED_POKEMON);

				cola_esperada = LOCALIZED_POKEMON;

				pthread_create(&hilo_gameboy, NULL,
						(void*) conectar_y_escuchar_gameboy, conexion_gameboy);

				pthread_detach(hilo_gameboy);

				sleep(1);
				//Conexion al socket_server desde un cliente
				int socket_enviar = socket_crear_client(conexion_gameboy -> ip, conexion_gameboy -> puerto);

				//ACTION
				int respuesta_enviar = enviar_paquete(pqt_test, socket_enviar);

				//ASSERT
				should_bool(conexion_exitosa(respuesta_enviar)) be truthy;

				socket_cerrar(socket_enviar);
				paquete_destruir(pqt_test);

			}end
		}end

	}end
}
