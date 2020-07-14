#include "../../dominio/estructuras-auxiliares/candidatos_intercambio.h"

#include "../../utils/team_logs.h"

//******************************************************

candidato_intercambio* candidato_intercambio_create(entrenador*unEntrenador){
	candidato_intercambio* elemento = malloc(sizeof(candidato_intercambio));
				  *elemento = (candidato_intercambio) {unEntrenador, entrenador_recursos_pedidos(unEntrenador), entrenador_recursos_sobrantes(unEntrenador)};
	return elemento;
}

void candidato_destroy(candidato_intercambio* elemento){
	recursos_destroy(elemento->necesidad);
	recursos_destroy(elemento->sobrantes);
	free(elemento);
}

bool candidato_puede_intercambiar_con(candidato_intercambio*unEntrenador, candidato_intercambio*otro){
	return recursos_alguno_en_comun_con(unEntrenador->necesidad, otro->sobrantes);
}

recurso candidato_recurso_que_le_entrega_a(candidato_intercambio* self, candidato_intercambio* aQuien){
	recurso unRecurso = recursos_alguno_en_comun_con(self->sobrantes, aQuien->necesidad);

	return unRecurso? unRecurso : recursos_cualquier_recurso(self->sobrantes);
}

void recursos_intercambiar_instancia_de(matriz_recursos recursos, recurso deQuien, recurso porQuien){
	recursos_quitar_instancia_de_recurso(recursos, deQuien);
	recursos_agregar_recurso(recursos, porQuien);
}

void candidato_actualizar_matrices_por_intercambio(candidato_intercambio* unCandidato, recurso cualDio, recurso cualRecibio){
	recursos_quitar_instancia_de_recurso(unCandidato->sobrantes, cualDio);

	bool loEstafaron = !recursos_cantidad_de_instancias_de(unCandidato->necesidad, cualRecibio);

	recursos_agregar_recurso((loEstafaron? unCandidato->sobrantes: unCandidato->necesidad), cualRecibio);
}

void candidato_intercambiar_con(candidato_intercambio* self, candidato_intercambio* parejaDeIntercambio){

	recurso teDoyEste = candidato_recurso_que_le_entrega_a(self, parejaDeIntercambio);
	recurso aCambioDeEste = candidato_recurso_que_le_entrega_a(parejaDeIntercambio, self);

	recursos_intercambiar_instancia_de(self->interesado->pokemonesCazados, teDoyEste, aCambioDeEste);
	recursos_intercambiar_instancia_de(parejaDeIntercambio->interesado->pokemonesCazados, aCambioDeEste, teDoyEste);

	log_enunciado_operacion_de_intercambio(self->interesado, parejaDeIntercambio->interesado, teDoyEste, aCambioDeEste);

	candidato_actualizar_matrices_por_intercambio(self, teDoyEste, aCambioDeEste);
	candidato_actualizar_matrices_por_intercambio(parejaDeIntercambio, aCambioDeEste, teDoyEste);
}

//****************************************************************************************

candidatos_intercambio candidatos_create(){
	return list_create();
}

void candidatos_agregar_entrenador(candidatos_intercambio candidatos, entrenador*unEntrenador){
	candidato_intercambio* candidato = candidato_intercambio_create(unEntrenador);
	candidatos_agregar_candidato(candidatos, candidato);
}

void candidatos_agregar_candidato(candidatos_intercambio candidatos, candidato_intercambio* candidato){
	list_add(candidatos, candidato);
}

void candidatos_descandidatear_al_entrenador(candidatos_intercambio candidatos, entrenador*unEntrenador){

	bool esEse(void*candidatoDeLista){
		return ((candidato_intercambio*)candidatoDeLista)->interesado->id == unEntrenador->id;
	}

	candidato_intercambio* unCandidato = list_remove_by_condition(candidatos, &esEse);

	candidato_destroy(unCandidato);
}

candidato_intercambio* candidatos_pareja_de_intercambio_para(candidatos_intercambio unEquipo, candidato_intercambio*unEntrenador){
	bool esParejaDeIntercambio(void*otro){
		return candidato_puede_intercambiar_con(unEntrenador, otro);
	}
	return list_remove_by_condition(unEquipo, &esParejaDeIntercambio);
}

void candidatos_destroy(candidatos_intercambio candidatos){
	list_destroy_and_destroy_elements(candidatos, (void(*)(void*))&candidato_destroy);
}
