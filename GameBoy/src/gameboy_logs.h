# ifndef _GAMEBOY_LOGS_
# define _GAMEBOY_LOGS_

// Si se quieren comentar o agregar nuevos logs, canalizar en este archivo, no modificar en el codigo.
// Aplica para puts y printf

//Enunciado
void log_enunciado_conexion_a_proceso(char* proceso, int respuesta);
void log_enunciado_suscripcion_a_cola_de_mensajes(char* cola_mensaje_string);

//Eventos

//Estas dos sirven para las pruebas que nos dan ellos, para no tener que hacer echo
void log_event_intento_de_envio(char* proceso, char* mensaje);
void log_event_intento_de_suscripcion_a_cola(char* cola, char* tiempo_conexion);

void log_event_intento_fallido_de_conexion(char*cola);
void log_event_perdida_de_conexion_a_cola(char*cola);
void log_event_fin_de_suscripcion_a_cola(int tiempo_conexion, char* cola_mensaje_string);

# endif
