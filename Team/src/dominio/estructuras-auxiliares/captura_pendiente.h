# ifndef _CAPTURA_PENDIENTE_
# define _CAPTURA_PENDIENTE_

#include "../estructuras-principales/pokemon.h"
//#include <commons/collections/dictionary.h> Posible refactor

//TAD Pendiente
typedef struct Captura_pendiente{
	t_id id;
	entrenador*cazador;
	pokemon*pokemonCatcheado;
	bool respondido;
}captura_pendiente;

//	Constructor
	captura_pendiente pendiente_create(t_id, entrenador*, pokemon*);

//	Destructor
	void pendiente_destroy(captura_pendiente*);

//****************************************************************************

//TAD Pendientes
typedef cr_list* capturas_pendientes;

//	Constructor
	capturas_pendientes pendientes_create();

//	Agregar
	void agregar_pendiente(capturas_pendientes, t_id, entrenador*, pokemon*);

// 	Obtener por id pendiente
	captura_pendiente* pendientes_remove_by_id(capturas_pendientes mensajesPendientes, t_id idPendiente);

// Retorna el pendiente correspondiente al id del entrenador y lo elimina de la lista
	captura_pendiente*pendientes_pendiente_del_entrenador(capturas_pendientes mensajesPendientes, t_id idEntrenador);

// TODO
	bool pendientes_reconocer_id(capturas_pendientes pendientes, t_id idCaptura);

//	Destructor
	void pendientes_destroy(capturas_pendientes);


#endif
