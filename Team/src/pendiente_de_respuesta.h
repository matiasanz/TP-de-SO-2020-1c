# ifndef _PENDIENTE_DE_RESPUESTA_
# define _PENDIENTE_DE_RESPUESTA_

#include "pokemon.h"
#include "mensajes.h"

//TAD
typedef t_list* mensajes_pendientes;

//Constructor
mensajes_pendientes mensajes_pendientes_create();

//Agregar
void agregar_mensaje_pendiente(mensajes_pendientes, t_id id, entrenador*, pokemon*);


#endif
