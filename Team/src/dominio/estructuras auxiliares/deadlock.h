#include "../estructuras principales/entrenador.h"

//TAD Candidatos de intercambio

typedef t_list* candidatos_intercambio;

typedef struct{
	entrenador*unEntrenador;
	matriz_recursos necesidad;
	matriz_recursos sobrantes;
}candidato_intercambio;

//Constructor
candidato_intercambio* candidato_intercambio_create(entrenador*);

//Destructor
void candidato_intercambio_destroy(candidato_intercambio*);

//TAD Par intercambio

typedef struct{
	t_id idEntrenador;
	entrenador*parejaDeIntercambio;
} par_intercambio;

par_intercambio* par_intercambio_create();


//Pares intercambio
typedef t_list* pares_intercambio;

pares_intercambio pares_deadlock_create();
