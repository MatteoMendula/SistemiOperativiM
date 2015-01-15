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
#define STANDARD_N 10
#define SPECIAL_N 5
#define MAX_PIASTRELLE_FORNO 30
#define N_LOTTO 5
#define T1 0
#define T2 1

//DICHIARAZIONI MONITOR

typedef struct
{
    int dimensione_lotto;
    int tempo_cottura;
    int tipo;
    
}Lotto;

typedef struct
{
    pthread_mutex_t lock;
    pthread_cond_t standard_coda;
    pthread_cond_t special_coda;
    
    int lotti_special_t1_in_coda;
    int lotti_standard_t1_in_coda;
    
    int lotti_special_t2_in_coda;
    int lotti_standard_t2_in_coda;
    
    int lotti_standard_t1_forno;
    int lotti_standard_t2_forno;
    int lotti_special_t1_forno;
    int lotti_special_t2_forno;
    
} Forno;

//DICHIARAZIONI VARIABILI
pthread_t lotti_standard[STANDARD_N];
pthread_t lotti_special[SPECIAL_N];
Forno forno;

//FUNZIONI
void init_forno(Forno *);
void entra_standard_t1(Forno *, Lotto *);
void entra_standard_t2(Forno *, Lotto *);

void entra_special_t1(Forno *, Lotto *);
void entra_special_t2(Forno *, Lotto *);

void esce_standard_t1(Forno *, Lotto *);
void esce_standard_t2(Forno *, Lotto *);

void esce_special_t1(Forno *, Lotto *);
void esce_special_t2(Forno *, Lotto *);

void risveglio(Forno *);

int totale_lotti_in_forno(Forno *);

//THREADS
void * lotto_standard(void *);
void * lotto_speciale(void *);

#endif
