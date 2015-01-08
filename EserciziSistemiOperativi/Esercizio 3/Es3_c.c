// NON FINITO!!
/*
* Si aggiunga al problema dell’esercizio 3b i seguenti vincoli:
* possono assistere alla visione del film vincitore:
*
*   • Sia le persone che hanno partecipato al sondaggio;
*   • Sia qualunque altro utente della rete televisiva.
*
* In particolare, si supponga che:
*
*   • il film sia accessibile soltanto in modalità streaming tramite il portale web della rete
*   • che vi sia un limite massimo MAXC al numero degli utenti che possono connettersi per la visione del film.
*
* Estendere la soluzione dell’esercizio 3b in modo tale da soddisfare i requisiti di cui sopra.
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
#define MAXC 5 //numero massimo di persone che possono guardare un film contemporaneamente

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


int main(int argc, char * argv[])
{
    //DA FARE
    return 0;
}