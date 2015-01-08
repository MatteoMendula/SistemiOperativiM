/*
* Si consideri la pista di pattinaggio sul ghiaccio di una località turistica
* montana.
* La pista di pattinaggio è aperta a tutti.
* In particolare i clienti dell’impianto si suddividono in due categorie:
* principianti e esperti. Ogni gruppo di principianti, durante la permanenza
* all’interno della pista, viene accompagnato da un istruttore messo a
* disposizione dalla società che gestisce l’impianto; a questo proposito, si
* supponga che il numero totale di istruttori sia NI.
* I pattinatori entrano ed escono dalla pista a gruppi omogenei (ogni gruppo
* è formato solo da principanti o solo da esperti) e monolitici (ogni
* pattinatore fa parte dello stesso gruppo, sia in ingresso che in uscita),
* ognuno caratterizzato da una consistenza numerica data.
* La capacità della pista è limitata dal valore MAX, che esprime il numero
* massimo di pattinatori che possono essere in pista contemporaneamente (gli
*  istruttori non vengono conteggiati).
* Inoltre, il regolamento dell’impianto prevede che debba essere sempre
* rispettata la relazione:
* P ≥ E
* dove: P è il numero dei principianti in pista, e E è il numero degli esperti in
* pista. Si sviluppi un’applicazione concorrente in C/pthread che
* rappresenti i gruppi di pattinatori come thread concorrenti.
* In particolare, la soluzione deve implementare una politica
* di sincronizzazione dei thread che rispetti le specifiche
* date, ed inoltre i vincoli seguenti:
* • nell’accesso alla pista: i principianti abbiano la precedenza sugli esperti; a parità di categoria, si privilegino i gruppi meno numerosi.
* • nell’uscita dalla pista: gli esperti abbiano la precedenza sui principianti; a parità di categoria, si privilegino i gruppi più numerosi.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

#define MAX_GRUPPO 10
#define MAX_PISTA 20
#define N_GRUPPI 5

typedef struct
{
    pthread_mutex_t lock;
    
    //condizioni
    pthread_cond_t codaIngresso[2][MAX_GRUPPO]; // 2 categorie: principianti ed esperti[0 = esperti, 1 = principianti]
    pthread_cond_t codaUscita[MAX_GRUPPO];
    
    int sospIngresso[2][MAX_GRUPPO]; // 2 categorie
    int sospUscita[MAX_GRUPPO];
    
    int inPista[2];
    int istruttori; //NI
    
} Pista; //monitor

Pista pista;
pthread_t gruppi[N_GRUPPI];

void gruppoPrincipianti(int persone, int tid)
{
    printf("Creato un gruppo Principianti\n");
    
    /*
     * POLITICA INGRESSO PRINCIPIANTI
     * In entrata deve aspettare se:
     * - Pista piena
     * - Gruppo di principianti meno numeroso in attesa
     * - Principianti < Esperti in pista
    */
    
    
    
    /*
     * POLITICA USCITA PRINCIPIANTI
     * In uscita deve aspettare se:
     * - Gruppi di esperti più numerosi
     * - Gruppo di esperti in uscita
     * - Principianti < Esperti in pista
     */
    
}

void gruppoEsperti(int persone, int tid)
{
    printf("Creato un gruppo Esperti\n");
    
    /*
     * POLITICA INGRESSO ESPERTI
     * In entrata deve aspettare se:
     * - Pista piena
     * - Gruppo di principianti in attesa di entrare
     * - Gruppo di esperti meno numerosi
     * - Principianti < Esperti in pista
     */
    
    
    
    /*
     * POLITICA USCITA ESPERTI
     * In uscita deve aspettare se:
     * - Gruppi di esperti più numerosi
     * - Principianti < Esperti in pista
     */
    
}

void * thread(void * t)
{
    int tid = (unsigned int ) t;
    int n_persone = rand() % MAX_GRUPPO;
    
    if(rand() % 2 == 0)
        gruppoPrincipianti(n_persone, tid);
    else
        gruppoEsperti(n_persone, tid);
    
    pthread_exit(NULL);
}

int main(int argc, char * argv[])
{
    // init
    printf("Inizializzo strutture dati...\n");
 
    int result;
    void * status;
    
    pthread_mutex_init(&pista.lock, NULL);
    for(int i=0; i < MAX_GRUPPO; i++)
    {
        pthread_cond_init(&pista.codaIngresso[0][i], NULL);
        pthread_cond_init(&pista.codaIngresso[1][i], NULL);
        pthread_cond_init(&pista.codaUscita[i], NULL);
        
        pista.sospIngresso[0][i] = 0;
        pista.sospIngresso[1][i] = 0;
        pista.sospUscita[i] = 0;
    }
    
    pista.inPista[0] = 0;
    pista.inPista[1] = 0;
    pista.istruttori = 0;
    
    srand((unsigned int) time(NULL));
    
    printf("Strutture dati inizializzate.\n");
    
    //creazione threads
    for(int i = 0; i < N_GRUPPI; i++)
    {
        result = pthread_create(&gruppi[i], NULL, thread, (void *)(intptr_t) i);
        if(result)
        {
            perror("Errore nella creazione dei gruppi\n");
            exit(-1);
        }
    }
    
    //join
    for(int i = 0; i < N_GRUPPI; i++)
    {
        result = pthread_join(gruppi[i], &status);
        if(result)
        {
            perror("Errore nella creazione dei gruppi\n");
            exit(-1);
        }
    }
    
    printf("Fine pattinata.\n");
    
    return 0;
}
