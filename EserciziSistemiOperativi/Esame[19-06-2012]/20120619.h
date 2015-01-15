#ifndef __EserciziSistemiOperativi__Template__Monitor__H
#define __EserciziSistemiOperativi__Template__Monitor__H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

//DEFINIZIONI
#define THREADS 10

//DICHIARAZIONI MONITOR

typedef struct
{
    pthread_mutex_t lock;
    pthread_cond_t coda1;
    pthread_cond_t coda2;
    
    int n_in_coda1;
    int n_in_coda2;
    
} Monitor;

//DICHIARAZIONI VARIABILI
pthread_t threads[THREADS];
Monitor monitor;

//FUNZIONI
void init_monitor(Monitor *);
void entra(Monitor *);
void esce(Monitor *);
void entra2(Monitor *, int *);
void esce2(Monitor *, int *);

//THREADS
void * thread1(void *);
void * thread2(void *);

#endif
