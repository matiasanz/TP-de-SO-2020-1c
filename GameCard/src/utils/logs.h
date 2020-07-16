# ifndef _LOGS_GAMECARD_
# define _LOGS_GAMECARD_

#include "mensajes-utils.h"

//*******************   Enunciado   **************************

//New -> Appeared
void log_enunciado_intento_interrumpido_de_new_pokemon(t_mensaje_new_pokemon* mensajeNew);
void log_enunciado_pokemon_guardado(t_mensaje_new_pokemon*);

//Get -> Localized
void log_enunciado_intento_interrumpido_de_get(t_mensaje_get_pokemon*);
void log_enunciado_posiciones_encontradas(char*especie, t_list*posiciones);

//Catch -> Caught
void log_enunciado_intento_interrumpido_de_catch(t_pokemon);
void log_enunciado_pokemon_atrapado(t_pokemon pokemon);
void log_enunciado_posicion_no_encontrada(t_pokemon pokemon);
void log_enunciado_pokemon_no_existe(t_pokemon);

//cambios en estructuras, foro issue 1838
void log_asignacion_bloque_nuevo(int nroBloque,char* especie);
void log_liberacion_de_bloque(int nroBloque);
void log_archivo_pokemon_se_vacio(char* especie);

//*******************    Eventos    **************************

//Mensajes
void log_event_mensaje_appeared_enviado(t_mensaje_appeared_catch_pokemon* );
void log_event_mensaje_caught_enviado(t_mensaje_caught_pokemon* , char*especie);
void log_event_mensaje_localized_enviado(t_mensaje_localized_pokemon* );
void log_event_pokemon_existe(t_pokemon);

#endif
