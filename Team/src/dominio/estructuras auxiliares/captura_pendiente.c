#include "captura_pendiente.h"

//Constructor
pendiente pendiente_create(t_id idMensaje, entrenador* unEntrenador, pokemon* unPokemon){
	return (pendiente) {idMensaje, unEntrenador, unPokemon};
}

//Destructor
void pendiente_destroy(pendiente* mensajePendiente){
	free (mensajePendiente);
}


//*********************************************************************************

//Constructor
pendientes pendientes_create(){
	return cr_list_create();
}

//Agregar
void agregar_pendiente(pendientes mensajesPendientes, t_id idMensaje, entrenador* unEntrenador, pokemon* unPokemon){
	pendiente*nuevoMensaje = (pendiente*)malloc(sizeof(pendiente));
			 *nuevoMensaje = pendiente_create(idMensaje, unEntrenador, unPokemon);

    cr_list_add_and_signal(mensajesPendientes, nuevoMensaje);
}

//Obtener
pendiente* pendientes_get(pendientes mensajesPendientes, t_id idRespuesta){
	bool cmp_pendiente_id(void* unPendiente){
		return ((pendiente*)unPendiente)->id == idRespuesta;
	}

	return cr_list_get_by_condition(mensajesPendientes,  &cmp_pendiente_id);
}

//Remover segun id entrenador
pendiente*pendientes_pendiente_del_entrenador(pendientes mensajesPendientes, t_id idEntrenador){

	bool esElEntrenador(void*unPendiente){
		return ((pendiente*) unPendiente)->cazador->id == idEntrenador;
	}

	return cr_list_remove_by_condition(mensajesPendientes, &esElEntrenador);
}


//Destructor
void pendientes_destroy(pendientes pendientes){
	cr_list_destroy_and_destroy_elements(pendientes, (void(*)(void*))&pendiente_destroy);
}
