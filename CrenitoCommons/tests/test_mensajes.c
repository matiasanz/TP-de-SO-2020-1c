#include "../src/crenito-commons/mensajes/mensaje_new_pokemon.h"
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

	describe("serializacion de mensajes") {
		it("Serializacion mensaje_new_pokemon") {

			//Arrange
			t_mensaje_new_pokemon* pkm_esperado = mensaje_new_pokemon_crear("pickachu", 1, 2, 3);

			//Action
			t_buffer* pkm_serializado = mensaje_new_pokemon_serializar(pkm_esperado);
			t_mensaje_new_pokemon* pkm_real = mensaje_new_pokemon_deserializar(pkm_serializado);

			//Assert
			assert_mensaje_new_pokemon(pkm_esperado, pkm_real);

			mensaje_new_pokemon_destruir(pkm_esperado);
			mensaje_new_pokemon_destruir(pkm_real);

		}end

	}end
}
