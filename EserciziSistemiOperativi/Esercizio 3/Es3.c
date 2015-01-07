/*
* Una rete televisiva vuole realizzare un sondaggio di opinione su un campione
* di N persone riguardante il gradimento di K film.
* Il sondaggio richiede che ogni persona interpellata risponda a K domande,
* ognuna relativa ad un diverso film: in particolare, ad ogni domanda l’utente
* deve fornire una risposta (appartenente al dominio [1,..10]) che esprime il
* voto assegnato dall’utente al film in questione.
*
* La raccolta delle risposte avviene in modo tale che, al termine della
* compilazione di ogni questionario, vengano presentati i risultati parziali del
* sondaggio, e cioè: per ognuna delle k domande, venga stampato il voto medio
* ottenuto dal film ad essa associato.
*
* Al termine del sondaggio devono essere stampati i risultati definitivi, cioè il
* voto medio ottenuto da ciascun film ed il nome del film con il massimo
* punteggio.
*
* Si realizzi un’applicazione concorrente che, facendo uso della libreria
* pthread e rappresentando ogni singola persona del campione come un
* thread concorrente, realizzi il sondaggio rispettando le specifiche date.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>

#define N 2 //numero persone
#define K 4 //numero film
#define MAX_VOTO 20

pthread_t threads[N];

typedef struct
{
    pthread_mutex_t mutex; //per l'accesso mutuamente esclusivo
    
    int sondaggi[K][N]; //K film con N voti
    float avg[K]; //Media dei voti
    
    int n; //numero sondaggi effettuati

} Gestore;

Gestore g;

void * persona_thread(void * t)
{
    int tid = (int) t;
    pthread_mutex_lock(&g.mutex); //sezione critica
    for(int i=0; i < K; i++)
    {
        //effettuo il sondaggio
        g.n++;
        
        //per ogni film assegno il voto
        g.sondaggi[i][tid] = rand() % MAX_VOTO;
        
        //calcolo la media
        int somma = 0;
        for(int k = 0; k < N; k++)
        {
            somma += g.sondaggi[i][k];
        }
        g.avg[i] = somma / N;
        printf("Votazione(%d) per il film %d effettuata da %d, stato attuale del sondaggio per il film: %.2f\n", g.sondaggi[i][tid], i, tid, g.avg[i]);
    }
    pthread_mutex_unlock(&g.mutex); //fine sezione critica
    return NULL;
}

int main()
{
    int result;
    int status;
    
    //inizializzo il seed di randomizzazione
    srand((unsigned int) time(NULL));
    
    //inizializzo il mutex
    pthread_mutex_init(&g.mutex, NULL);
    
    //creazione threads
    printf("Creo %d persone\n", N);
    for(int i = 0; i < N; i++)
    {
        result = pthread_create(&threads[i], NULL, persona_thread, (void *)(intptr_t) i);
        if(result)
        {
            perror("Errore nella creazione di un thread");
            exit(-1);
        }
    }
    
    //attendo la terminazione di tutti i threads
    for(int i = 0; i < N; i++)
    {
        int result = pthread_join(threads[i], (void*) &status);
        if(result)
        {
            perror("Errore nella join");
            exit(-1);
        }
    }
    
    //trovo il film più votato
    int max = 0;
    int max_film = 0;
    for(int i = 0; i < K; i++)
    {
        printf("Voto medio per il film %d = %.2f\n", i, g.avg[i]);
        if(g.avg[i] > max)
        {
            max = g.avg[i];
            max_film = i;
        }
    }
    
    printf("Film con il punteggio più alto (%d)...... Rullo di tamburi..... %d\n", max, max_film);
    return 0;
}