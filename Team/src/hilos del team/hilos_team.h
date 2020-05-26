#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include <commons/collections/list.h>

#include "../dominio/estructuras principales/pokemon.h"

//Hilos
pthread_t hiloReceptorDeMensajes;
pthread_t hiloPlanificador;
pthread_t hiloMensajesAppeard;
pthread_t hiloMensajesCAUGHT;
pthread_t hiloMensajesLOCALIZED;

//Semaforos

sem_t* EjecutarEntrenador;
sem_t EntradaSalida_o_FinDeEjecucion;
sem_t HayTareasPendientes;
sem_t HayEntrenadores;

pthread_mutex_t mutexMensaje;
//pthread_mutex_t*mutexEntrenador; TODO ver usos de acuerdo a implementacion

//Variables globales
bool FinDelProceso;

/*----------------*/

//Hilo correspondiente a un entrenador
void modelo_hilo_entrenador();
void team_hilo_entrenador();

//Hilo que se encarga de planificar los hilos entrenadores
void MODELO_PLANIFICAR();
void team_planificar(); //nueva implementacion

//Hilo que se encarga de recibir los mensajes
void broker_simulator();
void team_suscriptor_cola_APPEARD(cr_list* mensajes);
void team_suscriptor_cola_CAUGHT(cr_list* mensajes);
void team_suscriptor_cola_LOCALIZED(cr_list*mensajes);

//Hilo que se encarga de reaccionar a cada mensaje recibido
void team_procesar_mensajes();

//inicializar
pthread_t* inicializar_hilos_entrenadores(int*cantidad);

//auxiliares
void registrar_pokemon(pokemon*);
void registrar_en_cada_posicion(especie_pokemon, t_list*posiciones);
void entrenadores_despertar_en_caso_de_APPEARD(entrenadores equipo, entrenadores enReady);
entrenadores entrenadores_despertar_para_APPEARD(entrenadores, pokemon*);



