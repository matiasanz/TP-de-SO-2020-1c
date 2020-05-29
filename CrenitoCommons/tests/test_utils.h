/*
 * test_utils.h
 *
 *  Created on: 24 may. 2020
 *      Author: utnso
 */

#ifndef TESTS_TEST_UTILS_H_
#define TESTS_TEST_UTILS_H_

#include "../src/crenito-commons/modelo/posicion.h"
#include "../src/crenito-commons/modelo/pokemon.h"
#include "../src/crenito-commons/mensajes/mensaje_new_pokemon.h"
#include "../src/crenito-commons/mensajes/mensaje_localized_pokemon.h"
#include "../src/crenito-commons/mensajes/mensaje_get_pokemon.h"
#include <crenito-commons/mensajes/mensaje_appeared_catch_pokemon.h>
#include <crenito-commons/mensajes/mensaje_caught_pokemon.h>
#include <commons/collections/list.h>

#include <cspecs/cspec.h>

void should_posicion(t_posicion esperado, t_posicion real);
void should_pokemon(t_pokemon esperado, t_pokemon real);

#endif /* TESTS_TEST_UTILS_H_ */
