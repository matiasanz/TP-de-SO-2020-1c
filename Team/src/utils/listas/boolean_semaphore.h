# ifndef _BOOLEAN_SEMAPHORE_
# define _BOOLEAN_SEMAPHORE_

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct{
	bool flag;
	pthread_mutex_t mutex;
	sem_t endedAccesses;
	volatile uint8_t accesses;
}boolean_sem_t;

void boolean_sem_init(boolean_sem_t*);

void boolean_sem_open(boolean_sem_t*);

bool boolean_sem_consult(boolean_sem_t*);

void boolean_sem_turn_off(boolean_sem_t*);

void boolean_sem_close(boolean_sem_t*);

void boolean_sem_wait_end(boolean_sem_t*);

void boolean_sem_destroy(boolean_sem_t*);

# endif
