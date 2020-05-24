#include "hilos_team.h"
#include "../team.h"

void team_recibir_mensajes(t_list* mensajes){
	puts("Simulacro de recepcion de mensajes");

	int i;

	for(i=0; !FinDelProceso; i= (i+1)%6){

		mensaje* unMensaje = malloc(sizeof(mensaje));

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
//				pthread_mutex_lock(&mutexCapturasPendientes);
				if(!list_is_empty(capturasPendientes)){
//					pthread_mutex_unlock(&mutexCapturasPendientes);

					t_id unid = idProximoPendienteHARDCODEADO();

					resultado_captura*unResultado = malloc(sizeof(resultado_captura));
					*unResultado = (resultado_captura){unid, true};
					*unMensaje = (mensaje) {CAUGHT_POKEMON_, unResultado};
				}
				else{
//					pthread_mutex_unlock(&mutexCapturasPendientes);
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

		puts("W(mutexMensaje)");
	pthread_mutex_lock(&mutexMensaje);
		list_add(mensajes, unMensaje);
	pthread_mutex_unlock(&mutexMensaje);
		puts("Signal(mutexMensaje)");

		sem_post(&sem_HayMensajesRecibidos);
		puts("Signal(mensaje hay mas mensajes)");
		sleep(1);

	}

	log_info(event_logger, "Finalizo hilo receptor de mensajes");
}

//void reconexion_con_broker(){ //Gustavo
//	log_info(logger,"Inicio de proceso de reintento de comunicacion con Broker");
//	//TODO
//}
