# ifndef _PENDIENTE_DE_RESPUESTA_
# define _PENDIENTE_DE_RESPUESTA_

#include "pokemon.h"
#include "mensajes.h"

//TAD Pendiente
typedef struct Mensaje_pendiente{
	t_id id;
	entrenador*cazador;
	pokemon*pokemonCazado;
}pendiente;

//	Constructor
	pendiente pendiente_create(t_id, entrenador*, pokemon*);

//	Destructor
	void pendiente_destroy(pendiente*);

//****************************************************************************

//TAD Pendientes
typedef t_list* pendientes;

//	Constructor
	pendientes pendientes_create();

//	Agregar
	void agregar_pendiente(pendientes, t_id id, entrenador*, pokemon*);

// 	Obtener
	pendiente* pendiente_get(pendientes mensajesPendientes, t_id idRespuesta);

//	Destructor
	void pendientes_destroy(pendientes);

#endif
