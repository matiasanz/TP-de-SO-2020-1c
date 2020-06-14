#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <commons/collections/list.h>

#include "../estructuras-auxiliares/captura_pendiente.h"

#define PROCESO_ACTIVO 1


//Hilos
pthread_t* hilosEntrenadores;
pthread_t hiloReceptorDeMensajes;
pthread_t hiloPlanificador;
pthread_t hiloMensajesAppeard;
pthread_t hiloMensajesCAUGHT;
pthread_t hiloMensajesLOCALIZED;

//Semaforos

sem_t* EjecutarEntrenador;
sem_t EquipoNoPuedaCazarMas;
sem_t FinDeCiclo_CPU;
sem_t HayTareasPendientes;
sem_t HayEntrenadoresDisponibles;
sem_t finDeIntercambio;

pthread_mutex_t mutexHistorialEspecies;
pthread_mutex_t mutexEntrenadores;
pthread_mutex_t*mutexEstadoEntrenador;
pthread_mutex_t*mutexPosicionEntrenador;
pthread_mutex_t mutexInventariosGlobales;
pthread_mutex_t mutexRecursosEnMapa;

//Variables globales
numero cantidadDeEntrenadores; //Me guarda el tamaño del array para cuando tenga que finalizar

//HARDCODEOS ---------------------------------- ACORDARME DE BORRARRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
pthread_mutex_t MUTEX_FIN_DE_PROCESO_Para_que_pseudo_broker_deje_de_mandarme_mensajes_BORRAR;
bool finDeProceso;

/*----------------*/

//Hilo correspondiente a un entrenador
void team_hilo_entrenador();

//Hilo que se encarga de planificar los hilos entrenadores
void team_planificar();

//Hilo que se encarga de recibir los mensajes
void broker_simulator();
void team_suscriptor_cola_APPEARD(cr_list* mensajes);
void team_procesador_cola_CAUGHT(cr_list* mensajes);
void team_procesador_cola_LOCALIZED(cr_list*mensajes);

//Hilo que se encarga de reaccionar a cada mensaje recibido
void team_procesar_mensajes();

//inicializar
void inicializar_hilos_entrenadores();

//finalizar
void finalizar_hilos_entrenadores();

//Planificador
void equipo_despertar_en_caso_de_APPEARED();
bool entrenador_dormido_hasta_APPEARED(entrenador*);
void entrenadores_despertar_para_catch(entrenadores, pokemon*);

//auxiliares
void registrar_pokemon(pokemon*);
void registrar_en_cada_posicion(especie_pokemon, posiciones);
bool pokemon_es_requerido(pokemon unPokemon);
void mapa_mapear_requerido(pokemon*);
bool captura_sigue_siendo_requerida(captura_pendiente*);
numero objetivos_cantidad_bruta_requerida_de(especie_pokemon);
numero objetivos_cantidad_requerida_de(especie_pokemon unaEspecie);
void objetivos_actualizar_por_captura_de(especie_pokemon);
void posiciones_ordenar_por_cercania_al_equipo(posiciones);
numero posicion_distancia_a_equipo(t_posicion*);
numero posicion_distancia_a_entrenadores(t_posicion*, entrenadores);
void entrenador_bloquear_hasta_APPEARED(entrenador*);
