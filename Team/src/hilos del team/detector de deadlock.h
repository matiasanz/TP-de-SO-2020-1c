# ifndef _HILO_DETECTOR_DEADLOCK_
# define _HILO_DETECTOR_DEADLOCK_
//
#include "hilos_team.h"
//
///*TODO
// * testear y ya pasar al hilo
// * cachear a los entrenadores en una estructura que tenga los recursos que necesita, los que le sobran y el id, para no tener que repetir varias veces las mismas cuentas y sacar algunos destroy
// * cambiar los nombres de las funciones para que sean mas acordes a los fines
// * En teoria los entrenadores que retornan, llegan en orden de ejecucion, con lo cual con que yo remueva dos en orden deberian poder intercambiar
// */

bool algoritmo_detectar_deadlock(entrenadores candidatos, entrenadores enDeadlock);

void algoritmo_procesar_deadlock(entrenadores entrenadoresEnDeadlock);

bool hay_deadlock_exceptuando_al_siguiente(entrenadores unEquipo, matriz_recursos necesidad, matriz_recursos disponibles, entrenadores omitidos, entrenadores enDeadlock);

bool hay_deadlock_dejando_al_siguiente(entrenadores unEquipo, matriz_recursos necesidad, matriz_recursos disponibles, entrenadores omitidos, entrenadores enDeadlock);

bool hay_espera_circular(entrenadores unEquipo, matriz_recursos necesidad,matriz_recursos disponibles);

matriz_recursos entrenador_recursos_sobrantes(entrenador*);

matriz_recursos entrenador_recursos_pedidos(entrenador*);

matriz_recursos entrenadores_pedidos_insatisfechos(entrenadores unEquipo);

matriz_recursos entrenadores_recursos_sobrantes(entrenadores unEquipo);

#endif
