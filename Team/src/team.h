# ifndef _TEAM_
# define _TEAM_

#include "dominio/estructuras principales/pokemon.h"

#include "dominio/estructuras auxiliares/lector_config.h"
#include "dominio/estructuras auxiliares/captura_pendiente.h"
#include "dominio/estructuras auxiliares/mensajes.h"
#include "dominio/deadlock/detector de deadlock.h"
#include "dominio/deadlock/estructuras_deadlock.h"

#include "dominio/header_global_team.h"

#include <pthread.h>

//#include <crenito-commons/conexiones/conexiones.h>
//#include <crenito-commons/utils.h>

//funciones del team
void team_inicializar();
int team_exit();

//funciones auxiliares
void inicializar_listas();
void listas_destroy();

void subscribpcion_colas(); //(?)

void inicializar_semaforos();
void finalizar_semaforos();

void inicializar_hilos();
void finalizar_hilos();


/*Mis Listas (son variables globales, para que puedan acceder todos los hilos)*/
	entrenadores equipo;
	cola_entrenadores entrenadoresReady;
	mapa_pokemones pokemonesRequeridos;
	matriz_recursos objetivosGlobales;
	matriz_recursos inventariosGlobales; //cambiar nombre por recursosDisponiblesGlobales
	matriz_recursos recursosEnMapa; //cambiar nombre por recursosPorAdquirir o algo mas representativo
	pendientes capturasPendientes; //cambiar tipo de dato por capturas_pendientes
	especies_pokemones historialDePokemones;
	cr_list*mensajesAPPEARD;
	cr_list*mensajesCAUGHT;
	cr_list*mensajesLOCALIZED;
	candidatos_intercambio potencialesDeadlock;

/*--------------*/

# endif
