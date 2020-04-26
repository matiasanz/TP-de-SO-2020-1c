#include"utilidades-broker.h"

void serve_client(int* socket_cliente) {

	int cod_op = socket_recibir_cod_operacion(*socket_cliente);

	if (cod_op < 0)
		pthread_exit(NULL);

	process_request(cod_op, *socket_cliente);
}

void process_request(int cod_op, int socket_cliente) {

	switch (cod_op) {
	case GAMEBOY:
		log_info(logger, "mensaje recibido por: GAMEBOY");
		handover(GAMEBOY, socket_cliente);
		break;
	case GAMECARD:
		log_info(logger, "mensaje recibido por: GAMECARD");
		handover(GAMECARD, socket_cliente);
		break;
	case TEAM:
		log_info(logger, "mensaje recibido por: TEAM");
		handover(TEAM, socket_cliente);
		break;
	default:
		log_error(logger,
				"El codigo de operacion %d recibido desde el socket: %d es incorrecto",
				cod_op, socket_cliente);
		pthread_exit(NULL);
	}
}

void handover(int cod_op, int socket_cliente) {
	int size;
	char * msg;

	msg = socket_recibir_mensaje(socket_cliente, &size);
	log_info(logger, "contenido del mensaje: %s ", msg);

	t_buffer* buffer = cr_crear_buffer(size);
	memcpy(buffer->stream, msg, size);

	devolver_mensaje(cod_op, buffer, socket_cliente);
	log_info(logger, "mensaje devuelto con exito!");
	free(msg);
}

void devolver_mensaje(int cod_op, t_buffer* buffer, int socket_cliente) {
	t_paquete* paquete = cr_crear_paquete(cod_op, buffer);

	int bytes = 0;
	void* a_enviar = cr_serializar_paquete(paquete, &bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}
