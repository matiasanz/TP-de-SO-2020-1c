#include "../dominio/estructuras auxiliares/listas/cr_list.h"
#include <cspecs/cspec.h>

//	especies_pokemones especies = cr_list_create();
//	char*especie = "Jamon";
//	if(cr_list_silent_non_repetitive_add(especies, especie, especie_cmp)) puts("hola");
//	if(!cr_list_silent_non_repetitive_add(especies, especie, especie_cmp)) puts("chau");
//
//	cr_list_destroy(especies);

void test_listas(){ //Prueba rapida. A futuro hacer mas
	context(tests){
		describe("Comparador"){

			bool numcmp(int* a, int*b){
				return *a == *b;
			}

			cr_list*unaLista;
			int*i;

			before{
				i = malloc(sizeof(int));
				*i=2;
				unaLista = cr_list_create();
				cr_list_add_and_signal(unaLista, i);
			}end

			after{
				free(i);
				cr_list_destroy(unaLista);
			}end

			it("lista contiene elemento"){
				should_bool(list_contains_element(unaLista->lista, i, (bool(*)(void*, void*)) &numcmp)) be truthy;
			}end
		}end
	}
}
