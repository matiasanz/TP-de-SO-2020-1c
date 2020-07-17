
#include "broker_log.h"

static void log_envio_mensaje(char* header_string, uint32_t id_mensaje, uint32_t id_suscriptor);
static void string_append_contenido_envio_mensaje(char** string, char* header_string, uint32_t id_mensaje, uint32_t id_suscriptor);
static void log_eliminacion_mensaje(char* header, uint32_t id_mensaje, t_id_cola id_cola);

//Logs obligatorios
void log_conexion_proceso(t_proceso proceso) {
	log_proceso(logger, LOG_HEADER_CONEXION_PROCESO, proceso);
}

void log_nuevo_suscriptor(t_proceso proceso, t_id_cola id_cola) {

	char *header_string = string_new();
	string_append_with_format(&header_string, LOG_HEADER_NUEVO_SUSCRIPTOR, get_nombre_cola(id_cola));

	log_proceso(logger, header_string, proceso);
	free(header_string);
}

void log_mensaje_recibido(void* msj_recibido, t_id_cola id_cola) {

	switch (id_cola) {
	case NEW_POKEMON:
		mensaje_new_pokemon_log(logger, msj_recibido);
		break;
	case APPEARED_POKEMON:
		mensaje_appeared_catch_pokemon_log(logger, msj_recibido, APPEARED_POKEMON_STRING);
		break;
	case CATCH_POKEMON:
		mensaje_appeared_catch_pokemon_log(logger, msj_recibido, CATCH_POKEMON_STRING);
		break;
	case CAUGHT_POKEMON:
		mensaje_caught_pokemon_log(logger, msj_recibido);
		break;
	case GET_POKEMON:
		mensaje_get_pokemon_log(logger, msj_recibido);
		break;
	case LOCALIZED_POKEMON:
		mensaje_localized_pokemon_log(logger, msj_recibido);
		break;
	default:
		log_warning_cola(id_cola, "log_mensaje_recibido");
	}
}

void log_envio_mensaje_a_suscriptor(uint32_t id_mensaje, uint32_t id_suscriptor) {
	log_envio_mensaje(LOG_HEADER_ENVIO_MSJ_SUSCRIPTOR, id_mensaje, id_suscriptor);
}

void log_confirmacion_mensaje_suscriptor(uint32_t id_mensaje, uint32_t id_suscriptor) {
	log_envio_mensaje(LOG_HEADER_CONFIRMACION_MSJ_SUSCRIPTOR, id_mensaje, id_suscriptor);
}

void log_almacenamiento_mensaje_en_memoria(t_particion* particion) {

	char *string = string_new();

	string_append_separador(&string, LOG_HEADER_MENSAJE_ALMACENADO);
	string_append_particion(&string, particion);

	log_info(logger, string);
	free(string);
}

void log_eliminacion_particion(t_particion* particion) {

	char *string = string_new();

	string_append_separador(&string, LOG_HEADER_PARTICION_ELIMINADA);
	string_append_particion(&string, particion);

	log_info(logger, string);
	free(string);
}

void log_ejecucion_compactacion() {
	log_info(logger, get_separador_string(LOG_HEADER_COMPACTACION));
}

void log_asociacion_de_bloques(void* inicio_bloque, void* inicio_buddy) {
	char *string = string_new();
	string_append_separador(&string, LOG_HEADER_ASOCIACION);
	string_append_with_format(&string, " Bloques asociados: ");
	//Debug
	//string_append_with_format(&string, " %ju con %ju \n", (uintmax_t) (inicio_bloque - memoria_principal),(uintmax_t) (inicio_buddy - memoria_principal));
	string_append_with_format(&string, " %06p con %06p \n", inicio_bloque, inicio_buddy);
	log_info_and_destroy(logger, string);
}

void log_ejecucion_dump() {
	//TODO ver codigo duplicado esto puede ser una funcion que reciba el header por parametro
	char *string = string_new();
	string_append_separador(&string, LOG_EJECUCION_DUMP);
	log_info_and_destroy(logger, string);
}
//Logs adicionales
void log_event_inicio_consolidacion_colas(t_id_cola id_cola, int cantidad_mensajes) {
	log_info(event_logger, "Inicio consolidacion cola %s, cantidad de mensajes: %d \n", get_nombre_cola(id_cola),
			cantidad_mensajes);
}

void log_event_consolidacion_cola_eliminacion(uint32_t id_mensaje, t_id_cola id_cola) {

	log_eliminacion_mensaje("Consolidacion: cola", id_mensaje, id_cola);
}

void log_event_memoria_cola_eliminacion(uint32_t id_mensaje, t_id_cola id_cola) {
	log_eliminacion_mensaje("Memoria liberacion:", id_mensaje, id_cola);
}

static void log_eliminacion_mensaje(char* header, uint32_t id_mensaje, t_id_cola id_cola) {
	log_info(event_logger, "%s %s, mensaje id eliminado: %d \n", header, get_nombre_cola(id_cola), id_mensaje);
}

void log_event_inicio_proceso() {
	log_info(event_logger, "%s iniciado exitosamente \n", BROKER_STRING);
}

void log_event_reenvio_exitoso(t_suscriptor* un_suscriptor, uint32_t id_mensaje) {
	log_info(event_logger, "reenvio exitoso de mensaje %d al suscriptor %d", id_mensaje, suscriptor_get_id_proceso(un_suscriptor));
}

//Logs Errores
void log_error_atender_cliente(int* socket, t_paquete_header header) {

	log_error(event_logger,
			"El codigo de operacion %d recibido desde el socket: %d por el proceso con id: %d (%s) es incorrecto \n",
			header.codigo_operacion, socket, proceso_get_id(header.proceso), proceso_get_nombre(header.proceso));
	free(socket);
}

void log_error_conexion_proceso() {
	log_error(event_logger, "Error al recibir el header del mensaje. Finalizando hilo \n");
}

void log_error_inicio_proceso() {
	log_error(event_logger, "Se produjo un error de conexion al iniciar el %s. Finzalizando proceso \n",
	BROKER_STRING);
}

void log_error_get_suscriptores(uint32_t id_mensaje) {
	log_error(event_logger, "Error al obtener suscriptores asociados al mensaje con id: %d\n", id_mensaje);
}

void log_warning_envio_mensaje(uint32_t id_mensaje, uint32_t id_suscriptor) {

	char *string = string_new();

	string_append_contenido_envio_mensaje(&string, LOG_HEADER_WARNING_MSJ_SUSCRIPTOR, id_mensaje, id_suscriptor);
	log_warning_and_destroy(event_logger, string);
}

void log_warning_enviar_id_mensaje(uint32_t id_mensaje, t_proceso proceso) {
	log_warning(event_logger, 
			"Se produjo un error al enviar el id de mensaje: %d al proceso %s con id_proceso: \n", id_mensaje,
			proceso_get_nombre(proceso), proceso_get_id(proceso));
}

// Funciones Privadas

static void log_envio_mensaje(char* header_string, uint32_t id_mensaje, uint32_t id_suscriptor) {

	char *string = string_new();

	string_append_contenido_envio_mensaje(&string, header_string, id_mensaje, id_suscriptor);
	log_info_and_destroy(logger, string);
}

static void string_append_contenido_envio_mensaje(char** string, char* header_string, uint32_t id_mensaje,
		uint32_t id_suscriptor) {

	string_append_separador(string, header_string);
	string_append_with_format(string, " id mensaje: %d \n", id_mensaje);
	string_append_with_format(string, " id proceso: %d \n", id_suscriptor);
}
















//*********************************Helgrind






//#include "broker_log.h"
//
//void log_conexion_proceso(t_proceso proceso) {
//}
//
//void log_nuevo_suscriptor(t_proceso proceso, t_id_cola id_cola) {
//}
//
//void log_mensaje_recibido(void* msj_recibido, t_id_cola id_cola){
//
//}
//
//void log_envio_mensaje_a_suscriptor(uint32_t id_mensaje, uint32_t id_suscriptor) {
//}
//
//void log_confirmacion_mensaje_suscriptor(uint32_t id_mensaje, uint32_t id_suscriptor) {
//}
//
//void log_almacenamiento_mensaje_en_memoria(t_particion* particion) {
//}
//
//void log_eliminacion_particion(t_particion* particion) {
//}
//
//void log_ejecucion_compactacion() {
//}
//
//void log_asociacion_de_bloques(void* inicio_bloque, void* inicio_buddy)() {
//
//}
//void log_event_inicio_consolidacion_colas(t_id_cola id_cola, int cantidad_mensajes) {
//}
//
//void log_ejecucion_dump() {
//}
//void log_event_consolidacion_cola_eliminacion(uint32_t id_mensaje, t_id_cola id_cola) {
//}
//
//void log_event_memoria_cola_eliminacion(uint32_t id_mensaje, t_id_cola id_cola) {
//}
//void log_event_inicio_proceso() {
//}
//
//void log_event_reenvio_exitoso(t_suscriptor* un_suscriptor, uint32_t id_mensaje) {
//}
//
//void log_error_atender_cliente(int* socket, t_paquete_header header) {
//}
//
//void log_error_conexion_proceso() {
//}
//
//void log_error_inicio_proceso() {
//}
//
//void log_error_get_suscriptores(uint32_t id_mensaje) {
//}
//
//void log_warning_envio_mensaje(uint32_t id_mensaje, uint32_t id_suscriptor) {
//}
//
//void log_warning_enviar_id_mensaje(uint32_t id_mensaje, t_proceso proceso) {
//}
