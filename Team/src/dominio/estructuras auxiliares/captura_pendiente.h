# ifndef _CAPTURA_PENDIENTE_
# define _CAPTURA_PENDIENTE_

#include "../estructuras principales/pokemon.h"
//#include <commons/collections/dictionary.h> Posible refactor

//TAD Pendiente
typedef struct Captura_pendiente{
	t_id id;
	entrenador*cazador;
	pokemon*pokemonCatcheado;
}captura_pendiente;

//	Constructor
	captura_pendiente pendiente_create(t_id, entrenador*, pokemon*);

//	Destructor
	void pendiente_destroy(captura_pendiente*);

//****************************************************************************

//TAD Pendientes
typedef cr_list* pendientes;

//	Constructor
	pendientes pendientes_create();

//	Agregar
	void agregar_pendiente(pendientes, t_id, entrenador*, pokemon*);

// 	Obtener por id pendiente
	captura_pendiente* pendientes_get(pendientes mensajesPendientes, t_id idPendiente);

// Retorna el pendiente correspondiente al id del entrenador y lo elimina de la lista
	captura_pendiente*pendientes_pendiente_del_entrenador(pendientes mensajesPendientes, t_id idEntrenador);

//	Destructor
	void pendientes_destroy(pendientes);

//	Si alguien lee esto... no digan como vivo
//	Retorna el id de una captura pendiente si encuentra y sino -1
	t_id idProximoPendienteHARDCODEADO();


#endif
