/*
 * Si calcoli il massimo in un insieme di valori interi
 * di N elementi, memorizzati in un vettore V.
 * Si vuole affidare la ricerca del massimo a un
 * insieme di M thread concorrenti, ognuno dei quali
 * si dovrà occupare della ricerca del massimo in una
 * porzione del vettore di dimensione K
 * data (M=N/K).
 * Il thread iniziale dovrà quindi:
 * –  Inizializzare il vettore V con N valori casuali;
 * –  Creare gli M thread concorrenti;
 * –  Ricercare il massimo tra gli M risultati ottenuti
 * dai thread e stamparne il valore.
 */

/*
 *
 * FUNZIONA!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

#define MAX_RND_N 1024
#define N 100 //numero elementi
#define K 10  //numero thread

int V[N];

//numero elementi da calcolare dall'i-esimo thread
int n_elementi[K];


//definizione thread

void * single_thread(void * t)
{
    int tid = (intptr_t) t; //ricavo il thread-id
    
    int i, start = 0;
    int result = 0;
    
    //calcolo punto di partenza per il singolo thread.
    for(i=0; i < tid; i++)
        start += n_elementi[i];
    
    //trovo il massimo
    for(i=0; i < n_elementi[tid]; i++)
    {
        if(V[start+i] > result)
            result = V[start+i];
    }
    
    pthread_exit((void*)(intptr_t) result);
}

int main(int argc, char* argv[])
{
    int i;
    int result;
    void* status;
    
    //threads
    pthread_t thread[K];
    
    printf("\nNumero elementi: %d, numero thread: %d, numero casuale massimo: %d\n\n", N, K, MAX_RND_N);
    
    //genero il vettore di numeri casuali
    srand(time(NULL));
    
    printf("Creazione vettore");
    for(i=0; i<N; i++)
    {
        V[i] = rand() % MAX_RND_N;
        if(i%(N/K)==0)
            printf("\n");
        printf("%d\t",V[i]);
    }
    printf("\n\n");
    
    for(i=0; i<K; i++)
        n_elementi[i] = N/K;
    
    int resto = N % K;
    
    //evito che ci sia resto in giro e assegno i rimanenti ai primi thread.
    if (resto != 0)
        for(i=0; i < resto; i++)
            n_elementi[i]++;
    
    //creazione thread e calcolo
    printf("Creo %d threads\n", K);
    for(i=0; i<K; i++)
    {
        result = pthread_create(&thread[i], NULL, single_thread, (void*)(intptr_t) i);
        
        //in caso di errore--> MUORI
        if (result)
        {
            printf("ERRORE: %d\n", result);
            exit(-1);
        }
    }
    
    //join e calcolo del massimo
    int max = 0;
    for(i = 0; i < K; i++)
    {
        result = pthread_join(thread[i], &status);
        if (result)
        {
            printf("ERRORE join thread %d codice %d\n", i, result);
        }
        else
        {
            printf("Finito thread %d con ris. %d\n", i, (int)(intptr_t)status);
            
            // Al termine di ogni thread, se ha trovato un massimo più alto, consideralo il massimo
            if((int)(intptr_t)status > max)
                max = (int)(intptr_t)status;
        }
    }
    
    //risultato
    printf("\nMaximum is: %d\n", max);
    
    return 0;
}