# ifndef _ALGORITMOS_PLANIFICACION_
# define _ALGORITMOS_PLANIFICACION_

#include "../../hilos-del-team/hilos_team.h"

//******************************** Algoritmos de planificacion **********************************

//Ver si sirve de algo
typedef enum {FIFO, ROUND_ROBBIN, SJF_CD, SJF_SD, HRRN} t_algoritmo_planificacion;

//TAD SJF
typedef struct{
	numero* tiempoRafagaActual;
	numero* estimaciones;
	double alfa;
} datos_sjf;

//TAD HRRN
typedef struct{
	numero* espera;
	datos_sjf estimador;
} datos_hrrn;

//TAD VRR
//typedef struct{
//	numero* QUANTUM;
//} datos_vrr;

//TAD DATOS ALGORITMO
typedef union{
	numero QUANTUM;
	datos_sjf sjf;
	datos_hrrn hrrn;
} datos_algoritmo;

char* MOTIVO_DESALOJO;

//Inicializar
	void cargar_algoritmo_planificacion();
	void inicializar_fifo();
	void inicializar_sjf();
	void inicializar_rr(numero QUANTUM);
	void inicializar_hrrn(double alfa, numero cantidadDeEntrenadores, numero estimacionInicial);

  //Planificacion
	entrenador* (*proximo_a_ejecutar_segun_criterio)(cola_entrenadores);
	bool (*criterio_de_desalojo)(entrenador*, numero);
	void (*actualizar_datos_del_entrenador)(entrenador*unEntrenador, numero tiempo, bool finDeRafaga);

  //Proximo
	entrenador*proximo_segun_fifo(cola_entrenadores);
	entrenador*proximo_segun_rr(cola_entrenadores);
	entrenador*proximo_segun_sjf(cola_entrenadores);
	entrenador* proximo_segun_hrrn(cola_entrenadores);

  //Desalojo
	bool sin_desalojo();
	bool desalojo_en_rr(entrenador*, numero tiempo);
	bool desalojo_en_sjf_cd(entrenador*unEntrenador, numero tiempoQueLlevaEjecutando);

  //Actualizar datos
	void actualizar_datos_sjf(entrenador*unEntrenador, numero tiempoUltimaEjecucion, bool finDeRafaga);
	void actualizar_datos_hrrn(entrenador*unEntrenador, numero tiempoUltimaEjecucion, bool finDeRafaga);

  //Finalizar
	void finalizar_sjf();

//**************************************** Manejo de CPU ****************************************
  //Consumir CPU
	void consumir_ciclo_cpu();
	void entrenador_consumir_N_cpu(entrenador*, numero);
	void entrenador_esperar_y_consumir_cpu(entrenador*);

//**************************************** Auxiliares *******************************************
	void no_operation();

	datos_sjf estimador_create(double alfa, numero cantidadDeProcesos, numero estimacionInicial);
	void actualizar_estimador(datos_sjf* estimador, entrenador* unEntrenador, numero tiempoUltimaEjecucion, bool finDeRafaga);
	entrenador* entrenador_con_menor_response_ratio(entrenador* unEntrenador, entrenador* otroEntrenador);

	entrenador*cola_entrenador_con_menor_estimacion(cola_entrenadores colaReady);

	entrenador*cola_entrenador_con_menor_response_ratio(cola_entrenadores colaReady);


	numero entrenador_rafaga_estimada(entrenador*, datos_sjf);
	entrenador*cola_entrenador_con_menor_estimacion(cola_entrenadores);
	bool entrenador_termino_de_ejecutar(entrenador*);
	numero entrenador_tiempo_rafaga_estimado(entrenador*, datos_sjf);
	numero entrenador_tiempo_rafaga_cumplido(entrenador*, datos_sjf);
	numero entrenador_estimacion(entrenador*, datos_sjf);
	numero entrenador_tiempo_restante(entrenador*, numero tiempoEnEjecucion, datos_sjf);

#endif
