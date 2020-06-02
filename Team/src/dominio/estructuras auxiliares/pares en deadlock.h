#include "../estructuras principales/entrenador.h"

//TAD Par intercambio

typedef struct{
	t_id idEntrenador;
	entrenador*parejaDeIntercambio;
} par_intercambio;

par_intercambio* par_intercambio_create();


//Pares intercambio
typedef t_list* pares_intercambio;

pares_intercambio pares_deadlock_create();
