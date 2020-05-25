#include "hilos_team.h"
#include "../team.h"

void broker_simulator(){
	puts("Simulacro de recepcion de mensajes");

	sem_wait(&BORRAR_ESTO_MENSAJE_GET_ENVIADO); //pasar al switch de forma que vaya recorriendo la lista de objetivos globales (QUE TENGO QUE CORREGIR)
	mensaje* unMensaje = malloc(sizeof(mensaje));
	*unMensaje = (mensaje) {LOCALIZED_POKEMON_, localized_pokemon_crear("Pikachu")}; //a futuro le pasaria la cant de instancias que necesito del recurso

	cr_list_add_and_signal(mensajesLOCALIZED, unMensaje);

	int i;

	for(i=0; !FinDelProceso; i= (i+1)%6){

		unMensaje = malloc(sizeof(mensaje));

		switch(i){
			case 0: {
				*unMensaje = (mensaje) {APPEARD_POKEMON_, pokemon_ptr_create("Charmander", 2, 9)};
				break;
			}

			case 1: {
				*unMensaje = (mensaje) {APPEARD_POKEMON_, pokemon_ptr_create("Pikachu", 7, 4)};
				break;
			}

			case 2:{
				*unMensaje = (mensaje) {APPEARD_POKEMON_, pokemon_ptr_create("Pidgey", 3, 1)};
				break;
			}

			case 3:{
				pendiente*unPendiente = cr_list_get(capturasPendientes, 0);

				if(unPendiente){

					t_id unid = unPendiente->id;

					resultado_captura*unResultado = malloc(sizeof(resultado_captura));
					*unResultado = (resultado_captura){unid, true};
					*unMensaje = (mensaje) {CAUGHT_POKEMON_, unResultado};
				}
				else{
					puts("No hay capturas pendientes");
					*unMensaje = (mensaje) {APPEARD_POKEMON_, pokemon_ptr_create("Bulbasaur", 3, 1)};
				}

				break;
			}

			case 4:{
				*unMensaje = (mensaje) {APPEARD_POKEMON_, pokemon_ptr_create("Squirtle", 3, 1)};
				break;
			}

			default:{
				*unMensaje = (mensaje) {APPEARD_POKEMON_, pokemon_ptr_create("Zapato", 2500, 3)};
				break;
			}
		}

		switch(unMensaje->opcode){
			case APPEARD_POKEMON_:{
				cr_list_add_and_signal(mensajesAPPEARD, unMensaje);
				break;
			}

			case CAUGHT_POKEMON_:{
				cr_list_add_and_signal(mensajesCAUGHT, unMensaje);
				break;
			}

			default: break;
		}
//		puts("W(mutexMensaje)");
//	pthread_mutex_lock(&mutexMensaje);
//		list_add(mensajes, unMensaje);
//	pthread_mutex_unlock(&mutexMensaje);
//		puts("Signal(mutexMensaje)");
//
//		sem_post(&sem_HayMensajesRecibidos);
//		puts("Signal(mensaje hay mas mensajes)");
		sleep(1);

	}

	log_info(event_logger, "Finalizo hilo receptor de mensajes");
}

//void reconexion_con_broker(){ //Gustavo
//	log_info(logger,"Inicio de proceso de reintento de comunicacion con Broker");
//	//TODO
//}
