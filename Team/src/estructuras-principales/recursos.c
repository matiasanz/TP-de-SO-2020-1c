#include "../estructuras-principales/recursos.h"

matriz_recursos recursos_create(){
	return dictionary_create();
}

matriz_recursos recursos_from_string(char*cadena){

	if(string_is_empty(cadena)){
		return recursos_create();
	}

	matriz_recursos recursosLeidos = dictionary_create();

		void _a_la_matriz(especie_pokemon unaEspecie) {
			recursos_agregar_validado(recursosLeidos, unaEspecie);
		}

    especie_pokemon*tokens = string_split(cadena, "|");

    string_iterate_lines(tokens, _a_la_matriz);

//    void mostrar(t_posicion pos){
//    	printf("pos: (%u, %u)\n", pos.x, pos.y);
//    }

    string_iterate_lines(tokens, (void*)free); //TODO ver antes rompia
//    free(tokens);

    return recursosLeidos;
}

numero*recursos_get(matriz_recursos recursos, especie_pokemon unaEspecie){
	return dictionary_get(recursos, unaEspecie);
}

numero recursos_cantidad_de_instancias_de(matriz_recursos recursos, recurso unaEspecie){
	numero*cantidadDeInstancias = recursos_get(recursos, unaEspecie);
	return cantidadDeInstancias? *cantidadDeInstancias : 0;
}

void recursos_agregar_recurso(matriz_recursos recursos, recurso unaEspecie){
	recursos_agregar_N_instancias_de(recursos, unaEspecie, 1);
}

void recursos_agregar_validado(matriz_recursos recursos, recurso unaEspecie){
	if(!unaEspecie){
		error_show("E de Error de lectura (y de estupido)");
		exit(1);
	}

	recursos_agregar_recurso(recursos, unaEspecie);
}

void recursos_agregar_N_instancias_de(matriz_recursos recursos, recurso unaEspecie, numero cantidad){
	numero* cantidadDeInstancias = recursos_get(recursos, unaEspecie);

	if(cantidadDeInstancias){
		(*cantidadDeInstancias)+=cantidad;
	}
	else{
		cantidadDeInstancias = malloc(sizeof(numero));
		*cantidadDeInstancias = cantidad;
		dictionary_put(recursos, unaEspecie, cantidadDeInstancias);
	}
}

void recursos_quitar_instancia_de_recurso(matriz_recursos unaMatriz, recurso unRecurso){
	numero*cantidadDeInstancias = recursos_get(unaMatriz, unRecurso);
	if(!cantidadDeInstancias){
		error_show("Se intento quitar una instancia de un recurso que no esta disponible");
		exit(1);
	}

	(*cantidadDeInstancias)--;
}

void recursos_destroy(matriz_recursos recursos){
	dictionary_destroy_and_destroy_elements(recursos, free);
}

bool recursos_hay_recursos(matriz_recursos recursos){
	return !recursos_matriz_nula(recursos);
}

void recursos_mostrar(matriz_recursos recursos){

	if(recursos_hay_recursos(recursos)){
		void mostrar(especie_pokemon unaEspecie, void*cantidadDeInstancias){
			printf(" (%s, %d)", unaEspecie, *((numero*)cantidadDeInstancias));
		}

		dictionary_iterator(recursos, &mostrar);
	}

	else{
		printf(" NO HAY RECURSOS ");
	}
}

bool recursos_matriz_nula(matriz_recursos recursos){
	bool esNula = true;

	void esoNoEsCierto(char*recurso, void*cantidad){
		if(*((numero*)cantidad)){
			esNula = false;
		}
	}

	dictionary_iterator(recursos, esoNoEsCierto);

	return esNula || dictionary_is_empty(recursos);
}

numero recursos_contar(matriz_recursos recursos){
	numero cantidad = 0;

	void contar(recurso unRecurso, void*suCantidad){
		cantidad+= *((numero*)suCantidad);
	}

	dictionary_iterator(recursos, contar);

	return cantidad;
}

//Operaciones entre matrices
void recursos_sumar_recursos_a(matriz_recursos unaMatriz, matriz_recursos otraMatriz){
	void sumar_recursos(especie_pokemon unaEspecie, void*cantidadDeInstancias){
		recursos_agregar_N_instancias_de(unaMatriz, unaEspecie, *((numero*) cantidadDeInstancias));
	}

	dictionary_iterator(otraMatriz, sumar_recursos); //itero la otra matriz, dado que no necesariamente va a tener recursos de igual tipo

}

int max(int i, int j){
	return i>j? i: j;
}

void recursos_restar_recursos_a(matriz_recursos minuendo, matriz_recursos sustraendo){
	void restar_semejantes(especie_pokemon unaEspecie, void*cantidadDeInstancias){
		*((numero*) cantidadDeInstancias) -= recursos_cantidad_de_instancias_de(sustraendo, unaEspecie);
	}

	dictionary_iterator(minuendo, restar_semejantes);
}

matriz_recursos recursos_duplicar(matriz_recursos recursos){
	matriz_recursos duplicada = recursos_create();

	void duplicar(char* recurso, void*cantidad){
		numero* cantidadDuplicada = malloc(sizeof(numero));
		      * cantidadDuplicada = *((numero*)cantidad);

		dictionary_put(duplicada, recurso, cantidadDuplicada);
	}

	dictionary_iterator(recursos, duplicar);

	return duplicada;
}

matriz_recursos recursos_matriz_suma(matriz_recursos unaMatriz, matriz_recursos otra){
	matriz_recursos suma = recursos_duplicar(unaMatriz);
	recursos_sumar_recursos_a(suma, otra);

	return suma;
}

matriz_recursos recursos_matriz_diferencia(matriz_recursos unaMatriz, matriz_recursos otraMatriz){
	matriz_recursos matrizDiferencia = recursos_duplicar(unaMatriz);
	recursos_restar_recursos_a(matrizDiferencia, otraMatriz);
	return matrizDiferencia;
}

void recursos_resta_positiva_de_recursos_a(matriz_recursos unaMatriz, matriz_recursos otraMatriz){
	void restar_semejantes(especie_pokemon unaEspecie, void*cantidadDeInstancias){
		numero instancias = *((numero*) cantidadDeInstancias) - recursos_cantidad_de_instancias_de(otraMatriz, unaEspecie);

		*((numero*) cantidadDeInstancias) = max((int) instancias, 0);
	}

	dictionary_iterator(unaMatriz, restar_semejantes);
}

matriz_recursos recursos_matriz_diferencia_positiva(matriz_recursos unaMatriz, matriz_recursos otraMatriz){
	matriz_recursos matrizDiferencia = recursos_duplicar(unaMatriz);
	recursos_resta_positiva_de_recursos_a(matrizDiferencia, otraMatriz);
	return matrizDiferencia;
}

bool recursos_suficientes_para(matriz_recursos proveedora, matriz_recursos receptora){
	matriz_recursos diferencia = recursos_matriz_diferencia_positiva(receptora, proveedora);
	bool SiONo = recursos_matriz_nula(diferencia);
	recursos_destroy(diferencia);

	return SiONo;
}

recurso recursos_alguno_en_comun_con(matriz_recursos quien, matriz_recursos conQuien){
	bool enComun(recurso unRecurso, void*cantidad){
		return *((int*) cantidad) && recursos_cantidad_de_instancias_de(conQuien, unRecurso);
	}

	return dictionary_get_any_key(quien, &enComun);
}

recurso recursos_cualquier_recurso(matriz_recursos recursos){
	bool cualquiera(recurso unRecurso, void* cantidad){
		return *((int*)cantidad)>0;
	}

	return dictionary_get_any_key(recursos, &cualquiera);
}

