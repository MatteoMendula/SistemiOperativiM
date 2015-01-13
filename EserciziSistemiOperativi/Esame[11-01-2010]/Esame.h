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
#define CAPIENZA_SALA1 20
#define CAPIENZA_SALA2 30
#define MAX_FUMATORI 20
#define MAX_NON_FUMATORI 40
#define SALA1 0
#define SALA2 1

//DICHIARAZIONI MONITOR
typedef struct
{
    pthread_mutex_t lock;
    pthread_cond_t fumatori;
    pthread_cond_t non_fumatori;
    
    int posti_occupati_sala1_non_fumatori; //solo non fumatori
    int posti_occupati_sala2_fumatori;
    int posti_occupati_sala2_non_fumatori;
    
    int fumatori_in_attesa;
    int non_fumatori_in_attesa;
    
} Ristorante;

//DICHIARAZIONI VARIABILI
pthread_t fumatori[MAX_FUMATORI];
pthread_t non_fumatori[MAX_NON_FUMATORI];
Ristorante ristorante;

//FUNZIONI
void init_monitor(Ristorante *);

//FUNZIONI LOGICHE
void fumatori_in_entrata(Ristorante *);
void fumatori_in_uscita(Ristorante *);
void non_fumatori_in_entrata(Ristorante *, int *);
void non_fumatori_in_uscita(Ristorante *, int *);

//THREADS
void * thread_fumatori(void *);
void * thread_non_fumatori(void *);

#endif
