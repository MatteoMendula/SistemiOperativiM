/*
* Si vuole affidare il calcolo del prodotto scalare di due
* vettori A e B di dimensione N ad un insieme M thread
* concorrenti (si supponga che N sia multiplo intero di
* M, cioè N=KM).
* Ogni thread Ti si dovrà occupare del calcolo del prodotto
* scalare tra due sottovettori di dimensione K data (M=N/K).
* In altre parole, Ti dovrà calcolare il valore
* ai.bi+ai+1.bi+1+ ... +ai+k-1.bi+k-1
* Al termine del calcolo, ogni thread Ti dovrà aggiungere il
* valore calcolato alla variabile condivisa RIS[nella quale viene
* quindi accumulato il risultato finale A.B]; inoltre, per
* registrare l’ordine di aggiornamento della variabile, Ti dovrà
* scrivere il proprio identificatore nella prima posizione libera
* di un vettore LOG di M elementi.

* Il thread iniziale dovrà quindi:
* – Inizializzare i vettori A e B con N valori casuali;
* – Creare gli M thread concorrenti;
* – Attendere la terminazione dei thead e stampare
* il valore di RIS e il contenuto di LOG
*/


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

#define M 3 //numero thread
#define K 10 //elementi da moltiplicare dal K-esimo thread
#define N K*M //elementi dell'array generico
#define MAX_RAND 100

// Vettori di elementi A e B
int A[N], B[N], LOG[M];
int RIS;
pthread_mutex_t lock;

void * single_thread(void * t)
{
    int tid = (int) t;
    int start = tid * K; //Punto di partenza per fare calcoli
    int somma = 0;
    
    for(int i = start; i < start + K; i++)
        somma += A[i] * B[i];
    
    //Sezione critica
    
    pthread_mutex_lock(&lock);
    RIS+=somma;
    LOG[tid] = tid;
    pthread_mutex_unlock(&lock);
    
    //Fine sezione critica
    
    return NULL;
}

int main(int argc, const char * argv[])
{
    int i;
    
    void* status;
    int result;
    
    //threads
    pthread_t threads[M];
    
    
    printf("Thread principale avviato con valori: M = %d, K = %d, N = %d\n", M, K, N);
    
    //inizializzo i vettori A e B
    srand((unsigned int)time(NULL));
    
    printf("Creo i vettori\n");
    for(i = 0; i < N; i++)
    {
        A[i] = rand() % MAX_RAND;
        B[i] = rand() % MAX_RAND;
    }
    
    //creazione mutex
    if(pthread_mutex_init(&lock, NULL)!=0)
    {
        printf("Failed to create mutex\n");
        exit(-1);
    }

    //creazione threads
    for(i = 0; i < M; i++)
    {
        result = pthread_create(&threads[i], NULL, single_thread, (void *)(intptr_t) i);
        if(result)
        {
            printf("ERRORE codice: %d\n", result);
            exit(-1);
        }
    }
    
    //join e stampa
    for(i = 0; i < M; i++)
    {
        result = pthread_join(threads[i], &status);
        if(result)
            printf("Errore nel join del thread %d\n",i);
    }
    
    //distruzione del mutex
    pthread_mutex_destroy(&lock);
    
    printf("Contenuto di LOG: ");
    
    for(i = 0; i < M; i++)
        printf("LOG[%d] = %d, ", i, LOG[i]);
    
    printf("\nRisultato del prodotto scalare tra A e B = %d\n", RIS);
    return 0;
}
