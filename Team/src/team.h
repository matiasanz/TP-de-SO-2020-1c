# ifndef _TEAM_
# define _TEAM_

#include "header_global_team.h"

#include <pthread.h>

#include "algoritmos-de-planificacion/planificacion.h"
#include "deadlock/candidatos_intercambio.h"
#include "deadlock/detector_de_deadlock.h"
#include "estructuras-auxiliares/captura_pendiente.h"
#include "lector-config/lector_config.h"
#include "estructuras-auxiliares/mensajes.h"
#include "estructuras-principales/pokemon.h"

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

//Variables globales
	t_algoritmo_planificacion ALGORITMO_PLANIFICACION;
	numero RETARDO_CICLO_CPU;

//Variables de orden superior
	entrenador* (*proximo_a_ejecutar_segun_criterio)(cola_entrenadores);
	bool (*entrenador_puede_seguir_ejecutando_segun_algoritmo)(entrenador*, numero);
	void (*actualizar_datos_del_entrenador)(entrenador*unEntrenador);

/*Mis Listas (son variables globales, para que puedan acceder todos los hilos)*/
	entrenadores equipo;
	cola_entrenadores entrenadoresReady;
	mapa_pokemones pokemonesRequeridos;
	matriz_recursos objetivosGlobales;
	matriz_recursos inventariosGlobales; //cambiar nombre por recursosDisponiblesGlobales
	matriz_recursos recursosEnMapa; //cambiar nombre por recursosPorAdquirir o algo mas representativo
	capturas_pendientes capturasPendientes; //cambiar tipo de dato por capturas_pendientes
	especies_pokemones historialDePokemones;
	cr_list*mensajesAPPEARED;
	cr_list*mensajesCAUGHT;
	cr_list*mensajesLOCALIZED;
	candidatos_intercambio potencialesDeadlock;

/*--------------*/

# endif
