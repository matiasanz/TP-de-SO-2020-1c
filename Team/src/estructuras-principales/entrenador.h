# ifndef _TAD_ENTRENADOR_
# define _TAD_ENTRENADOR_ //PASAR A COMMONS

#include "../estructuras-principales/posicion.h"
#include "../estructuras-principales/recursos.h"

typedef enum{NEW,READY,	EXECUTE, LOCKED_HASTA_APPEARED, LOCKED_HASTA_DEADLOCK, LOCKED_HASTA_CAUGHT,	EXIT} t_estado; //VER cuales vale la pena conservar
typedef enum{CATCHEAR, CAPTURAR, INTERCAMBIAR /*, FINALIZAR*/ } t_tarea;

//TAD Pokemon									 Pasar a commons
typedef struct Pokemon{
	especie_pokemon especie; //string
	t_posicion posicion;
}pokemon;

//TAD Entrenador
typedef struct pcb_Entrenador{
	matriz_recursos objetivos; //especie de los pokemones
	matriz_recursos pokemonesCazados;
	t_posicion posicion; //(x,y)
	t_estado estado;
	t_id id;
	t_tarea siguienteTarea;
	pokemon*pokemonEntreOjos;
} entrenador;

//	Constructor
	entrenador entrenador_create(t_id, matriz_recursos pokemonesEnInventario, matriz_recursos objetivos, t_posicion unaPos);

// Constructor de puntero
	entrenador*entrenador_ptr_create(t_id, matriz_recursos pokemonesEnInventario, matriz_recursos objetivos, t_posicion);

//Constructor de entrenador al cual le puedo pasar los recursos como cadenas de caracteres. Ejemplo "Pikachu|Pidgey|Zapato"
	entrenador*entrenador_ptr_crear(t_id id, char* asignados, char* pedidos, t_posicion);

//	Desplaza un entrenador de una posicion a otra
//  Definicion en hiloEntrenador
	void entrenador_desplazarse_hacia(entrenador*, t_posicion);

// Retorna true si la posicion del entrenador coincide con la dada
//  Definicion en hiloEntrenador
	bool entrenador_llego_a(entrenador*, t_posicion);

// Retorna true si la cantidad de pokemones cazados no supera la cantidad permitida para ese entrenador
	bool entrenador_puede_cazar_mas_pokemones(entrenador*);

// Retorna true si el entrenador se encuentra actualmente en dicho estado
	bool entrenador_en_estado(entrenador* unEntrenador, t_estado ESTADO);

//	Pasa entrenador a un estado y loggea el pasaje con el motivo
//  Definida en hiloEntrenador
	void entrenador_pasar_a(entrenador*unEntrenador, t_estado estadoFinal, const char*motivo);

// Retorna una cadena de caracteres con el estado
	char*estadoFromEnum(t_estado);

// Retorna una matriz con los recursos que capturo y no necesita
	matriz_recursos entrenador_recursos_sobrantes(entrenador*);

// Retorna una matriz con los objetivos que aun no logro capturar
	matriz_recursos entrenador_recursos_pedidos(entrenador*);

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
	bool entrenador_necesita_recurso(entrenador*, especie_pokemon);//TODO

//Retorna true si los recursos que tiene en su inventario cubren sus objetivos
	bool entrenador_cumplio_sus_objetivos(entrenador*unEntrenador);

//Destructor de equipo
	void entrenadores_disolver_equipo(entrenadores unEquipo);

//Destructor de equipo y de entrenadores
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


# endif
