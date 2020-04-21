# ifndef _TEST_PRUEBA_CSPECS_
# define _TEST_PRUEBA_CSPECS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cspecs/cspec.h>

void test(){

	context (test_de_prueba) {

		describe("Tests de enteros") {	//Agrupa tests
			it("la suma de un numero y su opuesto es 0") { //test individual
				should_int(1+ (-1)) be equal to(0);
			} end //dentro del context t-o-d-o termina con end

			it("2+2 no es pez"){
				should_int(2+2) not be equal to(5);
			}end

		} end

		describe("Tests de string"){
			it("longitud de la cadena"){
				char* unaCadena = "instruction pointer";
				should_int(strlen(unaCadena)) be equal to(19);
				should_string(unaCadena) not be equal to("mnemonico");
			}end
			
		} end

		describe("Tests de bool"){
			it("1 es verdad"){
				should_bool(1) be truthy;
			}end
			
			it("1 negado es falso"){
				should_bool(!1) be falsey;
			}end
		}end

		describe("Tests de punteros"){
			int*i = NULL;

			before{ //Se ejecuta antes de cada test
				i = malloc(sizeof(int));
				*i = 4;
			}end

			after{ //Se ejecuta despues de cada test, a fin de reestablecer las condiciones para el proximo test.
				free(i);
			}end

			it("dos punteros apuntan al mismo lugar en memoria  y por lo tanto son iguales"){
				int*j = i;
				should_ptr(j) be equal to(i);
			}end

//			it("causar segmentation fault"){
//				char*error_de_estupido = NULL;
//				*error_de_estupido = 9;
//
//				//Probar: En teoria corta todo lo que sigue, pero a mi me anduvo
//			}end
//
//			it("test interrumpido por error del anterior"){
//			}end

		}end

//		describe("siguiente describe funciona a pesar de error?"){
//			it("por ejemplo"){
//			}end
//		}end
	}
}

#endif
