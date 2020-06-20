#include "../hilos-del-team/hilos_team.h"
#include "../team.h"

#include <crenito-commons/mensajes/mensaje_localized_pokemon.h>
#include <crenito-commons/mensajes/mensaje_caught_pokemon.h>
#include <crenito-commons/mensajes/mensaje_appeared_catch_pokemon.h>


void broker_simulator(){
	puts("Simulacro de recepcion de mensajes");

	FIN_PSEUDOBROKER = false;

	int i;
	pthread_mutex_init(&mutex_PSEUDOBROKER, NULL);

	for(i=0; PROCESO_ACTIVO; i= (i+1)%6){

		switch(i){
			case 0: {
				t_list*posiciones = list_create();
				list_add(posiciones, posicion_ptr_create(6, 0));
				list_add(posiciones, posicion_ptr_create(1, 4));

				t_mensaje_localized_pokemon*mensaje = mensaje_localized_pokemon_crear("Pikachu", posiciones);
				localized_pokemon_recibido(mensaje);
				break;
			}

			case 1: {
				t_mensaje_appeared_catch_pokemon*mensaje = mensaje_appeared_catch_pokemon_crear("Charmander", 2, 9);
				appeared_pokemon_recibido(mensaje);

				break;
			}

			case 2: {
				t_mensaje_appeared_catch_pokemon*mensaje = mensaje_appeared_catch_pokemon_crear("Pikachu", 7, 4);
				appeared_pokemon_recibido(mensaje);
				break;
			}

			case 3:{
				t_list*posiciones = list_create();
				list_add(posiciones, posicion_ptr_create(2, 7));
				list_add(posiciones, posicion_ptr_create(8, 1));
				list_add(posiciones, posicion_ptr_create(3, 5));
				list_add(posiciones, posicion_ptr_create(4, 9));

				t_mensaje_localized_pokemon*mensaje = mensaje_localized_pokemon_crear("Pidgey", posiciones);

				mensaje_localized_pokemon_set_id(mensaje, 1);

				localized_pokemon_recibido(mensaje);
				break;

			}

			case 4:{
				t_mensaje_appeared_catch_pokemon*mensaje = mensaje_appeared_catch_pokemon_crear("Bulbasaur", 3, 1);
				appeared_pokemon_recibido(mensaje);
				break;
			}

			case 5:{
				t_mensaje_appeared_catch_pokemon*mensaje = mensaje_appeared_catch_pokemon_crear("Squirtle", 3, 1);
				appeared_pokemon_recibido(mensaje);
				break;
			}

			case 6:{
				t_list*posiciones = list_create();
				list_add(posiciones, posicion_ptr_create(3, 3));

				t_mensaje_localized_pokemon*mensaje = mensaje_localized_pokemon_crear("Fruta", posiciones);
				mensaje_localized_pokemon_set_id(mensaje, 1);
				localized_pokemon_recibido(mensaje);

				break;
			}

			default:{
				t_mensaje_appeared_catch_pokemon*mensaje = mensaje_appeared_catch_pokemon_crear("Zapato", 2500, 3);
				appeared_pokemon_recibido(mensaje);
				break;
			}
		}


		sleep(1);

		pthread_mutex_lock(&mutex_PSEUDOBROKER);
		if(FIN_PSEUDOBROKER){
			pthread_mutex_unlock(&mutex_PSEUDOBROKER);
			break;
		} else{
			pthread_mutex_unlock(&mutex_PSEUDOBROKER);
		}
	}
}


