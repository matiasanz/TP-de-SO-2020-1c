# ifndef _ALGORITMOS_PLANIFICACION_
# define _ALGORITMOS_PLANIFICACION_

#include "../../hilos-del-team/hilos_team.h"

//******************************** Algoritmos de planificacion **********************************

typedef enum {FIFO, ROUND_ROBBIN, SJF_CD, SJF_SD, HRRN, VIRTUAL_ROUND_ROBBIN} t_algoritmo_planificacion;

//TAD Estimador
typedef struct{
	numero* tiempoRafagaActual;
	numero* estimaciones;
	double alfa;
} t_estimador;

//TAD DATOS ALGORITMO
typedef struct{
	numero QUANTUM; 		  // RR y VRR
	t_estimador estimador;    // SJF y HRRN
	numero* espera; 	      // HRRN
	numero* quantumConsumido; //VRR
} datos_algoritmo;

char* MOTIVO_DESALOJO;

//Inicializar
	void cargar_algoritmo_planificacion();
	void inicializar_fifo();
	void inicializar_sjf(double alfa, numero estimacionInicial, numero cantidadDeProcesos);
	void inicializar_rr(numero QUANTUM);
	void inicializar_hrrn(double alfa, numero cantidadDeEntrenadores, numero estimacionInicial);
	void inicializar_vrr(numero quantum, numero cantidadDeEntrenadores);

  //Planificacion
	entrenador* (*proximo_a_ejecutar_segun_criterio)(cola_entrenadores);
	bool (*criterio_de_desalojo)(entrenador*, numero);
	void (*actualizar_datos_del_entrenador)(entrenador*unEntrenador, numero tiempo, bool finDeRafaga);

  //Proximo
	entrenador*proximo_segun_fifo(cola_entrenadores);
	entrenador*proximo_segun_rr(cola_entrenadores);
	entrenador*proximo_segun_sjf(cola_entrenadores);
	entrenador* proximo_segun_hrrn(cola_entrenadores);
	entrenador* proximo_segun_vrr(cola_entrenadores);

  //Desalojo
	bool sin_desalojo();
	bool desalojo_en_rr(entrenador*, numero tiempo);
	bool desalojo_en_sjf_cd(entrenador*unEntrenador, numero tiempoQueLlevaEjecutando);
	bool desalojo_en_vrr(entrenador*unEntrenador, numero tiempoQueLlevaEjecutando);

  //Actualizar datos
	void actualizar_datos_sjf(entrenador*unEntrenador, numero tiempoUltimaEjecucion, bool finDeRafaga);
	void actualizar_datos_hrrn(entrenador*unEntrenador, numero tiempoUltimaEjecucion, bool finDeRafaga);
	void actualizar_datos_vrr(entrenador*unEntrenador, numero tiempoUltimaEjecucion, bool finDeRafaga);

  //Finalizar
	void finalizar_sjf();

//**************************************** Manejo de CPU ****************************************
  //Consumir CPU
	void consumir_ciclo_cpu();
	void entrenador_consumir_N_cpu(entrenador*, numero);
	void entrenador_esperar_y_consumir_cpu(entrenador*);

//**************************************** Auxiliares *******************************************

//Generales
	void no_operation();
	bool entrenador_termino_de_ejecutar(entrenador*);

//SJF
	t_estimador estimador_create(double alfa, numero cantidadDeProcesos, numero estimacionInicial);
	void actualizar_estimador(t_estimador estimador, entrenador* unEntrenador, numero tiempoUltimaEjecucion, bool finDeRafaga);
	numero entrenador_tiempo_rafaga_estimado(entrenador*, t_estimador);
	numero entrenador_tiempo_rafaga_cumplido(entrenador*, t_estimador);
	numero entrenador_estimacion(entrenador*);
	numero entrenador_tiempo_restante(entrenador*, numero tiempoEnEjecucion);
	entrenador*cola_entrenador_con_menor_estimacion(cola_entrenadores);
	numero entrenador_rafaga_estimada(entrenador*, t_estimador);

//HRRN
	numero* entrenador_get_espera(entrenador*unEntrenador);
	entrenador* entrenador_con_menor_response_ratio(entrenador* unEntrenador, entrenador* otroEntrenador);
	entrenador*cola_entrenador_con_menor_response_ratio(cola_entrenadores colaReady);

//VRR
	numero entrenador_virtual_quantum_consumido(entrenador* unEntrenador);
	numero entrenador_virtual_quantum_por_ejecutar(entrenador* unEntrenador, numero tiempo);
	bool entrenador_ready_plus(entrenador*);
	entrenador* cola_get_proximo_en_ready_plus(cola_entrenadores);

#endif
