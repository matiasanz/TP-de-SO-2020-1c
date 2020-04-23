# ifndef _TEST_PRUEBA_CSPECS_
# define _TEST_PRUEBA_CSPECS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crenito-commons/prueba.h>
#include <cspecs/cspec.h>

void test() {

	context (test_de_prueba) {

		describe("Tests de una funcion de las commons") {	//Agrupa tests
			it("llamo a las commons y soy feliz") { //test individual
				should_int(cr_get_uno()) be equal to(1);
			}end //dentro del context t-o-d-o termina con end

			it("llamo a las commons y soy feliz 2.") { //test individual
				should_int(cr_get_cero()) be equal to(0);
			}end

			it("2+2 no es pez") {
				should_int(2+2) not be equal to(5);
			}end

		}end

		describe("Tests de enteros") {	//Agrupa tests
			it("la suma de un numero y su opuesto es 0") { //test individual
				should_int(1+ (-1)) be equal to(0);
			}end //dentro del context t-o-d-o termina con end

			it("2+2 no es pez") {
				should_int(2+2) not be equal to(5);
			}end

		}end

		describe("Tests de string") {
			it("longitud de la cadena") {
				char* unaCadena = "instruction pointer";
				should_int(strlen(unaCadena)) be equal to(19);
				should_string(unaCadena) not be equal to("mnemonico");
			}end

		}end

		describe("Tests de bool") {
			it("1 es verdad") {
				should_bool(1) be truthy;
			}end

			it("1 negado es falso") {
				should_bool(!1) be falsey;
			}end
		}end

		describe("Tests de punteros") {
			int*i = NULL;

			before { //Se ejecuta antes de cada test
				i = malloc(sizeof(int));
				*i = 4;
			}end

			after { //Se ejecuta despues de cada test, a fin de reestablecer las condiciones para el proximo test.
				free(i);
			}end

			it("dos punteros apuntan al mismo lugar en memoria  y por lo tanto son iguales") {
				int*j = i;
				should_ptr(j) be equal to(i);
			}end

		}end
	}
}

#endif
