/*
* La rete televisiva vuole utilizzare i risultati del sondaggio per
* stabilire quale dei K film interessati dalle domande del
* questionario mandare in onda, secondo le seguenti modalità.
* Ognuno degli N utenti ha un comportamento strutturato in due
* fasi consecutive:
*
*    1. Nella prima fase partecipa al sondaggio
*    2. Nella seconda fase vede il film risultato vincitore nel
*
* sondaggio (quello, cioè, con la valutazione massima).
* Si realizzi un’applicazione concorrente nella quale ogni thread
* rappresenti un diverso utente, che tenga conto dei vincoli
* dati e, in particolare, che ogni utente non possa eseguire la
* seconda fase (visione del film vincitore) se prima non si è
* conclusa la fase precedente (compilazione del questionario)
* per tutti gli utenti.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> //per sleep

#define K 10 //numero film
#define N 10 //numero persone
#define MAX_VOTE 10

typedef struct
{
    pthread_mutex_t fine_sondaggio;
    sem_t barrier;
    int sondaggi_terminati;
    
} Barriera; //sincronizzazione threads!

typedef struct
{
    pthread_mutex_t lock;
    int voti[N];
} Sondaggio;

Sondaggio sondaggi[K]; //un sondaggio per ogni film
Barriera b;
pthread_mutex_t film_lock;
pthread_t spettatori[N];
int winner;

int found_winner();
void init_sync();

int found_winner()
{
    int max = 0;
    
    //per ogni film
    for(int i = 0; i < K; i++)
    {
        int somma = 0;
        int avg = 0;
        for(int j = 0; j < N; j++)
        {
            somma += sondaggi[i].voti[j];
        }
        avg = somma / N;
        if(avg > max)
            max = avg;
    }
    return max;
}

void * spettatore(void * t)
{
    int tid = (unsigned int) t;
    
    //per ogni film
    for(int i = 0; i < K; i++)
    {
        //prima faccio il sondaggio
        pthread_mutex_lock(&sondaggi[i].lock);
        sondaggi[i].voti[tid] = rand() % MAX_VOTE;
        pthread_mutex_unlock(&sondaggi[i].lock);
    }
    
    //poi attendo la barriera
    pthread_mutex_lock(&b.fine_sondaggio);
    
    b.sondaggi_terminati++;
    printf("Sondaggio terminato...\n");
    
    pthread_mutex_unlock(&b.fine_sondaggio);
    
    if(b.sondaggi_terminati == N)
    {
        printf("Sondaggi terminati... calcolo vincitore...\n");
        
        //calcolo il vincitore e sblocco la barriera
        winner = found_winner();
        printf("The winner is %d\n", winner);
        
        //sblocco barriera
        printf("Sblocco la barriera\n");
        sem_post(&b.barrier);
    }
    
    //altrimenti mi fermo ad aspettare
    sem_wait(&b.barrier);
    
    //appena mi sblocco dico agli altri che possono andare
    sem_post(&b.barrier);

    //poi guardo il film
    pthread_mutex_lock(&film_lock);
    printf("Spettatore %d...... visione film %d in corso...\n", tid, winner);
    sleep(2);
    pthread_mutex_unlock(&film_lock);
    
    return NULL;
}

void init_sync()
{
    //inizializzo barriera
    pthread_mutex_init(&b.fine_sondaggio, NULL);
    sem_init(&b.barrier, 0, 0);
    b.sondaggi_terminati = 0;
    
    //inizializzo sondaggi
    for(int i = 0; i < K; i++)
    {
        pthread_mutex_init(&sondaggi[i].lock, NULL);
        for(int j = 0; j < N; j++)
            sondaggi[i].voti[j] = 0;
    }
    
    pthread_mutex_init(&film_lock, NULL);
    winner = -1;
}

int main(int argc, char * argv[])
{
    int result;
    void * status;
    printf("Inizializzazione\n");
    srand((unsigned int) time(NULL));
    
    //inizializzo i mutex e i semafori
    init_sync();
    
    //creazione threads
    for(int i = 0; i < N; i++)
    {
        result = pthread_create(&spettatori[i], NULL, spettatore, (void *)(intptr_t)i);
        if(result)
        {
            perror("Errore nella creazione dei thread\n");
            exit(-1);
        }
    }
    
    //attendo la terminazione di tutti i threads
    for(int i = 0; i < N; i++)
    {
        result = pthread_join(spettatori[i], &status);
        if(result)
        {
            perror("Errore nella terminazione dei threads\n");
            exit(-1);
        }
    }
    
    printf("Exit\n");
    return 0;
}
