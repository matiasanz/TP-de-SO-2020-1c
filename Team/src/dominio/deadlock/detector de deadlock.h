# ifndef _HILO_DETECTOR_DEADLOCK_
# define _HILO_DETECTOR_DEADLOCK_

#include "../../hilos del team/hilos_team.h"

void team_ejecutar_algoritmo_de_deteccion_de_deadlock();

bool algoritmo_detectar_deadlock(entrenadores candidatos);

void algoritmo_procesar_deadlock();

void loggear_deadlock(bool resultado);

void esperar_que_equipo_no_pueda_cazar_mas();

#endif
