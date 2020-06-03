/*
 * test_conexiones.c
 *
 *  Created on: 3 jun. 2020
 *      Author: utnso
 */

#include "../src/crenito-commons/conexiones/conexiones.h"
#include "test_utils.h"

context (test_conexiones) {

	t_id_cola cola_esperada;
	t_paquete* pqt_test;
	t_mensaje_new_pokemon* mensaje_new_pokemon_esperado;
	t_conexion_server* server;

	t_paquete* crear_paquete_test() {
		mensaje_new_pokemon_esperado = mensaje_new_pokemon_crear("pickachu", 4,
				5, 6);
		mensaje_new_pokemon_set_id(mensaje_new_pokemon_esperado, 5);
		mensaje_new_pokemon_set_id_correlativo(mensaje_new_pokemon_esperado, 1);

		return paquete_crear(
				paquete_header_crear(MENSAJE, cola_esperada, NEW_POKEMON),
				mensaje_new_pokemon_serializar(mensaje_new_pokemon_esperado));
	}

	void assert_mensaje_recibido(t_id_cola id_cola_real, void* serializado_real) {

		should_int(id_cola_real) be equal to (pqt_test ->header.id_cola);

		t_mensaje_new_pokemon* msj_real = mensaje_new_pokemon_deserializar(
				serializado_real);

		assert_mensaje_new_pokemon(mensaje_new_pokemon_esperado, msj_real);

		free(serializado_real);
		mensaje_new_pokemon_destruir(msj_real);
	}

	describe("tests de conexiones") {

		before {

			inicializar_logs();
			server = conexion_server_crear("127.0.0.1", "3999", cola_esperada);
			pqt_test = crear_paquete_test();

		}end

		it("Un cliente envia un paquete y un servidor lo recibe") {

			//ARRANGE
			cola_esperada = GAMEBOY;
			//Socket server que escucha mensajes
			int socket_server = socket_crear_listener(server -> ip, server -> puerto);
			//Conexion al socket_server desde un cliente
			int socket_enviar = socket_crear_client(server -> ip, server -> puerto);
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
			should_int(0) be equal to (respuesta_recibir);

			//Free
			conexion_server_destruir(server);
			mensaje_new_pokemon_destruir(mensaje_new_pokemon_esperado);
			paquete_destruir(pqt_test);

			//Cierre de sockets
			socket_cerrar(socket_server);
			socket_cerrar(socket_enviar);
			socket_cerrar(socket_cliente);

		}end

		it("Si el BROKER está caido la función enviar devuelve el error esperado") {

			//ACTION
			// Enviamos el paquete desde el cliente
			int respuesta_enviar = enviar(server, pqt_test);

			should_bool(error_conexion(respuesta_enviar)) be truthy;
			//Free
			mensaje_new_pokemon_destruir(mensaje_new_pokemon_esperado);
			paquete_destruir(pqt_test);
		}end

		after {
			log_destroy(logger);
			log_destroy(event_logger);
		}end

	}end
}
