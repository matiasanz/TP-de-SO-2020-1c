#include "boolean_semaphore.h"

void boolean_sem_init(boolean_sem_t* semaforo){
	semaforo->flag=1;

	pthread_mutex_init(&semaforo->mutex, NULL);
//	sem_init(&semaforo->endedAccesses, 0, 0);
//	semaforo->accesses=0;

//	puts("\n\n*** PRENDO SEMAFORO ***\n\n");
}

void boolean_sem_open(boolean_sem_t* semaforo){
	semaforo->accesses++;
//	puts("\n\n*** Tengo otro acceso ***\n\n");
//
//	if(boolean_sem_consult(semaforo)){
//		puts("Esta prendido");
//	}
//	else{
//		puts("\n\npero estaba apagado :(\n");
//	}
}

bool boolean_sem_consult(boolean_sem_t* semaforo){
	pthread_mutex_lock(&semaforo->mutex);
	bool flag = semaforo->flag;
	pthread_mutex_unlock(&semaforo->mutex);

//	puts("\n\n*** CONSULTO SEMAFORO ***\n\n");
//
//	printf("***** Estado flag = %d\n", flag);

	return flag;
}

void boolean_sem_turn_off(boolean_sem_t* semaforo){
	pthread_mutex_lock(&semaforo->mutex);
	semaforo->flag=false;
	pthread_mutex_unlock(&semaforo->mutex);
//	puts("\n\n*** APAGO SEMAFORO ***\n\n");
}

//void boolean_sem_close(boolean_sem_t* semaforo){
////	puts("\n\n*** CORTO ACCESO ***\n\n");
//
//	sem_post(&semaforo->endedAccesses);
//
//
////	if(!boolean_sem_consult(semaforo)){
////		puts("\n\nigual esta apagado **\n");
////	}
//}

//void boolean_sem_wait_end(boolean_sem_t* semaforo){
////	puts("\n\n*** ESPERO QUE TERMINE SEMAFORO ***\n\n");
//
//	int accesos = semaforo->accesses;
//	while(accesos--){
//		sem_wait(&semaforo->endedAccesses);
//		semaforo->accesses--;
//	}
//
//}

void boolean_sem_destroy(boolean_sem_t* semaforo){
//	sem_destroy(&semaforo->endedAccesses);
	pthread_mutex_destroy(&semaforo->mutex);

//	puts("\n\n*** MATE AL SEMAFORO ***\n\n");
}
