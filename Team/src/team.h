# ifndef _TEAM_
# define _TEAM_

#include "dominio/header_global_team.h"

#include <pthread.h>

#include "dominio/team_logs.h"

#define CONFIG_PATH "config/team.config"

//funciones del team
void team_inicializar();
int team_exit();

//funciones auxiliares
void inicializar_logs_y_config();
void finalizar_logs_y_config();

void inicializar_listas();
void listas_destroy();

void inicializar_semaforos();
void finalizar_semaforos();

void inicializar_hilos();
void finalizar_hilos();

void inicializar_conexiones();

//Estadisticas
void inicializar_estadisticas(numero);
void finalizar_estadisticas();
char*estadisticas_to_string(estadisticas_team resultados);
void team_loggear_resultados();
char*num_array_to_string(numero*arreglo, int length);


//Variables globales
	estadisticas_team Estadisticas;

	t_algoritmo_planificacion ALGORITMO_PLANIFICACION;
	numero RETARDO_CICLO_CPU;
	datos_algoritmo DATOS_ALGORITMO;

	numero PROCESOS_SIN_FINALIZAR;
	numero TIEMPO_RECONEXION;


	entrenador* (*proximo_a_ejecutar_segun_criterio)(cola_entrenadores);
	bool (*criterio_de_desalojo)(entrenador*, numero);
	void (*actualizar_datos_del_entrenador)(entrenador*unEntrenador, numero tiempo);

/*Mis Listas (son variables globales, para que puedan acceder todos los hilos)*/
	entrenadores equipo;
	cola_entrenadores entrenadoresReady;
	mapa_pokemones pokemonesRecibidos;
	matriz_recursos objetivosGlobales;
	matriz_recursos inventariosGlobales; //cambiar nombre por recursosDisponiblesGlobales
	matriz_recursos recursosEnMapa; //cambiar nombre por recursosPorAdquirir o algo mas representativo
	capturas_pendientes capturasPendientes; //cambiar tipo de dato por capturas_pendientes
	especies_pokemones historialDePokemones;
	candidatos_intercambio potencialesDeadlock;
	pthread_mutex_t mutexRepuestos;
	t_list* mapaRequeridos;
	t_list* registroDePedidos;

	//en revision
	cr_list*mensajesCAUGHT;
	cr_list*mensajesLOCALIZED;


/*--------------*/

# endif
