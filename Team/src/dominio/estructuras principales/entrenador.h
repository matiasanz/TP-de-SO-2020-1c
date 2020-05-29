# ifndef _TAD_ENTRENADOR_
# define _TAD_ENTRENADOR_ //PASAR A COMMONS

#include <semaphore.h>
#include "posicion.h"

typedef /*const*/char* especie_pokemon;
typedef t_dictionary* matriz_recursos;

typedef enum{NEW,READY,	EXECUTE, LOCKED_HASTA_APPEARD, LOCKED_HASTA_DEADLOCK, LOCKED_HASTA_CAUGHT,	EXIT} t_estado; //VER cuales vale la pena conservar
typedef enum{CATCHEAR, CAPTURAR, DEADLOCK /*, FINALIZAR*/ } t_tarea;

//TAD Entrenador
typedef struct pcb_Entrenador{
	matriz_recursos objetivos; //especie de los pokemones
	matriz_recursos pokemonesCazados;
	t_posicion posicion; //(x,y)
	t_estado estado;
	t_id id;
	t_tarea siguienteTarea;
} entrenador;

//	Constructor; agregar lista infinita de objetivos
	entrenador entrenador_create(t_id, matriz_recursos pokemonesEnInventario, matriz_recursos objetivos, t_posicion unaPos);

	entrenador*entrenador_ptr_create(t_id, matriz_recursos pokemonesEnInventario, matriz_recursos objetivos, t_posicion);

//	Desplaza un entrenador de una posicion a otra
	void entrenador_ir_a(entrenador*, t_posicion);

// Retorna true si la posicion del entrenador coincide con la dada
	bool entrenador_llego_a(entrenador, t_posicion);

// Retorna true si la cantidad de pokemones cazados no supera la cantidad permitida para ese entrenador
	bool entrenador_puede_cazar_mas_pokemones(entrenador*);

// Retorna true si todos los objetivos fueron cazados
	bool entrenador_objetivos_cumplidos(entrenador*);

// Retorna true si el entrenador se encuentra actualmente en dicho estado
	bool entrenador_en_estado(entrenador* unEntrenador, t_estado ESTADO);

//	Pasa entrenador a un estado y loggea el pasaje con el motivo
	void entrenador_pasar_a(entrenador*unEntrenador, t_estado estadoFinal, const char*motivo);

// Retorna una cadena de caracteres con el estado
	char*estadoFromEnum(t_estado);

//Destructor
	void entrenador_destroy(entrenador*);

//bool entrenador_necesita_recurso(entrenador*unEntrenador);

//****************************************************************************************
//Equipo completo
typedef t_list* entrenadores;

//Constructor
	entrenadores entrenadores_create();

// Remueve al entrenador con ese id y retorna un puntero al mismo
entrenador* entrenadores_remover_del_equipo_a(entrenadores, t_id);

//retorna los objetivos de todos los entrenadores
	matriz_recursos entrenadores_objetivos_globales(entrenadores);

// Retorna una matriz con los pokemones que fueron capturados por todos los entrenadores
	matriz_recursos entrenadores_inventarios_globales(entrenadores unEquipo);

//retorna el entrenador mas cercano a una posicion
	entrenador* entrenadores_mas_cercano(entrenadores, t_posicion);

//Retorna los entrenadores que se encuentran en un estado
	entrenadores entrenadores_en_estado(entrenadores, t_estado estado);

//Retorna true si el entrenador requiere al menos una instancia de la especie
	bool entrenador_necesita_recurso(entrenador*unEntrenador); //TODO

//Retorna true si los recursos que tiene en su inventario cubren sus objetivos
	bool entrenador_cumplio_sus_objetivos(entrenador*unEntrenador);

//Destructor
	void entrenadores_destroy(entrenadores);

//***************************************************************************************
//Cola de entrenadores
typedef cr_list* cola_entrenadores;

//Agrega a la cola y avisa que hay elementos
	void entrenador_agregar_a_cola(entrenador*, cola_entrenadores);

//Queda en wait hasta que haya algun elemento en la cola y luego remueve el primer elemento
	entrenador*entrenador_esperar_y_desencolar(cola_entrenadores);

//Retorna el proximo entrenador a ser planificado
	entrenador* entrenadores_proximo_a_planificar(cola_entrenadores); //, criterio); Hacer con enum

//Retorna el id del proximo entrenador a ser planificado
	t_id* entrenadores_id_proximo_a_planificar(cola_entrenadores);


//****************************************************************************************
//TAD Recursos (implementado con t_dictionary*)
typedef especie_pokemon recurso;

// Constructor
	matriz_recursos recursos_create();

// Destructor
	void recursos_destroy(matriz_recursos recursos);

// Retorna la cantidad de instancias que se tienen de ese recurso
	numero recursos_cantidad_de_instancias_de(matriz_recursos, recurso);

// Incrementa en 1 la cantidad de instancias de un recurso
	void recursos_agregar_recurso(matriz_recursos, recurso);

// Agrega el recurso luego de validar que el mismo no es NULL
	void recursos_agregar_validado(matriz_recursos, recurso);

// Incrementa en N la cantidad de instancias de un recurso
	void recursos_agregar_N_instancias_de(matriz_recursos, recurso, numero);

// "R1|R2|R2|R3|R3|R3" --> {(R1,1), (R2,2), (R3,3)}
	matriz_recursos recursos_from_string(char*cadena);

// Muestra los recursos junto con su cantidad
	void recursos_mostrar(matriz_recursos);

// A la primera matriz le suma los recursos de la segunda
	void recursos_sumar_recursos_a(matriz_recursos, matriz_recursos);

// A la primera matriz le resta el contenido de la segunda, asumiendo que tiene recursos del mismo tipo
	void recursos_restar_recursos_a(matriz_recursos, matriz_recursos);

// Retorna la diferencia entre la primera y la segunda
	matriz_recursos recursos_matriz_diferencia(matriz_recursos, matriz_recursos);

// Retorna true si todos los recursos tienen 0 instancias
	bool recursos_matriz_nula(matriz_recursos);

// Retorna la cantidad total de recursos que posee la matriz
	numero recursos_contar(matriz_recursos);

// Retorna true si la proveedora tiene mas instancias de cada recurso que la receptora
	bool recursos_suficientes_para(matriz_recursos proveedora, matriz_recursos receptora);

# endif
