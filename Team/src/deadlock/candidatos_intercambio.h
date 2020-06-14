# ifndef _TAD_INTERCAMBIOS_
# define _TAD_INTERCAMBIOS_

#include "../estructuras-principales/entrenador.h"
#include "../algoritmos-de-planificacion/planificacion.h"

//TAD Candidatos de intercambio

typedef t_list* candidatos_intercambio;

typedef struct{
	entrenador*interesado;
	matriz_recursos necesidad;
	matriz_recursos sobrantes;
}candidato_intercambio;

//Constructor
candidato_intercambio* candidato_intercambio_create(entrenador*);

//Retorna true si el interesado necesita algun pokemon que al otro le sobra
bool candidato_puede_intercambiar_con(candidato_intercambio*interesado, candidato_intercambio*otro);

//Desplaza al candidato hacia la posicion del otro, tardando un segundo por paso
void candidato_desplazarse_hacia_el_otro(candidato_intercambio* self, candidato_intercambio* otro);

//Ambos intercambian pokemones, si tiene uno que le sirve al otro le da ese y sino el primero que sale
void candidato_intercambiar_con(candidato_intercambio* self, candidato_intercambio* parejaDeIntercambio);

//Destructor
void candidato_destroy(candidato_intercambio*);

//****************************************************************

//Lista
//Constructor
candidatos_intercambio candidatos_create();

//Agregar
void candidatos_agregar_entrenador(candidatos_intercambio, entrenador*);
void candidatos_agregar_candidato(candidatos_intercambio, candidato_intercambio*);

//Retorna un entrenador con el que puede intercambiar
candidato_intercambio* candidatos_pareja_de_intercambio_para(candidatos_intercambio, candidato_intercambio*);

//Elimina al entrenador de la lista de candidatos
void candidatos_descandidatear_al_entrenador(candidatos_intercambio candidatos, entrenador*unEntrenador);

//Destructor
void candidatos_destroy(candidatos_intercambio);

# endif
