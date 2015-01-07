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

#define M 2 //numero thread
#define K 5 //elementi da moltiplicare dal K-esimo thread
#define N K*M //elementi dell'array generico

// Vettori di elementi A e B
int A[N], B[N];

void * single_thread(void * t)
{
    int tid = (intptr_t) t; //thread-id
    int i; // indice
    int displ=0; // displacement
    int result=0; // risultato
    
    // Displacement = numero di partenza dove ogni thread deve partire per il calcolo del prodotto scalare dei due vettori
    displ = tid*K;
    
    // Calcolo il prodotto scalare della parte di competenza
    for(i=displ; i < displ+K; i++)
    {
        result+=(A[i]*B[i]);
    }
    
    pthread_exit((void*)(intptr_t) result);
}

int main(int argc, const char * argv[])
{

    return 0;
}
