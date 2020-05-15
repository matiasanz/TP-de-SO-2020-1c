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
	return list_create();
}

t_id ME_TENGO_QUE_ACORDAR_DE_BORRAR_ESTO=1;

//Agregar
void agregar_pendiente(pendientes mensajesPendientes, t_id idMensaje, entrenador* unEntrenador, pokemon* unPokemon){
	pendiente*nuevoMensaje = (pendiente*)malloc(sizeof(pendiente));
			 *nuevoMensaje = pendiente_create(idMensaje, unEntrenador, unPokemon);
	list_add(mensajesPendientes, nuevoMensaje);

	ME_TENGO_QUE_ACORDAR_DE_BORRAR_ESTO=idMensaje;
}

//Obtener
pendiente* pendiente_get(pendientes mensajesPendientes, t_id idRespuesta){
	bool cmp_pendiente_id(pendiente* unPendiente){
		return unPendiente->id == idRespuesta;
	}

	pendiente*mensaje = list_remove_by_condition(mensajesPendientes, (bool(*)(void*)) &cmp_pendiente_id);

//	if(!mensaje){
//		error_show("El id %d no corresponde a ningun mensaje pendiente", idRespuesta);
//		exit(1);
//	}

	//TODO BORRAR
	ME_TENGO_QUE_ACORDAR_DE_BORRAR_ESTO = !list_is_empty(mensajesPendientes)? ((pendiente*)list_get(mensajesPendientes, 0))->id: -1;

	return mensaje;
}

t_id DAME_UN_IDEEEEEEEEEEiEEEEEEEEEEEEEEEEEEEiEEEEEEEEEEEEEEEiEEEEEEEEEEEEEE(){
	return ME_TENGO_QUE_ACORDAR_DE_BORRAR_ESTO;
}

//Destructor
void pendientes_destroy(pendientes pendientes){
	list_destroy_and_destroy_elements(pendientes, (void(*)(void*))&pendiente_destroy);
}
