# ifndef _TEAM_
# define _TEAM_

#include <pthread.h>
#include "dominio/header_global_team.h"
#include "dominio/team_logs.h"

#define CARPETA_CONFIG "config/"

//funciones del team
void team_inicializar();
int team_exit();

//funciones auxiliares
void inicializar_config(char* NombreEquipo);
void inicializar_logs();
void finalizar_logs_y_config();

void inicializar_listas();
void listas_destroy();

void inicializar_semaforos();
void finalizar_semaforos();

void inicializar_hilos();
void finalizar_hilos();

void inicializar_conexiones();
void finalizar_conexiones();

void esperar_que_equipo_no_pueda_cazar_mas();
void finalizar_suscripcion_a_colas();


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

//Listas
	entrenadores equipo;
	cola_entrenadores entrenadoresReady;
	mapa_pokemones pokemonesRecibidos;
	matriz_recursos objetivosGlobales;
	matriz_recursos inventariosGlobales;
	matriz_recursos recursosEnMapa;
	capturas_pendientes capturasPendientes;
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
