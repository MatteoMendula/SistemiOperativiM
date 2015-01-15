//
//  Template.c
//
//  Created by Marco Moschettini
//
//  Compilare con
//  gcc -D_REENTRANT -std=c99 -o Esame Esame.c -lpthread
//

#include "Template_Monitor.h"

void * thread1(void * t)
{
    entra(&monitor);
    sleep(2);
    esce(&monitor);
    pthread_exit(NULL);
}

void * thread2(void * t)
{
    //esempio con memoria
    int scelta;
    
    entra2(&monitor, &scelta);
    sleep(2);
    esce2(&monitor, &scelta);
    
    pthread_exit(NULL);
}

void entra(Monitor * m)
{
    pthread_mutex_lock(&m->lock);
    //condizione di blocco
    while(1)
    {
        m->n_in_coda1++
        pthread_cond_wait(&m->coda1);
        m->n_in_coda1--;
    }
    
    //cose da fare quando mi sblocco
    
    //risveglio secondo priorità
    if(n_in_coda1 > 0)
        pthread_cond_signal(&m->coda1);
    else if(m->n_in_coda2 > 0)
        pthread_cond_signal(&m->coda2);
    
    pthread_mutex_unlock(&m->lock);
}

void esce(Monitor * m)
{
    pthread_mutex_lock(&m->lock);
    
    //cose da fare
    
    //risveglio secondo priorità
    if(n_in_coda1 > 0)
        pthread_cond_signal(&m->coda1);
    else if(m->n_in_coda2 > 0)
        pthread_cond_signal(&m->coda2);
    
    pthread_mutex_unlock(&m->lock);
}

void entra2(Monitor *m, int * scelta)
{
    pthread_mutex_lock(&m->lock);
    //condizione di blocco
    while(1)
    {
        m->n_in_coda2++
        pthread_cond_wait(&m->coda2);
        m->n_in_coda2--;
    }
    
    //cose da fare quando mi sblocco
    
    //salvo la scelta! All'uscita se lo ricorderà
    if(1)
        *scelta = 1;
    else if(0)
        *scelta = 0;
    
    
    //risveglio secondo priorità (in questo caso priorità coda1)
    if(n_in_coda1 > 0)
        pthread_cond_signal(&m->coda1);
    else if(m->n_in_coda2 > 0)
        pthread_cond_signal(&m->coda2);
    
    pthread_mutex_unlock(&m->lock);
}

void esce2(Monitor * m, int * scelta)
{
    pthread_mutex_lock(&m->lock);
    
    //cose da fare
    if(*scelta == 1)
        printf("blabla");
    else if(*scelta == 2)
        printf("babab");
    
    //risveglio secondo priorità
    if(n_in_coda1 > 0)
        pthread_cond_signal(&m->coda1);
    else if(m->n_in_coda2 > 0)
        pthread_cond_signal(&m->coda2);
    
    pthread_mutex_unlock(&m->lock);
}


void init_monitor(Monitor * monitor)
{
    pthread_mutex_init(&monitor->lock, NULL);
    pthread_cond_init(&monitor->condition1, NULL);
    pthread_cond_init(&monitor->condition2, NULL);
    
    monitor->n = 0;
}

int main(int argc, char *  argv[])
{
    int result;
    void * status;
    
    printf("Programma avviato.\n");
    
    init_monitor(&monitor);
    
    for(int i = 0; i < THREADS; i++)
    {
        result = pthread_create(&threads[i], NULL, thread1, (void *)(intptr_t) i);
        if(result)
        {
            printf("Errore nella creazione del thread %d con stato %d\n", i, result);
            exit(-1);
        }
    }
    
    //avvio di ulteriori threads
    
    //join dei threads
    for(int i = 0; i < THREADS; i++)
    {
        result = pthread_join(threads[i], status);
        if(result)
        {
            printf("Errore nel join del thread %d con stato %d e risultato %d\n", i, result, (int)(intptr_t)status);
        }
    }
    
    printf("Fine del programma.\n");
}