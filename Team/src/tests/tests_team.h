#include "../dominio/estructuras principales/pokemon.h"
#include "../dominio/estructuras auxiliares/mensajes.h"
#include "../dominio/estructuras auxiliares/captura_pendiente.h"
#include "../team.h"
#include "factory.h"

#include <cspecs/cspec.h>

void assert_equals_pendiente(pendiente* mensajePendiente, pendiente esperado);

context(tests_team){
	describe("posicion"){
		t_posicion unaPosicion = posicion_create(1,0);

		it("Se creo una posicion"){
			should_int(unaPosicion.pos_x) be equal to(1);
			should_int(unaPosicion.pos_y) be equal to(0);
		}end

		it("distancia sobre eje x"){
			should_int(posicion_distancia(unaPosicion, posicion_create(2,0))) be equal to(1);
		}end

		it("distancia sobre eje y"){
			should_int(posicion_distancia(unaPosicion, posicion_create(1,3))) be equal to(3);
		}end

		it("distancia sobre dos ejes"){
			should_int(posicion_distancia(unaPosicion, posicion_create(4,5)) ) be equal to(8);
		}end
	}end
	describe("especie de pokemon"){
		it("dos especies son iguales"){
			should_bool(especie_cmp("garudamon", "garudamon")) be truthy;
		}end

		it("dos especies son diferentes"){
			should_bool(especie_cmp("garudamon", "zapato")) be falsey;
		}end
	}end

	describe("entrenador"){
		entrenador* unEntrenador;
		t_posicion unaPosicion = posicion_create(5,4);

		before{
			unEntrenador = factory_entrenador_de_prueba();
		}end

		after{
			entrenador_destroy(unEntrenador);
		}end

		it("un entrenador se crea correctamente"){
			especie_pokemon unaEspecie = list_remove(unEntrenador->objetivos,0);
			should_bool(especie_cmp(unaEspecie, "pucho")) be truthy;
			should_bool(list_is_empty(unEntrenador->objetivos)) be truthy;
			should_bool(list_is_empty(unEntrenador->pokemonesCazados)) be truthy;
			should_bool(posicion_cmp(unEntrenador->posicion, posicion_create(1,2))) be truthy;
		}end

		it("un entrenador se desplaza de una posicion a otra"){
			entrenador_ir_a(unEntrenador, unaPosicion);
			should_bool(posicion_cmp(unEntrenador->posicion, unaPosicion)) be truthy;
		}end

		it("un entrenador mueve a su misma posicion"){
			t_posicion posActual = unEntrenador->posicion;
			entrenador_ir_a(unEntrenador, posActual);
			should_bool(posicion_cmp(unEntrenador->posicion, posActual)) be truthy;
		}end

		it("un entrenador puede cazar mas pokemones"){
			should_bool(entrenador_puede_cazar_mas_pokemones(*unEntrenador)) be truthy;
		}end

		it("un entrenador captura un pokemon correctamente"){
			pokemon*unaVictima = factory_pokemon_de_prueba();
			entrenador_capturar(unEntrenador, unaVictima);
			
			should_string(list_get(unEntrenador->pokemonesCazados, 0)) be equal to("pucho");
		}end

		it("un entrenador cumple sus objetivos y no puede cazar mas pokemones"){
			pokemon*pokemonDePrueba = malloc(sizeof(pokemon));
				   *pokemonDePrueba = pokemon_create("pucho", 5, 6);
			entrenador_capturar(unEntrenador, pokemonDePrueba);
			
			puts("hola");
			should_bool(entrenador_puede_cazar_mas_pokemones(*unEntrenador)) be falsey;
		}end

		it("un entrenador que necesita muchos pokemon, puede seguir cazando despues de capturar uno"){
			pokemon* pokemonDePrueba = factory_pokemon_de_prueba();
			especie_pokemon especie= "pochi";
			list_add(unEntrenador->objetivos, &especie);
			entrenador_capturar(unEntrenador, pokemonDePrueba);			
			
			should_bool(entrenador_puede_cazar_mas_pokemones(*unEntrenador)) be truthy;
		}end

		it("un entrenador se bloquea y pasa a estado <<LOCKED>>"){
			entrenador_pasar_a(unEntrenador, LOCKED_HASTA_APPEARD, "");
			should_int(unEntrenador->estado) be equal to(LOCKED_HASTA_APPEARD);
		}end
	}end

	describe("entrenadores"){
		entrenadores equipo;
		before{
			equipo = entrenadores_create();
			list_add(equipo, factory_entrenador_de_prueba());
		}end

		after{
			entrenadores_destroy(equipo);
		}end

		it("objetivos globales de un equipo con un solo entrenador que tiene un objetivo"){
			especies_pokemones objetivos = entrenadores_objetivos_globales(equipo);
			should_string(list_remove(objetivos, 0)) be equal to("pucho");
			should_bool(list_is_empty(objetivos)) be truthy;
			list_destroy(objetivos);
		}end
		
		it("objetivos globales de un equipo vacio son vacios"){
			entrenadores equipoVacio = list_create();
			especies_pokemones objetivos = entrenadores_objetivos_globales(equipoVacio);
			should_bool(list_is_empty(objetivos)) be truthy;
			
			list_destroy(objetivos);
			entrenadores_destroy(equipoVacio);
		}end
		
		it("objetivos globales de un equipo con mas de un entrenador"){
			entrenadores unEquipo = factory_entrenadores_de_prueba(5);
			
			especies_pokemones objetivos = entrenadores_objetivos_globales(unEquipo);
			
			int i;
			for(i=0; !list_is_empty(objetivos); i++){
				especie_pokemon unObjetivo = list_remove(objetivos, 0);
				especie_pokemon especieEsperada = itoa(i);
				should_string(unObjetivo) be equal to(especieEsperada);
			}
			
			list_destroy(objetivos);
			entrenadores_destroy(unEquipo);
		}end
	}end
	
	describe("pokemon"){
		pokemon*unPokemon;

		before{
			 unPokemon = factory_pokemon_de_prueba();
		}end

		after{
			pokemon_destroy(unPokemon);
		}end

		it("un pokemon se crea correctamente"){
			should_string(unPokemon->especie) be equal to("pucho");
			should_bool(posicion_cmp(unPokemon->posicion, (t_posicion){3,6})) be truthy;
		}end
	}end

	describe("Mapa"){ //cambiarle el nombre al tipo de dato
		mapa_pokemones mapaDePokemones;
		pokemon*unPokemon;
		before{
			mapaDePokemones = mapa_create();
			unPokemon = factory_pokemon_de_prueba();
			mapa_mapear_objetivo(mapaDePokemones, unPokemon);
		}end

		after{
			mapa_destroy(mapaDePokemones);
		}end

		it("un pokemon se mapea correctamente"){
			pokemon*pokemonMapeado = cr_list_get(mapaDePokemones, 0);
			should_ptr(pokemonMapeado) be equal to(unPokemon);
		}end

		it("un pokemon se desmapea correctamente"){
			pokemon*pokemonMapeado = mapa_desmapear(mapaDePokemones);
			should_ptr(pokemonMapeado) be equal to(unPokemon);
			pokemon_destroy(pokemonMapeado);
		}end
	}end

	describe("Respuesta pendiente"){
		
		pendiente respuestaPendiente;
		entrenador* unEntrenador;
		pokemon*unPokemon;
		
		before{
			unEntrenador = factory_entrenador_de_prueba();
			unPokemon = factory_pokemon_de_prueba();
			respuestaPendiente = pendiente_create((t_id)5, unEntrenador, unPokemon);
		}end
		
		after{
			entrenador_destroy(unEntrenador);
			pokemon_destroy(unPokemon);
		}end
		
		it("Un pendiente se crea correctamente"){
			should_int(respuestaPendiente.id) be equal to((t_id) 5);
			should_ptr(respuestaPendiente.cazador) be equal to(unEntrenador);
			should_ptr(respuestaPendiente.pokemonCatcheado) be equal to(unPokemon);
		}end
	}end
	
	describe("Pendientes"){
		pendientes mensajesPendientes;
		pendiente respuestaPendiente;
		entrenador* unEntrenador;
		pokemon*unPokemon;
		
		before{
			unEntrenador = factory_entrenador_de_prueba();
			unPokemon = factory_pokemon_de_prueba();
			
			mensajesPendientes = pendientes_create();
			respuestaPendiente = pendiente_create(5, unEntrenador, unPokemon);
			agregar_pendiente(mensajesPendientes, 5, unEntrenador, unPokemon);
		}end
		
		after{
			pendientes_destroy(mensajesPendientes);
			entrenador_destroy(unEntrenador);
			pokemon_destroy(unPokemon);
			
		}end
		
		it("un mensaje pendiente se agrega correctamente"){
			pendiente* agregado = cr_list_remove_and_signal(mensajesPendientes, 0);
			assert_equals_pendiente(agregado, respuestaPendiente);
			pendiente_destroy(agregado);
		}end
		
		it("un mensaje pendiente se obtiene a partir de su id"){
			pendiente* agregado = pendientes_get(mensajesPendientes, respuestaPendiente.id);
			assert_equals_pendiente(agregado, respuestaPendiente);
			pendiente_destroy(agregado);
		}end
		
		it("un mensaje pendiente se quita correctamente"){
			pendiente* agregado = pendientes_get(mensajesPendientes, respuestaPendiente.id);
			cr_list_is_empty(mensajesPendientes);
			pendiente_destroy(agregado);
		}end
				
		skip ("si se coloca un id incorrecto, corta la ejecucion"){
			pendientes_get(mensajesPendientes, 2500);
		}end
	}end
}

//Funciones auxiliares
void assert_equals_pendiente(pendiente* mensajePendiente, pendiente esperado){
		should_int(mensajePendiente->id) be equal to(esperado.id);
		should_ptr(mensajePendiente->cazador) be equal to(esperado.cazador);
		should_ptr(mensajePendiente->pokemonCatcheado) be equal to(esperado.pokemonCatcheado);
}
