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
#include "../src/crenito-commons/conexiones/conexiones.h"
#include "../../GameCard/src/hilos/hilo_catch_pokemon.h"
#include "../../GameCard/src/hilos/hilo_get_pokemon.h"
#include "../../GameCard/src/hilos/hilo_new_pokemon.h"
#include "../../GameCard/src/hilos/hilo_gameboy.h"
#include "../src/crenito-commons/conexiones/paquete.h"
#include <cspecs/cspec.h>

#include "../../Team/src/suscripciones/hilo_appeared_pokemon.h"
#include "../../Team/src/suscripciones/hilo_caught_pokemon.h"
#include "../../Team/src/suscripciones/hilo_gameboy.h"
#include "../../Team/src/suscripciones/hilo_localized_pokemon.h"

void inicializar_logs();

void should_posicion(t_posicion esperado, t_posicion real);
void should_pokemon(t_pokemon esperado, t_pokemon real);

void assert_mensaje_recibido(t_id_cola id_cola_esperada, void* serializado_real);

void assert_mensaje_new_pokemon(t_mensaje_new_pokemon* esperado, t_mensaje_new_pokemon* real);
void assert_mensaje_localized_pokemon(t_mensaje_localized_pokemon* esperado, t_mensaje_localized_pokemon* real);
void assert_mensaje_get_pokemon(t_mensaje_get_pokemon* esperado, t_mensaje_get_pokemon* real);
void assert_mensaje_appeared_catch_pokemon(t_mensaje_appeared_catch_pokemon* esperado, t_mensaje_appeared_catch_pokemon* real);
void assert_mensaje_caught_pokemon(t_mensaje_caught_pokemon* esperado, t_mensaje_caught_pokemon* real);

t_mensaje_new_pokemon* mensaje_new_pokemon_test();
t_paquete* paquete_new_pokemon_test();

t_mensaje_localized_pokemon* mensaje_localized_pokemon_test();
t_paquete* paquete_localized_pokemon_test();

t_mensaje_get_pokemon* mensaje_get_pokemon_test();
t_paquete* paquete_get_pokemon_test();

t_mensaje_appeared_catch_pokemon* mensaje_appeared_catch_pokemon_test();
t_paquete* paquete_appeared_catch_pokemon_test(t_id_cola id_cola);

t_mensaje_caught_pokemon* mensaje_caught_pokemon_test();
t_paquete* paquete_caught_pokemon_test();

#endif /* TESTS_TEST_UTILS_H_ */
