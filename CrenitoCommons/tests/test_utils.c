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

void assert_mensaje_new_pokemon(t_mensaje_new_pokemon* esperado, t_mensaje_new_pokemon* real) {

	should_ptr(real) not be null;
	should_int(mensaje_new_pokemon_get_id(real)) be equal to
	(mensaje_new_pokemon_get_id(esperado));

	should_int(mensaje_new_pokemon_get_id_correlativo(real)) be equal to
	(mensaje_new_pokemon_get_id_correlativo(esperado));

	should_int(real -> cantidad) be equal to (esperado -> cantidad);
	should_pokemon(real->pokemon, esperado->pokemon);
}

void inicializar_logs() {

	logger = log_create("./log/test.log", "TESTS", 1,LOG_LEVEL_ERROR);
	event_logger = log_create("./log/test_event.log", "TESTS", 1, LOG_LEVEL_ERROR);
}

