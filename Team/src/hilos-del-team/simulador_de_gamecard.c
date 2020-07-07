#include "../hilos-del-team/hilos_team.h"
#include "../team.h"

#include <crenito-commons/mensajes/mensaje_localized_pokemon.h>
#include <crenito-commons/mensajes/mensaje_caught_pokemon.h>
#include <crenito-commons/mensajes/mensaje_appeared_catch_pokemon.h>

void hardcodear_appeared(especie_pokemon especie, coordenada x, coordenada y){
	char* especiePkm = string_from_format(especie);
	appeared_pokemon_recibido(mensaje_appeared_catch_pokemon_crear(especiePkm, x, y));
	free(especiePkm);
}

void gamecard_simulator(){
	puts("Simulacro de recepcion de mensajes");

	FIN_PSEUDO_GAMECARD = false;

	int i;
	pthread_mutex_init(&mutex_PSEUDOGAMECARD, NULL);

	for(i=0; PROCESO_ACTIVO; i= (i+1)%11){

		switch(i){
			case 0: {
				char* especie = string_from_format("Flareon");
				t_list*posiciones = list_create();
				list_add(posiciones, posicion_ptr_create(6, 0));
				list_add(posiciones, posicion_ptr_create(1, 4));

				t_mensaje_localized_pokemon*mensaje = mensaje_localized_pokemon_crear(especie, posiciones);
				localized_pokemon_recibido(mensaje);

				free(especie);
				list_destroy(posiciones);
				break;
			}

			case 1: {
				hardcodear_appeared("Umbreon", 2, 9);
				break;
			}

			case 2: {
				hardcodear_appeared("Espeon", 7, 4);
				break;
			}

			case 3:{
				char* especie = string_from_format("Pikachu");

				t_list*posiciones = list_create();
				list_add(posiciones, posicion_ptr_create(2, 7));
				list_add(posiciones, posicion_ptr_create(8, 1));
				list_add(posiciones, posicion_ptr_create(3, 5));
				list_add(posiciones, posicion_ptr_create(4, 9));

				t_mensaje_localized_pokemon*mensaje = mensaje_localized_pokemon_crear(especie, posiciones);

				mensaje_localized_pokemon_set_id(mensaje, 1);

				localized_pokemon_recibido(mensaje);
				list_destroy(posiciones);
				break;

			}

			case 4:{
				hardcodear_appeared("Bulbasaur", 3, 1);
				break;
			}

			case 5:{
				hardcodear_appeared("Vaporeon", 3, 1);
				break;
			}

			case 6:{
				char* especie = string_from_format("Oblivion");

				t_list*posiciones = list_create();
				list_add(posiciones, posicion_ptr_create(3, 3));

				t_mensaje_localized_pokemon*mensaje = mensaje_localized_pokemon_crear(especie, posiciones);
				mensaje_localized_pokemon_set_id(mensaje, 1);
				localized_pokemon_recibido(mensaje);

				free(especie);
				list_destroy(posiciones);
				break;
			}

			case 7:{
				hardcodear_appeared("Jolteon", 2, 9);
				break;
			}

			case 8: {
				hardcodear_appeared("Pikachu", 4, 4);
				break;
			}

			case 9: {
				hardcodear_appeared("Onix", 4, 4);
				break;
			}

			case 10: {
				hardcodear_appeared("Charmander", 4, 4);
				break;
			}

			case 11: {
				hardcodear_appeared("Flareon", 4, 4);
				break;
			}

			default:{
				hardcodear_appeared("Zapato", 2500, 9);
				break;
			}
		}


		sleep(1);

		pthread_mutex_lock(&mutex_PSEUDOGAMECARD);
		if(FIN_PSEUDO_GAMECARD){
			pthread_mutex_unlock(&mutex_PSEUDOGAMECARD);
			break;
		} else{
			pthread_mutex_unlock(&mutex_PSEUDOGAMECARD);
		}
	}
}


