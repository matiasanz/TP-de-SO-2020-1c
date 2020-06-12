# ifndef _HEADER_PRINCIPAL_
# define _HEADER_PRINCIPAL_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

//#include <commons/string.h> //no deja correr tests en cspec (palabra reservada end)
#include <commons/error.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <commons/log.h>
#include <commons/config.h>

#include "listas/cr_list.h"

#include <crenito-commons/conexiones/conexiones.h>
#include "utils/mensajes-utils.h"

//#include <crenito-commons/mensajes/mensaje_new_pokemon.h>
//#include <crenito-commons/prueba.h>
//#include <crenito-commons/conexiones/conexiones.h>

# define NUMERO_MAXIMO 4294967295

//Tipos de dato
typedef uint32_t numero;
typedef numero t_id;

/*Variables Globales*/

	pthread_mutex_t Mutex_AndoLoggeando;
	pthread_mutex_t Mutex_AndoLoggeandoEventos;

/*--------------*/

# endif
