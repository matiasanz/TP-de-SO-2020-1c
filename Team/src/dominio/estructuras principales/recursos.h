# ifndef _TAD_RECURSOS_
# define _TAD_RECURSOS_

# include <commons/string.h>
# include "../header_global_team.h"

typedef /*const*/char* especie_pokemon;
typedef t_dictionary* matriz_recursos;

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

// Decrementa el recurso de la matriz de pokemones
	void recursos_quitar_instancia_de_recurso(matriz_recursos, recurso);

// "R1|R2|R2|R3|R3|R3" --> {(R1,1), (R2,2), (R3,3)}
	matriz_recursos recursos_from_string(char*cadena);

// Muestra los recursos junto con su cantidad
	void recursos_mostrar(matriz_recursos);

// A la primera matriz le suma los recursos de la segunda
	void recursos_sumar_recursos_a(matriz_recursos, matriz_recursos);

// A la primera matriz le resta el contenido de la segunda, asumiendo que tienen recursos del mismo tipo y la diferencia resulta positiva
	void recursos_restar_recursos_a(matriz_recursos aQuien, matriz_recursos cuales);

// Retorna la diferencia entre la primera y la segunda
	matriz_recursos recursos_matriz_diferencia(matriz_recursos, matriz_recursos);

// Resta las componentes homologas y en caso de que resulte negativa, deja 0 - VER TODO
	void recursos_resta_positiva_de_recursos_a(matriz_recursos aQuien, matriz_recursos cuales);

// Resta las componentes homologas y en caso de que resulte negativa, deja 0 - VER TODO
	matriz_recursos recursos_matriz_diferencia_positiva(matriz_recursos, matriz_recursos);

// Retorna true si todos los recursos tienen 0 instancias
	bool recursos_matriz_nula(matriz_recursos);

// Retorna la cantidad total de recursos que posee la matriz
	numero recursos_contar(matriz_recursos);

// Retorna true si la proveedora tiene mas instancias de cada recurso que la receptora
	bool recursos_suficientes_para(matriz_recursos proveedora, matriz_recursos receptora);

// Retorna una copia de la matriz que se ingresa por parametro
	matriz_recursos recursos_duplicar(matriz_recursos);

# endif
