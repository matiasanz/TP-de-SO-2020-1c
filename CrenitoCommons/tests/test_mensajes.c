#include "test_utils.h"

context (test_mensajes) {

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

	describe("serializacion de mensajes") {
		it("Serializacion mensaje_new_pokemon") {

			//Arrange
			t_mensaje_new_pokemon* pkm_esperado = mensaje_new_pokemon_crear("pickachu", 1, 2, 3);
			mensaje_new_pokemon_set_id(pkm_esperado, 5);
			mensaje_new_pokemon_set_id_correlativo(pkm_esperado, 9);

			//Action
			t_buffer* pkm_serializado = mensaje_new_pokemon_serializar(pkm_esperado);
			t_mensaje_new_pokemon* pkm_real = mensaje_new_pokemon_deserializar(pkm_serializado -> stream);

			//Assert
			assert_mensaje_new_pokemon(pkm_esperado, pkm_real);

			//Free
			buffer_destruir(pkm_serializado);
			mensaje_new_pokemon_destruir(pkm_esperado);
			mensaje_new_pokemon_destruir(pkm_real);

		}end

		it("Serializacion mensaje_localized_pokemon") {

			//Arrange
			t_list * posiciones = list_create();

			t_posicion* p0 = posicion_crear_ptr(0, 0);
			list_add(posiciones, p0);

			t_posicion* p2 = posicion_crear_ptr(5, 3);
			list_add(posiciones, p2);

			t_posicion* p1 = posicion_crear_ptr(2, 4);
			list_add(posiciones, p1);

			t_mensaje_localized_pokemon* lcd_esperado = mensaje_localized_pokemon_crear("bulbasaur", posiciones);
			mensaje_localized_pokemon_set_id(lcd_esperado, 3);
			mensaje_localized_pokemon_set_id_correlativo(lcd_esperado, 2);

			//Action
			t_buffer* lcd_serializado = mensaje_localized_pokemon_serializar(lcd_esperado);
			t_mensaje_localized_pokemon* lcd_real = mensaje_localized_pokemon_deserializar(lcd_serializado -> stream);

			//Assert
			assert_mensaje_localized_pokemon(lcd_esperado, lcd_real);

			//Free
			list_destroy(posiciones);
			buffer_destruir(lcd_serializado);
			mensaje_localized_pokemon_destruir(lcd_esperado);
			mensaje_localized_pokemon_destruir(lcd_real);

		}end

		it("Serializacion mensaje_get_pokemon") {

			//Arrange
			t_mensaje_get_pokemon* get_pkm_esperado = mensaje_get_pokemon_crear("charmander");
			mensaje_get_pokemon_set_id(get_pkm_esperado, 1);
			mensaje_get_pokemon_set_id_correlativo(get_pkm_esperado, 1);

			//Action
			t_buffer* get_pkm_serializado = mensaje_get_pokemon_serializar(get_pkm_esperado);
			t_mensaje_get_pokemon* get_pkm_real = mensaje_get_pokemon_deserializar(get_pkm_serializado -> stream);

			//Assert
			assert_mensaje_get_pokemon(get_pkm_esperado, get_pkm_real);

			//Free
			buffer_destruir(get_pkm_serializado);
			mensaje_get_pokemon_destruir(get_pkm_esperado);
			mensaje_get_pokemon_destruir(get_pkm_real);

		}end

		it("Serializacion mensaje_appeared_catch_pokemon") {

			//Arrange
			t_mensaje_appeared_catch_pokemon* msj_esperado = mensaje_appeared_catch_pokemon_crear("squirtle", 7, 9);
			mensaje_appeared_catch_pokemon_set_id(msj_esperado, 1);
			mensaje_appeared_catch_pokemon_set_id_correlativo(msj_esperado, 2);
			//Action
			t_buffer* msj_serializado = mensaje_appeared_catch_pokemon_serializar(msj_esperado);
			t_mensaje_appeared_catch_pokemon* msj_real = mensaje_appeared_catch_pokemon_deserializar(msj_serializado -> stream);

			//Assert
			assert_mensaje_appeared_catch_pokemon(msj_esperado, msj_real);

			//Free
			buffer_destruir(msj_serializado);
			mensaje_appeared_catch_pokemon_destruir(msj_esperado);
			mensaje_appeared_catch_pokemon_destruir(msj_real);

		}end

		it("Serializacion mensaje_caught_pokemon") {

			//Arrange
			t_mensaje_caught_pokemon* msj_esperado = mensaje_caught_pokemon_crear(1);
			mensaje_caught_pokemon_set_id(msj_esperado, 3);
			mensaje_caught_pokemon_set_id_correlativo(msj_esperado, 2);

			//Action
			t_buffer* msj_serializado = mensaje_caught_pokemon_serializar(msj_esperado);
			t_mensaje_caught_pokemon* msj_real = mensaje_caught_pokemon_deserializar(msj_serializado -> stream);

			//Assert
			assert_mensaje_caught_pokemon(msj_esperado, msj_real);

			//Free
			buffer_destruir(msj_serializado);
			mensaje_caught_pokemon_destruir(msj_esperado);
			mensaje_caught_pokemon_destruir(msj_real);

		}end

	}end
}
