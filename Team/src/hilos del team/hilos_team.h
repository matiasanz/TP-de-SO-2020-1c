#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

//Semaforos

//pthread_mutex =
//sem_t unContador =

//TODO definir aqui...

/*----------------*/

//Hilo correspondiente a un entrenador
void team_hilo_entrenador();

//Hilo que se encarga de planificar los hilos entrenadores
void MODELO_PLANIFICAR();
void team_planificar(); //nueva implementacion

//Hilo que se encarga de reaccionar a cada mensaje recibido
void team_procesar_mensajes();
