# ifndef _GAMEBOY_LOGS_
# define _GAMEBOY_LOGS_

//Enunciado
void log_enunciado_conexion_a_proceso(char* proceso, int respuesta);
void log_enunciado_suscripcion_a_cola_de_mensajes(char* cola_mensaje_string);

//Eventos
void log_event_fin_de_suscripcion_a_cola(int tiempo_conexion, char* cola_mensaje_string);
void log_event_intento_de_envio(char* proceso, char* mensaje);
void log_event_intento_de_suscripcion_a_cola(char* cola, char* tiempo_conexion);
void log_event_error_de_conexion(char*cola);

# endif
