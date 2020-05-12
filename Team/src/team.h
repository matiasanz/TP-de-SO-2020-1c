# ifndef _TEAM_
# define _TEAM_

#include "dominio/estructuras principales/pokemon.h"

#include "dominio/estructuras auxiliares/lector_config.h"
#include "dominio/estructuras auxiliares/captura_pendiente.h"
#include "dominio/estructuras auxiliares/mensajes.h"

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
void subscribpcion_colas();

/*Mis Listas (son variables globales, para que puedan acceder todos los hilos)*/
	entrenadores equipo;
	mapa_pokemones pokemonesRequeridos;
	pendientes mensajesPendientes;
	especies_pokemones historialDePokemones;
/*--------------*/

# endif
