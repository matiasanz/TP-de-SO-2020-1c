#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <commons/collections/list.h>

#include "../utils/mensajes.h"
#include "../dominio/estructuras-auxiliares/captura_pendiente.h"

#define PROCESO_ACTIVO 1

//Hilos
pthread_t* hilosEntrenadores;
pthread_t hiloReceptorDeMensajes;
pthread_t hiloPlanificador;
pthread_t hiloMensajesAppeard;
pthread_t hiloMensajesCAUGHT;
pthread_t hiloMensajesLOCALIZED;
pthread_t hiloProcesadorDePokemones;

//Semaforos

sem_t* EjecutarEntrenador;
sem_t EquipoNoPuedaCazarMas;
sem_t FinDeCiclo_CPU;
//sem_t HayTareasPendientes;
sem_t HayEntrenadoresDisponibles;
sem_t finDeIntercambio;
sem_t FinDePlanificacion;

pthread_mutex_t mutexHistorialEspecies;
pthread_mutex_t mutexEntrenadores;
pthread_mutex_t*mutexEstadoEntrenador;
pthread_mutex_t*mutexPosicionEntrenador;
//pthread_mutex_t mutexInventariosGlobales;
pthread_mutex_t mutexRecursosDisponibles;
pthread_mutex_t mutexPedidos;

//Variables globales
numero cantidadDeEntrenadores;

//HARDCODEOS ---------------------------------- ACORDARME DE BORRARRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
pthread_mutex_t mutex_PSEUDOGAMECARD;
bool FIN_PSEUDO_GAMECARD;

/*----------------*/

//Hilo correspondiente a un entrenador
void team_hilo_entrenador();

//Hilo que se encarga de planificar los hilos entrenadores
void team_planificar();

//Hilo que se encarga de recibir los mensajes
void gamecard_simulator();
void team_procesador_cola_CAUGHT(cr_list* mensajes);
void team_procesador_cola_LOCALIZED(cr_list*mensajes);
void team_procesar_pokemones();

//inicializar
void inicializar_hilos_entrenadores();

//finalizar
void finalizar_hilos_entrenadores();

//Entrenadores
void entrenador_despertar(entrenador*unEntrenador, char* motivo);
void entrenadores_despertar_por_llegada_de(entrenadores, pokemon*);
void entrenador_dormir_hasta_llegada_de_pokemon(entrenador*);
bool entrenador_dormido_hasta_llegada_de_pokemon(entrenador*unEntrenador);
bool entrenador_verificar_objetivos(entrenador*unEntrenador);
void entrenador_dar_un_paso_hacia(entrenador*unEntrenador, t_posicion posicionFinal);
void desplazar_unidimensional(coordenada* posicionInicial, coordenada posicionFinal);

//Pokemones
void registrar_pokemon(pokemon*);
void registrar_en_cada_posicion(especie_pokemon, posiciones);
bool pokemon_es_requerido(pokemon unPokemon);
bool pokemon_en_tramite(pokemon*unPokemon);
pokemon* pokemones_esperar_y_leer();

//Objetivos
numero objetivos_cantidad_bruta_requerida_de(especie_pokemon);
numero objetivos_cantidad_requerida_de(especie_pokemon unaEspecie);
void objetivos_actualizar_por_captura_de(especie_pokemon);
void validar_captura(captura_pendiente*);

//Otras
pokemon*leer_pokemon(t_mensaje_appeared_pokemon*);
bool mensaje_appeared_responde_a_mi_pedido(t_mensaje_appeared_pokemon*);

bool mensaje_caught_pokemon_get_resultado(t_mensaje_caught_pokemon*);
void procesar_resultado_de_captura(captura_pendiente*, bool fueExitosa);
void captura_procesar_fallo(captura_pendiente*capturaFallida);

bool mensaje_localized_me_sirve(t_mensaje_localized_pokemon* mensaje);
bool mensaje_localized_es_para_mi(t_mensaje_localized_pokemon* unMensaje);
bool id_responde_a_mi_pedido(t_id idCorrelativo);

void posiciones_ordenar_por_cercania_al_equipo(posiciones);
numero posicion_distancia_a_equipo(t_posicion*);
numero posicion_distancia_a_entrenadores(t_posicion*, entrenadores);


