/*
 * test_utils.c
 *
 *  Created on: 24 may. 2020
 *      Author: utnso
 */

#include "test_utils.h"

void should_posicion(t_posicion esperado, t_posicion real) {

	should_int(esperado.pos_x) be equal to (real.pos_x);
	should_int(esperado.pos_y) be equal to (real.pos_y);
}

void should_pokemon(t_pokemon esperado, t_pokemon real) {

	should_int(esperado.especie_lenght) be equal to (real.especie_lenght);
	should_string(esperado.especie) be equal to (real.especie);
	should_posicion(esperado.posicion, real.posicion);
}
