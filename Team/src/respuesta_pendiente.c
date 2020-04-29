#include "respuesta_pendiente.h"

//Constructor
pendiente pendiente_create(t_id idMensaje, entrenador* unEntrenador, pokemon* unPokemon){
	return (pendiente) {idMensaje, unEntrenador, unPokemon};
}

//Destructor
void pendiente_destroy(pendiente* mensajePendiente){
	pokemon_destroy(mensajePendiente->pokemonCazado); //Duda
	free (mensajePendiente);
}


//*********************************************************************************

//Constructor
pendientes pendientes_create(){
	return list_create();
}

//Agregar
void agregar_pendiente(pendientes mensajesPendientes, t_id idMensaje, entrenador* unEntrenador, pokemon* unPokemon){
	pendiente*nuevoMensaje = malloc(sizeof(pendiente));
			 *nuevoMensaje = pendiente_create(idMensaje, unEntrenador, unPokemon);
	list_add(mensajesPendientes, nuevoMensaje);
}

//Obtener
pendiente* pendiente_get(pendientes mensajesPendientes, t_id idRespuesta){
	bool cmp_pendiente_id(pendiente* unPendiente, t_id id){
		return unPendiente->id == id;
	}

	pendiente*mensaje = list_find(mensajesPendientes, (bool(*)(void*)) &cmp_pendiente_id);

	if(!mensaje){
		error_show("El id no corresponde a ningun mensaje pendiente");
		exit(1);
	}

	return mensaje;
}

//Destructor
void pendientes_destroy(pendientes pendientes){
	list_destroy_and_destroy_elements(pendientes, (void(*)(void*))&pendiente_destroy);
}
