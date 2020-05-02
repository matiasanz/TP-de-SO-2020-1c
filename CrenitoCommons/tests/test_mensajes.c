#include "../src/crenito-commons/mensajes/mensaje_new_pokemon.h"
#include <cspecs/cspec.h>

static t_mensaje_new_pokemon* crear_mensaje_new_pokemon(char* nombre, int pos_x,
		int pos_y, int cantidad) {

	t_mensaje_new_pokemon* new_pkm = mensaje_new_pokemon_crear();

	new_pkm->cantidad = cantidad;
	new_pkm->especie_lenght = strlen(nombre) + 1;

	new_pkm->especie = malloc(new_pkm->especie_lenght);
	strcpy(new_pkm->especie, nombre);

	new_pkm->posicion->x = pos_x;
	new_pkm->posicion->y = pos_y;

	return new_pkm;
}

context (test_mensajes) {

	void assert_mensaje_new_pokemon(t_mensaje_new_pokemon* esperado,
			t_mensaje_new_pokemon* real) {

		should_ptr(real) not be null;
		should_int(real -> id_correlativo) be equal to (esperado -> id_correlativo);
		should_int(real -> cantidad) be equal to (esperado -> cantidad);
		should_int(real -> especie_lenght) be equal to (esperado -> especie_lenght);
		should_string(real -> especie) be equal to (esperado -> especie);
		should_ptr(real -> posicion) be equal to (esperado -> posicion);
	}

	describe("serializacion de mensajes") {

		t_buffer* pkm_serializado;
		t_mensaje_new_pokemon* pkm_esperado;
		t_mensaje_new_pokemon* pkm_real;

		after {
			mensaje_new_pokemon_destruir(pkm_esperado);
			mensaje_new_pokemon_destruir(pkm_real);
		}end

		it("Serializacion mensaje_new_pokemon") {

			//Arrange
			pkm_esperado = crear_mensaje_new_pokemon("pickachu", 1, 2, 3);

			//Action
			pkm_serializado = mensaje_new_pokemon_serializar(pkm_esperado);
			pkm_real = mensaje_new_pokemon_deserializar(pkm_serializado);

			//Assert
			assert_mensaje_new_pokemon(pkm_esperado, pkm_real);

		}end

	}end
}
