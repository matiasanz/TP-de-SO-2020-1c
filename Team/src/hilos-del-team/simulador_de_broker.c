#include "../hilos-del-team/hilos_team.h"
#include "../team.h"

#include <crenito-commons/mensajes/mensaje_localized_pokemon.h>
#include <crenito-commons/mensajes/mensaje_caught_pokemon.h>
#include <crenito-commons/mensajes/mensaje_appeared_catch_pokemon.h>


void broker_simulator(){
	puts("Simulacro de recepcion de mensajes");

	finDeProceso = false;

	int i;
	pthread_mutex_init(&MUTEX_FIN_DE_PROCESO_Para_que_pseudo_broker_deje_de_mandarme_mensajes_BORRAR, NULL);

	for(i=0; PROCESO_ACTIVO; i= (i+1)%6){

		switch(i){
			case 0: {
				t_list*posiciones = list_create();
				list_add(posiciones, posicion_ptr_create(6, 0));
				list_add(posiciones, posicion_ptr_create(1, 4));

				t_mensaje_localized_pokemon*mensaje = mensaje_localized_pokemon_crear("Pikachu", posiciones);
				cr_list_add_and_signal(mensajesLOCALIZED, mensaje);
				break;
			}

			case 1: {
				t_mensaje_appeared_catch_pokemon*mensaje = mensaje_appeared_catch_pokemon_crear("Charmander", 2, 9);
				cr_list_add_and_signal(mensajesAPPEARED, mensaje);
				break;
			}

			case 2: {
				t_mensaje_appeared_catch_pokemon*mensaje = mensaje_appeared_catch_pokemon_crear("Pikachu", 7, 4);
				cr_list_add_and_signal(mensajesAPPEARED, mensaje);
				break;
			}

			case 3:{
				t_list*posiciones = list_create();
				list_add(posiciones, posicion_ptr_create(2, 7));
				list_add(posiciones, posicion_ptr_create(8, 1));

				t_mensaje_localized_pokemon*mensaje = mensaje_localized_pokemon_crear("Pidgey", posiciones);
				cr_list_add_and_signal(mensajesLOCALIZED, mensaje);
				break;

			}

			case 4:{
//				captura_pendiente*unPendiente = cr_list_get(capturasPendientes, 0);
//
//				if(unPendiente){
//					t_mensaje_caught_pokemon*mensaje = malloc(sizeof(t_mensaje_caught_pokemon));
//											*mensaje = (t_mensaje_caught_pokemon){(t_mensaje_header){unPendiente->id, unPendiente->id}, 1};
//					cr_list_add_and_signal(mensajesCAUGHT, mensaje);
//				}
//				else{
//					puts("No hay capturas pendientes");
					t_mensaje_appeared_catch_pokemon*mensaje = mensaje_appeared_catch_pokemon_crear("Bulbasaur", 3, 1);
					cr_list_add_and_signal(mensajesAPPEARED, mensaje);
//				}

				break;
			}

			case 5:{
				t_mensaje_appeared_catch_pokemon*mensaje = mensaje_appeared_catch_pokemon_crear("Squirtle", 3, 1);
				cr_list_add_and_signal(mensajesAPPEARED, mensaje);
				break;
			}

			case 6:{
				t_list*posiciones = list_create();
				list_add(posiciones, posicion_ptr_create(3, 3));

				t_mensaje_localized_pokemon*mensaje = mensaje_localized_pokemon_crear("Rodolfo", posiciones);
				cr_list_add_and_signal(mensajesLOCALIZED, mensaje);
				break;
			}

			default:{
				t_mensaje_appeared_catch_pokemon*mensaje = mensaje_appeared_catch_pokemon_crear("Zapato", 2500, 3);
				cr_list_add_and_signal(mensajesAPPEARED, mensaje);
				break;
			}
		}


		sleep(1);

		pthread_mutex_lock(&MUTEX_FIN_DE_PROCESO_Para_que_pseudo_broker_deje_de_mandarme_mensajes_BORRAR);
		if(finDeProceso){
			pthread_mutex_unlock(&MUTEX_FIN_DE_PROCESO_Para_que_pseudo_broker_deje_de_mandarme_mensajes_BORRAR);
			break;
		} else{
			pthread_mutex_unlock(&MUTEX_FIN_DE_PROCESO_Para_que_pseudo_broker_deje_de_mandarme_mensajes_BORRAR);
		}
	}
}

