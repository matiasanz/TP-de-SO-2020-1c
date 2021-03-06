#include "../estructuras-auxiliares/captura_pendiente.h"

//Constructor
captura_pendiente pendiente_create(t_id idMensaje, entrenador* unEntrenador, pokemon* unPokemon){
	return (captura_pendiente) {idMensaje, unEntrenador, unPokemon, false};
}

//Destructor
void pendiente_destroy(captura_pendiente* mensajePendiente){
	free(mensajePendiente);
}


//*********************************************************************************

//Constructor
capturas_pendientes pendientes_create(){
	return cr_list_create();
}

//Agregar
void agregar_pendiente(capturas_pendientes mensajesPendientes, t_id idMensaje, entrenador* unEntrenador, pokemon* unPokemon){
	captura_pendiente*nuevoMensaje = (captura_pendiente*)malloc(sizeof(captura_pendiente));
			 *nuevoMensaje = pendiente_create(idMensaje, unEntrenador, unPokemon);

    cr_list_add_and_signal(mensajesPendientes, nuevoMensaje);
}

bool pendiente_cmp(captura_pendiente* unaCaptura, captura_pendiente* otra){
	return unaCaptura->id == otra->id;
}

//Obtener
captura_pendiente* pendientes_remove_by_id(capturas_pendientes mensajesPendientes, t_id idRespuesta){
	bool cmp_pendiente_id(void* unPendiente){
		return ((captura_pendiente*)unPendiente)->id == idRespuesta && !((captura_pendiente*)unPendiente)->respondido;
	}

	captura_pendiente*esePendiente = cr_list_silent_remove_by_condition(mensajesPendientes,  &cmp_pendiente_id);

	if(esePendiente) esePendiente->respondido = true;

	return esePendiente;
}

//Remover segun id entrenador
captura_pendiente*pendientes_pendiente_del_entrenador(capturas_pendientes mensajesPendientes, t_id idEntrenador){

	bool esElEntrenador(void*unPendiente){
		return ((captura_pendiente*) unPendiente)->cazador->id == idEntrenador;
	}

	return cr_list_silent_remove_by_condition(mensajesPendientes, &esElEntrenador);
}


//Destructor
void pendientes_destroy(capturas_pendientes pendientes){
	cr_list_destroy_and_destroy_elements(pendientes, (void(*)(void*))&pendiente_destroy);
}


bool pendiente_coincide_id(t_id* id, captura_pendiente*captura){
	return captura->id==*id;
}

bool pendientes_reconocer_id(capturas_pendientes pendientes, t_id idCaptura){
	return cr_list_contains_element(pendientes, &idCaptura, (bool(*)(void*, void*))pendiente_coincide_id);
}
