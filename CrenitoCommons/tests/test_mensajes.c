#include "../src/crenito-commons/mensajes/mensaje_new_pokemon.h"
#include "../src/crenito-commons/mensajes/mensaje_localized_pokemon.h"
#include "../src/crenito-commons/mensajes/mensaje_get_pokemon.h"
#include <crenito-commons/mensajes/mensaje_appeared_catch_pokemon.h>
#include <commons/collections/list.h>
#include "test_utils.h"

context (test_mensajes) {

	void should_ids(t_mensaje_id esperado, t_mensaje_id real) {

		should_int(esperado.id) be equal to (esperado.id);
		should_int(esperado.id_correlativo) be equal to (esperado.id_correlativo);
	}

	void assert_mensaje_new_pokemon(t_mensaje_new_pokemon* esperado, 
			t_mensaje_new_pokemon* real) {

		should_ptr(real) not be null;
		should_ids(real->ids, esperado->ids);
		should_int(real -> cantidad) be equal to (esperado -> cantidad);
		should_pokemon(real->pokemon, esperado->pokemon);
	}

	void assert_mensaje_localized_pokemon(t_mensaje_localized_pokemon* esperado,
			t_mensaje_localized_pokemon* real) {

		should_ptr(real) not be null;
		should_ids(real->ids, esperado->ids);
		should_int(real -> especie_lenght) be equal to (esperado -> especie_lenght);
		should_string(real ->especie) be equal to (esperado ->especie);
		should_int(list_size(real -> posiciones)) be equal to (list_size(esperado -> posiciones));

		int i = 0;
		for (i = 0; i < esperado->posiciones_lenght; ++i) {

			t_posicion* posicion_esperada = list_get(esperado->posiciones, i);
			t_posicion* posicion_real = list_get(real->posiciones, i);

			should_ptr(posicion_esperada) be equal to (posicion_real);
		}

	}

	void assert_mensaje_get_pokemon(t_mensaje_get_pokemon* esperado,
			t_mensaje_get_pokemon* real) {

		should_ptr(real) not be null;
		should_ids(real->ids, esperado->ids);
		should_int(real -> especie_lenght) be equal to (esperado -> especie_lenght);
		should_string(real ->especie) be equal to (esperado ->especie);
	}

	void assert_mensaje_appeared_catch_pokemon(t_mensaje_appeared_catch_pokemon* esperado,
			t_mensaje_appeared_catch_pokemon* real) {

		should_ptr(real) not be null;
		should_ids(real->ids, esperado->ids);
		should_pokemon(esperado->pokemon, real->pokemon);
	}

	describe("serializacion de mensajes") {
		it("Serializacion mensaje_new_pokemon") {

			//Arrange
			t_mensaje_new_pokemon* pkm_esperado = mensaje_new_pokemon_crear("pickachu", 1, 2, 3);

			//Action
			t_buffer* pkm_serializado = mensaje_new_pokemon_serializar(pkm_esperado);
			t_mensaje_new_pokemon* pkm_real = mensaje_new_pokemon_deserializar(pkm_serializado);

			//Assert
			assert_mensaje_new_pokemon(pkm_esperado, pkm_real);

			//Free
			mensaje_new_pokemon_destruir(pkm_esperado);
			mensaje_new_pokemon_destruir(pkm_real);

		}end

		it("Serializacion mensaje_localized_pokemon") {

			//Arrange
			t_list * posiciones = list_create();

			t_posicion* p0 = posicion_crear_ptr(0, 0);
			list_add(posiciones, p0);

			t_posicion* p1 = posicion_crear_ptr(2, 4);
			list_add(posiciones, p1);

			t_posicion* p2 = posicion_crear_ptr(5, 3);
			list_add(posiciones, p2);

			t_mensaje_localized_pokemon* lcd_esperado = mensaje_localized_pokemon_crear("bulbasaur", posiciones);

			//Action
			t_buffer* lcd_serializado = mensaje_localized_pokemon_serializar(lcd_esperado);
			t_mensaje_localized_pokemon* lcd_real = mensaje_localized_pokemon_deserializar(lcd_serializado);

			//Assert
			assert_mensaje_localized_pokemon(lcd_esperado, lcd_real);

			//Free
			mensaje_localized_pokemon_destruir(lcd_esperado);
			mensaje_localized_pokemon_destruir(lcd_real);

		}end

		it("Serializacion mensaje_get_pokemon") {

			//Arrange
			t_mensaje_get_pokemon* get_pkm_esperado = mensaje_get_pokemon_crear("charmander");

			//Action
			t_buffer* get_pkm_serializado = mensaje_get_pokemon_serializar(get_pkm_esperado);
			t_mensaje_get_pokemon* get_pkm_real = mensaje_get_pokemon_deserializar(get_pkm_serializado);

			//Assert
			assert_mensaje_get_pokemon(get_pkm_esperado, get_pkm_real);

			//Free
			mensaje_get_pokemon_destruir(get_pkm_esperado);
			mensaje_get_pokemon_destruir(get_pkm_real);

		}end

		it("Serializacion mensaje_appeared_catch_pokemon") {

			//Arrange
			t_mensaje_appeared_catch_pokemon* msj_esperado = mensaje_appeared_catch_pokemon_crear("squirtle", 7, 9);

			//Action
			t_buffer* msj_serializado = mensaje_appeared_catch_pokemon_serializar(msj_esperado);
			t_mensaje_appeared_catch_pokemon* msj_real = mensaje_appeared_catch_pokemon_deserializar(msj_serializado);

			//Assert
			assert_mensaje_appeared_catch_pokemon(msj_esperado, msj_real);

			//Free
			mensaje_appeared_catch_pokemon_destruir(msj_esperado);
			mensaje_appeared_catch_pokemon_destruir(msj_real);

		}end

	}end
}
