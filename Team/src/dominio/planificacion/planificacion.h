# ifndef _ALGORITMOS_PLANIFICACION_
# define _ALGORITMOS_PLANIFICACION_

#include "../../hilos-del-team/hilos_team.h"

//******************************** Algoritmos de planificacion **********************************

//Ver si sirve de algo
typedef enum {FIFO, ROUND_ROBBIN, SJF_CD, SJF_SD} t_algoritmo_planificacion;

//TAD SJF
typedef struct{
//	numero* tiempo;
	numero* estimaciones;
	double alfa;
} datos_sjf;

//TAD DATOS ALGORITMO
typedef union{
	numero QUANTUM;
	datos_sjf sjf;
} datos_algoritmo;

//Inicializar
	void cargar_algoritmo_planificacion();
	void inicializar_fifo();
	void inicializar_sjf();
	void inicializar_rr(numero QUANTUM);

  //Proximo
	entrenador*proximo_segun_fifo(cola_entrenadores);
	entrenador*proximo_segun_rr(cola_entrenadores);
	entrenador*proximo_segun_sjf(cola_entrenadores);

  //Desalojo
	bool sin_desalojo();
	bool desalojo_en_RR(entrenador*, numero tiempo);
	bool desalojo_en_sjf_cd(entrenador*unEntrenador, numero tiempoQueLlevaEjecutando);

  //Actualizar datos
	void actualizar_datos_sjf(entrenador*unEntrenador, numero tiempoUltimaEjecucion);

  //Finalizar
	void finalizar_sjf();

//**************************************** Manejo de CPU ****************************************
	//Consumir CPU
	void consumir_ciclo_cpu();
	void entrenador_consumir_N_cpu(entrenador*, numero);
	void entrenador_esperar_y_consumir_cpu(entrenador*);

//Auxiliares
	void no_operation();
	numero estimacion_del_entrenador(entrenador*);
	entrenador*cola_entrenador_con_menor_estimacion(cola_entrenadores);
	bool entrenador_termino_de_ejecutar(entrenador*unEntrenador);

	#endif
