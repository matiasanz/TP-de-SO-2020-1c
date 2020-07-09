/*
 * test_utils.c
 *
 *  Created on: 24 may. 2020
 *      Author: utnso
 */

#include "test_utils.h"

void inicializar_logs() {

	logger = log_create("./log/test.log", "TESTS", 1, LOG_LEVEL_INFO);
	event_logger = log_create("./log/test_event.log", "TESTS", 1,
			LOG_LEVEL_ERROR);
}

//+++++++++++++++++++++ SHOULD

void should_posicion(t_posicion esperado, t_posicion real) {

	should_int(esperado.pos_x) be equal to (real.pos_x);
	should_int(esperado.pos_y) be equal to (real.pos_y);
}

void should_pokemon(t_pokemon esperado, t_pokemon real) {

	should_int(esperado.especie_lenght) be equal to (real.especie_lenght);
	should_string(esperado.especie) be equal to (real.especie);
	should_posicion(esperado.posicion, real.posicion);
}

//+++++++++++++++++++++ ASSERT

void assert_mensaje_new_pokemon(t_mensaje_new_pokemon* esperado,
		t_mensaje_new_pokemon* real) {

	should_ptr(real) not be null;
	should_int(mensaje_new_pokemon_get_id(real)) be equal to
	(mensaje_new_pokemon_get_id(esperado));

	should_int(mensaje_new_pokemon_get_id_correlativo(real)) be equal to
	(mensaje_new_pokemon_get_id_correlativo(esperado));

	should_int(real -> cantidad) be equal to (esperado -> cantidad);
	should_pokemon(real->pokemon, esperado->pokemon);
}

void assert_mensaje_localized_pokemon(t_mensaje_localized_pokemon* esperado,
		t_mensaje_localized_pokemon* real) {

	should_ptr(real) not be null;
	should_int(mensaje_localized_pokemon_get_id(real)) be equal to
	(mensaje_localized_pokemon_get_id(esperado));

	should_int(mensaje_localized_pokemon_get_id_correlativo(real)) be equal to
	(mensaje_localized_pokemon_get_id_correlativo(esperado));

	should_int(real -> especie_lenght) be equal to (esperado -> especie_lenght);
	should_string(real ->especie) be equal to (esperado ->especie);
	should_int(list_size(real -> posiciones)) be equal to (list_size(esperado -> posiciones));

	int i = 0;
	for (i = 0; i < esperado->posiciones_lenght; ++i) {

		t_posicion* posicion_esperada = list_get(esperado->posiciones, i);
		t_posicion* posicion_real = list_get(real->posiciones, i);

		should_posicion(*posicion_esperada, *posicion_real);
	}

}

void assert_mensaje_get_pokemon(t_mensaje_get_pokemon* esperado,
		t_mensaje_get_pokemon* real) {

	should_ptr(real) not be null;
	should_int(mensaje_get_pokemon_get_id(real)) be equal to
	(mensaje_get_pokemon_get_id(esperado));

	should_int(mensaje_get_pokemon_get_id_correlativo(real)) be equal to
	(mensaje_get_pokemon_get_id_correlativo(esperado));
	should_int(real -> especie_lenght) be equal to (esperado -> especie_lenght);
	should_string(real ->especie) be equal to (esperado ->especie);
}

void assert_mensaje_appeared_catch_pokemon(
		t_mensaje_appeared_catch_pokemon* esperado,
		t_mensaje_appeared_catch_pokemon* real) {

	should_ptr(real) not be null;

	should_int(mensaje_appeared_catch_pokemon_get_id(real)) be equal to
	(mensaje_appeared_catch_pokemon_get_id(esperado));

	should_int(mensaje_appeared_catch_pokemon_get_id_correlativo(real)) be equal to
	(mensaje_appeared_catch_pokemon_get_id_correlativo(esperado));

	should_pokemon(real->pokemon, esperado->pokemon);
}

void assert_mensaje_caught_pokemon(t_mensaje_caught_pokemon* esperado,
		t_mensaje_caught_pokemon* real) {

	should_ptr(real) not be null;
	should_int(mensaje_caught_pokemon_get_id(real)) be equal to
	(mensaje_caught_pokemon_get_id(esperado));

	should_int(mensaje_caught_pokemon_get_id_correlativo(real)) be equal to
	(mensaje_caught_pokemon_get_id_correlativo(esperado));
	should_int(real -> atrapado) be equal to (esperado -> atrapado);
}

void assert_mensaje_recibido(t_id_cola id_cola_esperada, void* serializado_real) {

	void* msj_real;
	void* msj_esperado;

	switch (id_cola_esperada) {
	case NEW_POKEMON:

		msj_real = mensaje_new_pokemon_deserializar(serializado_real);
		mensaje_new_pokemon_log(logger, msj_real);
		msj_esperado = mensaje_new_pokemon_test();
		assert_mensaje_new_pokemon(msj_esperado, msj_real);
		mensaje_new_pokemon_destruir(msj_esperado);
		mensaje_new_pokemon_destruir(msj_real);
		break;

	case APPEARED_POKEMON:
	case CATCH_POKEMON:

		msj_real = mensaje_appeared_catch_pokemon_deserializar(serializado_real);
		mensaje_appeared_catch_pokemon_log(logger, msj_real, get_nombre_cola(id_cola_esperada));
		msj_esperado = mensaje_appeared_catch_pokemon_test();
		assert_mensaje_appeared_catch_pokemon(msj_esperado, msj_real);
		mensaje_appeared_catch_pokemon_destruir(msj_esperado);
		mensaje_appeared_catch_pokemon_destruir(msj_real);
		break;

	case CAUGHT_POKEMON:

		msj_real = mensaje_caught_pokemon_deserializar(serializado_real);
		mensaje_caught_pokemon_log(logger, msj_real);
		msj_esperado = mensaje_caught_pokemon_test();
		assert_mensaje_caught_pokemon(msj_esperado, msj_real);
		mensaje_caught_pokemon_destruir(msj_esperado);
		mensaje_caught_pokemon_destruir(msj_real);
		break;

	case GET_POKEMON:

		msj_real = mensaje_get_pokemon_deserializar(serializado_real);
		mensaje_get_pokemon_log(logger, msj_real);
		msj_esperado = mensaje_get_pokemon_test();
		assert_mensaje_get_pokemon(msj_esperado, msj_real);
		mensaje_get_pokemon_destruir(msj_esperado);
		mensaje_get_pokemon_destruir(msj_real);
		break;

	case LOCALIZED_POKEMON:

		msj_real = mensaje_localized_pokemon_deserializar(serializado_real);
		mensaje_localized_pokemon_log(logger, msj_real);
		msj_esperado = mensaje_localized_pokemon_test();
		assert_mensaje_localized_pokemon(msj_esperado, msj_real);
		mensaje_localized_pokemon_destruir(msj_esperado);
		mensaje_localized_pokemon_destruir(msj_real);
		break;

	default:
		log_error(event_logger, "No existe el tipo de cola: %d",
				id_cola_esperada);
		break;
	}

	free(serializado_real);
}

void assert_mensaje_recibido_thread(t_id_cola id_cola_esperada,
		void* serializado_real) {

	assert_mensaje_recibido(id_cola_esperada, serializado_real);

	pthread_exit(NULL);

}

//+++++++++++++++++++++ TEST_DATA

t_mensaje_new_pokemon* mensaje_new_pokemon_test() {

	t_mensaje_new_pokemon* new_pokemon = mensaje_new_pokemon_crear("pickachu",
			4, 5, 6);

	mensaje_new_pokemon_set_id(new_pokemon, 5);
	mensaje_new_pokemon_set_id_correlativo(new_pokemon, 1);

	return new_pokemon;
}

t_paquete* paquete_new_pokemon_test() {

	t_mensaje_new_pokemon* msj = mensaje_new_pokemon_test();

	t_paquete* pqt = paquete_crear(
			paquete_header_crear(MENSAJE, GAMEBOY, NEW_POKEMON, 1),
			mensaje_new_pokemon_serializar(msj));

	mensaje_new_pokemon_destruir(msj);

	return pqt;
}

t_mensaje_localized_pokemon* mensaje_localized_pokemon_test() {

	t_list * posiciones = list_create();

	t_posicion* p0 = posicion_crear_ptr(0, 0);
	list_add(posiciones, p0);

	t_posicion* p2 = posicion_crear_ptr(5, 3);
	list_add(posiciones, p2);

	t_posicion* p1 = posicion_crear_ptr(2, 4);
	list_add(posiciones, p1);

	t_mensaje_localized_pokemon* localized = mensaje_localized_pokemon_crear(
			"bulbasaur", posiciones);
	mensaje_localized_pokemon_set_id(localized, 3);
	mensaje_localized_pokemon_set_id_correlativo(localized, 2);

	list_destroy(posiciones);

	return localized;
}

t_paquete* paquete_localized_pokemon_test() {

	t_mensaje_localized_pokemon* msj = mensaje_localized_pokemon_test();

	t_paquete* pqt = paquete_crear(
			paquete_header_crear(MENSAJE, GAMEBOY, LOCALIZED_POKEMON, 1),
			mensaje_localized_pokemon_serializar(msj));

	mensaje_localized_pokemon_destruir(msj);

	return pqt;
}

t_mensaje_get_pokemon* mensaje_get_pokemon_test() {

	t_mensaje_get_pokemon* get_pkm = mensaje_get_pokemon_crear("charmander");
	mensaje_get_pokemon_set_id(get_pkm, 1);
	mensaje_get_pokemon_set_id_correlativo(get_pkm, 1);

	return get_pkm;
}

t_paquete* paquete_get_pokemon_test() {

	t_mensaje_get_pokemon* msj = mensaje_get_pokemon_test();

	t_paquete* pqt = paquete_crear(
			paquete_header_crear(MENSAJE, GAMEBOY, GET_POKEMON, 1),
			mensaje_get_pokemon_serializar(msj));

	mensaje_get_pokemon_destruir(msj);

	return pqt;
}

t_mensaje_appeared_catch_pokemon* mensaje_appeared_catch_pokemon_test() {

	t_mensaje_appeared_catch_pokemon* appeared_catch =
			mensaje_appeared_catch_pokemon_crear("squirtle", 7, 9);
	mensaje_appeared_catch_pokemon_set_id(appeared_catch, 1);
	mensaje_appeared_catch_pokemon_set_id_correlativo(appeared_catch, 2);

	return appeared_catch;
}

t_paquete* paquete_appeared_catch_pokemon_test(t_id_cola cola_esperada) {

	t_mensaje_appeared_catch_pokemon* msj =
			mensaje_appeared_catch_pokemon_test();

	t_paquete* pqt = paquete_crear(
			paquete_header_crear(MENSAJE, GAMEBOY, cola_esperada, 1),
			mensaje_appeared_catch_pokemon_serializar(msj));

	mensaje_appeared_catch_pokemon_destruir(msj);

	return pqt;
}

t_mensaje_caught_pokemon* mensaje_caught_pokemon_test() {

	t_mensaje_caught_pokemon* caught_pkm = mensaje_caught_pokemon_crear(1);
	mensaje_caught_pokemon_set_id(caught_pkm, 3);
	mensaje_caught_pokemon_set_id_correlativo(caught_pkm, 2);

	return caught_pkm;
}

t_paquete* paquete_caught_pokemon_test() {

	t_mensaje_caught_pokemon* msj = mensaje_caught_pokemon_test();

	t_paquete* pqt = paquete_crear(
			paquete_header_crear(MENSAJE, GAMEBOY, CAUGHT_POKEMON, 1),
			mensaje_caught_pokemon_serializar(msj));

	mensaje_caught_pokemon_destruir(msj);

	return pqt;
}
