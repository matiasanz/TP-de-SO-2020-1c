#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include <commons/collections/list.h>

//Hilos
pthread_t hiloReceptorDeMensajes;
pthread_t hiloProcesadorDeMensajes;

pthread_t hiloPlanificador;


//Semaforos

sem_t sem_HayMensajesRecibidos;
sem_t sem_HayMasPokemonesEnMapa;
sem_t sem_PokemonRemovido;
sem_t* sem_Entrenador;

pthread_mutex_t mutexMensaje;
pthread_mutex_t mutexMapaPokemones;
pthread_mutex_t mutexCapturasPendientes;
pthread_mutex_t mutexFinDeProceso;
pthread_mutex_t*mutexEntrenador;

//TODO definir aqui...

//Variables globales (?)
bool FinDelProceso;

/*----------------*/

//Hilo correspondiente a un entrenador
void team_hilo_entrenador();

//Hilo que se encarga de planificar los hilos entrenadores
void MODELO_PLANIFICAR();
void team_planificar(); //nueva implementacion

//Hilo que se encarga de recibir los mensajes
void team_recibir_mensajes(t_list* mensajes);

//Hilo que se encarga de reaccionar a cada mensaje recibido
void team_procesar_mensajes();

//inicializar
pthread_t* inicializar_hilos_entrenadores(int*cantidad);
