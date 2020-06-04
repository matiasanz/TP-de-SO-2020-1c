#include "test_utils.h"

context (test_mensajes) {


	describe("serializacion de mensajes") {
		it("Serializacion mensaje_new_pokemon") {

			//Arrange
			t_mensaje_new_pokemon* pkm_esperado = mensaje_new_pokemon_test();

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
			t_mensaje_localized_pokemon* lcd_esperado = mensaje_localized_pokemon_test();

			//Action
			t_buffer* lcd_serializado = mensaje_localized_pokemon_serializar(lcd_esperado);
			t_mensaje_localized_pokemon* lcd_real = mensaje_localized_pokemon_deserializar(lcd_serializado -> stream);

			//Assert
			assert_mensaje_localized_pokemon(lcd_esperado, lcd_real);

			//Free
			buffer_destruir(lcd_serializado);
			mensaje_localized_pokemon_destruir(lcd_esperado);
			mensaje_localized_pokemon_destruir(lcd_real);

		}end

		it("Serializacion mensaje_get_pokemon") {

			//Arrange
			t_mensaje_get_pokemon* get_pkm_esperado = mensaje_get_pokemon_test();

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
			t_mensaje_appeared_catch_pokemon* msj_esperado = mensaje_appeared_catch_pokemon_test();
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
			t_mensaje_caught_pokemon* msj_esperado = mensaje_caught_pokemon_test();

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
