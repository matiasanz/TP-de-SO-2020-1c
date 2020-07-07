#include "hilos_team.h"
#include "../team.h"
#include "../dominio/header_global_team.h"

void team_procesar_pokemones(){
	while(PROCESO_ACTIVO){

		sem_wait(&HayEntrenadoresDisponibles);
		pokemon*unPokemon = pokemones_esperar_y_leer();

		pthread_mutex_lock(&mutexRecursosDisponibles);
		if(pokemon_es_requerido(*unPokemon)){
			recursos_agregar_recurso(recursosEnMapa, unPokemon->especie);
			pthread_mutex_unlock(&mutexRecursosDisponibles);
			entrenadores_despertar_por_llegada_de(equipo, unPokemon);
		}

		else{

			if(pokemon_en_tramite(unPokemon)){
				pthread_mutex_unlock(&mutexRecursosDisponibles);



				pthread_mutex_lock(&mutexRepuestos);
				list_add(pokemonesDeRepuesto, unPokemon);
				log_event_pokemon_mapeado(unPokemon->especie);
				pthread_mutex_unlock(&mutexRepuestos);
			}

			else{
				log_event_loggear_situacion_actual(objetivosGlobales, inventariosGlobales);
				pthread_mutex_unlock(&mutexRecursosDisponibles);

				log_event_loggear_pokemon_descartado(unPokemon->especie);

				pokemon_destroy_hard(unPokemon);
			}

			sem_post(&HayEntrenadoresDisponibles);
		}
	}
}


/*************************************** Funciones Auxiliares ************************************************/

pokemon* pokemones_esperar_y_leer(){
	return cr_list_wait_and_remove(pokemonesRecibidos, 0);
}

bool pokemon_en_tramite(pokemon*unPokemon){
	return recursos_cantidad_de_instancias_de(recursosEnMapa, unPokemon->especie);
}

//Pasa a ready un entrenador
void entrenador_despertar(entrenador*unEntrenador, char* motivo){
	entrenador_pasar_a(unEntrenador, READY, motivo);
	cr_list_add_and_signal(entrenadoresReady, unEntrenador);
}

// Pasa a ready al entrenador que este mas cerca del pokemon, de los que estan dormidos a la espera
void entrenadores_despertar_por_llegada_de(entrenadores unEquipo, pokemon* unPokemon){

	pthread_mutex_lock(&mutexEntrenadores);
	entrenadores dormidos = list_filter(unEquipo, (bool(*)(void*)) &entrenador_dormido_hasta_llegada_de_pokemon);
	pthread_mutex_unlock(&mutexEntrenadores);

	entrenador*unEntrenador = entrenadores_mas_cercano(dormidos, unPokemon->posicion);

	if(!unEntrenador){
		error_show("No hay entrenadores");
		exit(1);
	}

	unEntrenador->proximaPresa = unPokemon;
	entrenador_despertar(unEntrenador, "Acaba de llegar un pokemon que puede cazar");

	entrenadores_disolver_equipo(dormidos);
}

// Retorna true si el entrenador se encuentra a la espera de la llegada de un pokemon
bool entrenador_dormido_hasta_llegada_de_pokemon(entrenador*unEntrenador){
	pthread_mutex_lock(&mutexEstadoEntrenador[unEntrenador->id]);
	bool estaDormido = entrenador_en_estado(unEntrenador, LOCKED_HASTA_APPEARED) || entrenador_en_estado(unEntrenador, NEW);
	pthread_mutex_unlock(&mutexEstadoEntrenador[unEntrenador->id]);
	return estaDormido;
}

bool entrenador_en_estado(entrenador* unEntrenador, t_estado ESTADO){
	return unEntrenador->estado == ESTADO;
}

bool pokemon_es_requerido(pokemon unPokemon){
	return objetivos_cantidad_requerida_de(unPokemon.especie);
}

numero objetivos_cantidad_bruta_requerida_de(especie_pokemon unaEspecie){
	numero instanciasTotales = recursos_cantidad_de_instancias_de(objetivosGlobales, unaEspecie);
	numero instanciasDisponibles = recursos_cantidad_de_instancias_de(inventariosGlobales, unaEspecie);

	return instanciasTotales - instanciasDisponibles;
}

numero objetivos_cantidad_requerida_de(especie_pokemon unaEspecie){

	numero instanciasBrutas = objetivos_cantidad_bruta_requerida_de(unaEspecie);
	numero instanciasEnMapa = recursos_cantidad_de_instancias_de(recursosEnMapa, unaEspecie);

	return instanciasBrutas - instanciasEnMapa;
}
