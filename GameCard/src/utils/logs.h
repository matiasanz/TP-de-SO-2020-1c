# ifndef _LOGS_GAMECARD_
# define _LOGS_GAMECARD_

#include "mensajes-utils.h"

//Enunciado
void log_enunciado_posiciones_encontradas(char*especie, t_list*posiciones);
void log_enunciado_fallo_intento_localizar(t_mensaje_get_pokemon* mensajeGet);
void log_enunciado_pokemon_guardado(t_mensaje_new_pokemon* mensajeNew);
void log_enunciado_intento_fallido_de_new_pokemon(t_mensaje_new_pokemon* mensajeNew);

//Mensajes
void log_event_mensaje_appeared_enviado(t_mensaje_appeared_catch_pokemon* );
void log_event_mensaje_caught_enviado(t_mensaje_caught_pokemon* , char*especie);
void log_event_mensaje_localized_enviado(t_mensaje_localized_pokemon* );


#endif
