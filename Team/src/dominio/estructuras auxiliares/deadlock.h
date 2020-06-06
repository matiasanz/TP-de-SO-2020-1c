#include "../estructuras principales/entrenador.h"

//TAD Candidatos de intercambio

typedef t_list* candidatos_deadlock;

typedef struct{
	entrenador*unEntrenador;
	matriz_recursos necesidad;
	matriz_recursos sobrantes;
}candidato_deadlock;

//Constructor
candidato_deadlock* candidato_deadlock_create(entrenador*);

//Destructor
void candidato_deadlock_destroy(candidato_deadlock*);

//Lista
//Constructor
candidatos_deadlock candidatos_create();

//Agregar
void candidatos_agregar_entrenador(candidatos_deadlock, entrenador*);
void candidatos_agregar_candidato(candidatos_deadlock, candidato_deadlock*);

//Destructor
void candidatos_destroy(candidatos_deadlock);

//***************************************************************
//TAD Par intercambio

typedef struct{
	candidato_deadlock*unEntrenador;
	candidato_deadlock*parejaDeIntercambio;
} par_intercambio;

par_intercambio* par_intercambio_create();


//Pares intercambio
typedef t_list* pares_intercambio;

pares_intercambio pares_deadlock_create();
