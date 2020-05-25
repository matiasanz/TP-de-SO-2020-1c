# ifndef _TAD_ENTRENADOR_
# define _TAD_ENTRENADOR_ //PASAR A COMMONS

#include <semaphore.h>
#include "posicion.h"

typedef char* especie_pokemon;
typedef t_list* especies_pokemones;
typedef t_dictionary recursos;

typedef enum{NEW,READY,	EXECUTE, LOCKED_HASTA_APPEARD, LOCKED_HASTA_DEADLOCK, LOCKED_HASTA_CAUGHT,	EXIT} t_estado; //VER cuales vale la pena conservar
typedef enum{CATCHEAR, CAPTURAR, DEADLOCK /*, FINALIZAR*/ } t_tarea;

//TAD Entrenador
typedef struct pcb_Entrenador{
	especies_pokemones objetivos; //especie de los pokemones
	especies_pokemones pokemonesCazados;
	t_posicion posicion; //(x,y)
	t_estado estado;
	t_id id;
	t_tarea siguienteTarea;
} entrenador;

//	Constructor; agregar lista infinita de objetivos
	entrenador entrenador_create(t_id, especies_pokemones pokemonesEnInventario, especies_pokemones objetivos, t_posicion unaPos);

	entrenador*entrenador_ptr_create(t_id, especies_pokemones pokemonesEnInventario, especies_pokemones objetivos, t_posicion);

//	Desplaza un entrenador de una posicion a otra
	void entrenador_ir_a(entrenador*, t_posicion);

// Retorna true si la posicion del entrenador coincide con la dada
	bool entrenador_llego_a(entrenador, t_posicion);

// Retorna true si la cantidad de pokemones cazados no supera la cantidad permitida para ese entrenador
	bool entrenador_puede_cazar_mas_pokemones(entrenador);

// Retorna true si todos los objetivos fueron cazados
	bool entrenador_objetivos_cumplidos(entrenador*);

// Retorna true si el entrenador se encuentra actualmente en dicho estado
	bool entrenador_en_estado(entrenador* unEntrenador, t_estado ESTADO);

//	Pasa entrenador a un estado y loggea el pasaje con el motivo
	void entrenador_pasar_a(entrenador*unEntrenador, t_estado estadoFinal, const char*motivo);

//Destructor
	void entrenador_destroy(entrenador*);

//****************************************************************************************
//Cola de entrenadores
typedef t_list* entrenadores;

//Constructor
	entrenadores entrenadores_create();

// Remueve al entrenador con ese id y retorna un puntero al mismo
entrenador* entrenadores_remover_del_equipo_a(entrenadores, t_id);

//retorna los objetivos de todos los entrenadores
	especies_pokemones entrenadores_objetivos_globales(entrenadores);

//retorna el entrenador mas cercano a una posicion
	entrenador* entrenadores_mas_cercano(entrenadores, t_posicion);

//Retorna el proximo entrenador a ser planificado
	entrenador* entrenadores_proximo_a_planificar(entrenadores); //, criterio); Hacer con enum

//Retorna el id del proximo entrenador a ser planificado
	t_id* entrenadores_id_proximo_a_planificar(entrenadores equipo);
//Retorna los entrenadores que se encuentran en un estado
	entrenadores entrenadores_en_estado(entrenadores, t_estado estado);

//Pasa al entrenador que se este ejecutando a LOCKED hasta APPEARD
	void entrenadores_bloquear_por_captura(entrenadores);

//Destructor
	void entrenadores_destroy(entrenadores);

# endif
