#include "../src/crenito-commons/mensajes/mensaje_new_pokemon.h"
#include "../src/crenito-commons/mensajes/mensaje_localized_pokemon.h"
#include <commons/collections/list.h>
#include <cspecs/cspec.h>

context (test_mensajes) {

	void assert_mensaje_new_pokemon(t_mensaje_new_pokemon* esperado,
			t_mensaje_new_pokemon* real) {

		should_ptr(real) not be null;
		should_int(real -> ids.id) be equal to (esperado -> ids.id);
		should_int(real -> ids.id_correlativo) be equal to (esperado -> ids.id_correlativo);
		should_int(real -> cantidad) be equal to (esperado -> cantidad);
		should_int(real -> pokemon.especie_lenght) be equal to (esperado -> pokemon.especie_lenght);
		should_string(real -> pokemon.especie) be equal to (esperado -> pokemon.especie);
		should_int(real -> pokemon.posicion.pos_x) be equal to (esperado -> pokemon.posicion.pos_x);
		should_int(real -> pokemon.posicion.pos_y) be equal to (esperado -> pokemon.posicion.pos_y);

	}

	void assert_mensaje_localized_pokemon(t_mensaje_localized_pokemon* esperado,
			t_mensaje_localized_pokemon* real) {

		should_ptr(real) not be null;
		should_int(real -> ids.id) be equal to (esperado -> ids.id);
		should_int(real -> ids.id_correlativo) be equal to (esperado -> ids.id_correlativo);
		should_int(real -> especie_lenght) be equal to (esperado -> especie_lenght);
		should_string(real ->especie) be equal to (esperado ->especie);
		should_int(list_size(real -> posiciones)) be equal to (list_size(esperado -> posiciones));

		int i = 0;
		for (i = 0; i < esperado->posiciones_lenght; ++i) {
			t_posicion* posicion_esperada = list_get(esperado->posiciones, i);
			t_posicion* posicion_real = list_get(real->posiciones, i);

			should_ptr(posicion_real) be equal to (posicion_esperada);
		}

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

	}end
}
