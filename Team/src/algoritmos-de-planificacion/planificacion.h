# ifndef _ALGORITMOS_PLANIFICACION_
# define _ALGORITMOS_PLANIFICACION_

#include "../hilos-del-team/hilos_team.h"

typedef enum {FIFO, ROUND_ROBBIN, SJF_CD, SJF_SD} t_algoritmo_planificacion;
//Ver si sirve de algo

typedef struct{
	numero* tiempo;
	numero* estimaciones;
	float alfa;
} datos_sjf;

typedef union{
	numero QUANTUM;
	datos_sjf datos;
} datos_algoritmo;

//Inicializar
void cargar_algoritmo_planificacion();

//FIFO
//Proximo a planificar
	entrenador*proximo_segun_fifo(cola_entrenadores colaReady);

//SJF

//RR

//Generales
	bool puede_seguir_sin_desalojo();

//Consumir CPU
void consumir_ciclo_cpu();
void entrenador_consumir_N_cpu(entrenador*, numero);
void entrenador_esperar_y_consumir_cpu(entrenador*);

#endif
