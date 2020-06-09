# ifndef _ALGORITMOS_PLANIFICACION_
# define _ALGORITMOS_PLANIFICACION_

#include "../hilos-del-team/hilos_team.h"

typedef enum {FIFO, ROUND_ROBBIN, SJF_CD, SJF_SD} t_algoritmo_planificacion;

typedef struct{
	numero* tiempo;
	numero* estimaciones;
	float alfa;
} datos_sjf;

typedef union{
	numero QUANTUM;
	datos_sjf datos;
} datos_algoritmo;

//void cargar_algoritmo_planificacion(); //TODO

bool puede_seguir_ejecutando_segun_algoritmo(entrenador*unEntrenador, numero tiempo, t_algoritmo_planificacion);

#endif
