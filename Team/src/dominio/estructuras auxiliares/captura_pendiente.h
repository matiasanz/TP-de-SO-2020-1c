# ifndef _CAPTURA_PENDIENTE_
# define _CAPTURA_PENDIENTE_

#include "../estructuras principales/pokemon.h"
//#include <commons/collections/dictionary.h> Posible refactor

//TAD Pendiente
typedef struct Captura_pendiente{
	t_id id;
	entrenador*cazador;
	pokemon*pokemonCatcheado;
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
	void agregar_pendiente(pendientes, t_id, entrenador*, pokemon*);

// 	Obtener
	pendiente* pendiente_get(pendientes mensajesPendientes, t_id idRespuesta);

//	Destructor
	void pendientes_destroy(pendientes);

//	Si alguien lee esto... no digan como vivo
//	Retorna el id de una captura pendiente si encuentra y sino -1
	t_id DAME_UN_IDEEEEEEEEEEiEEEEEEEEEEEEEEEEEEEiEEEEEEEEEEEEEEEiEEEEEEEEEEEEEE();


#endif
