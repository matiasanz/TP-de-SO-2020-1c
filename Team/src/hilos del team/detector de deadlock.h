# ifndef _HILO_DETECTOR_DEADLOCK_
# define _HILO_DETECTOR_DEADLOCK_

#include "hilos_team.h"


bool algoritmo_detectar_deadlock(entrenadores candidatos, entrenadores enDeadlock);

void algoritmo_procesar_deadlock(entrenadores entrenadoresEnDeadlock);

bool hay_deadlock_exceptuando_al_siguiente(entrenadores unEquipo, matriz_recursos necesidad, matriz_recursos disponibles, entrenadores omitidos, entrenadores enDeadlock);

bool hay_deadlock_dejando_al_siguiente(entrenadores unEquipo, matriz_recursos necesidad, matriz_recursos disponibles, entrenadores omitidos, entrenadores enDeadlock);

bool hay_espera_circular(entrenadores unEquipo, matriz_recursos necesidad,matriz_recursos disponibles, entrenadores enDeadlock);

matriz_recursos entrenador_recursos_sobrantes(entrenador*);

matriz_recursos entrenador_recursos_pedidos(entrenador*);

matriz_recursos entrenadores_pedidos_insatisfechos(entrenadores unEquipo);

matriz_recursos entrenadores_recursos_sobrantes(entrenadores unEquipo);

#endif
