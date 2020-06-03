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

void inicializar_logs();
void should_posicion(t_posicion esperado, t_posicion real);
void should_pokemon(t_pokemon esperado, t_pokemon real);
void assert_mensaje_new_pokemon(t_mensaje_new_pokemon* esperado, t_mensaje_new_pokemon* real);

#include "../src/crenito-commons/conexiones/paquete.h"

#endif /* TESTS_TEST_UTILS_H_ */
