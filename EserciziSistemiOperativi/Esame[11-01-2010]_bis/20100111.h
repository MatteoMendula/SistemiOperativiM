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
#define N_MAGGIORENNI 5
#define N_MINORENNI 10
#define N_GO_KART_C1 2
#define N_GO_KART_C2 3
#define C1 0
#define C2 1

//DICHIARAZIONI MONITOR

typedef struct
{
    pthread_mutex_t lock;
    pthread_cond_t coda_maggiorenni;
    pthread_cond_t coda_minorenni;
    
    int maggiorenni_in_attesa;
    int minorenni_in_attesa;
    int c1_in_pista_maggiorenni;
    int c2_in_pista_maggiorenni;
    int c2_in_pista_minorenni;
    
} Pista;

//DICHIARAZIONI VARIABILI
pthread_t maggiorenni[N_MAGGIORENNI];
pthread_t minorenni[N_MINORENNI];
Pista pista;

//FUNZIONI
void init_pista(Pista *);
void maggiorenne_noleggia_go_kart(Pista *, int *);
void maggiorenne_restituisce_go_kart(Pista *, int *);
void minorenne_noleggia_go_kart(Pista *);
void minorenne_restituisce_go_kart(Pista *);
void cliente_esce_da_pista(int eta);

//THREADS
void * cliente_maggiorenne(void *);
void * cliente_minorenne(void *);

#endif
