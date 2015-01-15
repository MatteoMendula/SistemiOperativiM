/*
 * Si consideri una pista di go-kart aperta a due tipi di clienti: Maggiorenni (MA) o
 * Minorenni (MI).
 * Presso la pista sono disponibili go-kart di 2 clindrate: C1 (125cc) e C2 (50cc).
 * In particolare:
 *
 * • I go-kart di cilindrata C1 possono essere utilizzati solo da clienti MA; il
 *   numero di go kart C1 a disposizione del noleggio è N1; ogni go kart di
 *   cilindrata C1 può essere utilizzato da una sola persona maggiorenne.
 * • I go-kart di cilindrata C2 possono essere utilizzati sia da clienti MA che da
 *   clienti MI; il numero di go kart C2 a disposizione del noleggio è N2; ogni go
 *   kart di cilindrata C2 può essere utilizzato da una sola persona.
 *
 * I clienti accedono alla pista singolarmente: ogni cliente richiede un go-kart; una volta
 * ottenuto, lo usa per un tempo arbitrario all’interno della pista; al termine restituisce il
 * go-kart ed esce.
 * Il gestore del noleggio ha stabilito la seguente politica di allocazione dei go kart:
 * 
 * • la richiesta da parte di un MA può essere servita se è disponibile un go kart
 *   tipo C1, oppure, se non ci sono go kart C1, se è disponibile un go kart di tipo C2.
 * • la richiesta di un MI può essere servita se è disponibile un go kart C2.
 *
 * Inoltre, motivi di sicurezza, ad ogni istante i go-kart allocati devono essere tutti dello
 * stesso tipo: la presenza in pista di uno o più go kart di tipo C1 impedisce l’accesso a
 * go kart di tipo C2; analogamente, la presenza in pista di go kart di tipo C2 impedisce
 * l’accesso a go kart di tipo C1.
 * Si sviluppi un’applicazione concorrente in C/pthread basata sul concetto di Monitor,
 * che rappresenti clienti della pista come thread concorrenti.
 * In particolare, la soluzione deve realizzare una politica di sincronizzazione dei thread
 * che rispetti le specifiche date, ed inoltre i vincoli seguenti:
 * • nell’acquisizione dei go-kart i thread MA abbiano la precedenza sui MI
 */

#include "20100111.h"

void * cliente_maggiorenne(void * t)
{
    //dobbiamo tenere traccia del go kart assegnato
    int go_kart;
    
    maggiorenne_noleggia_go_kart(&pista, &go_kart);
    sleep(2);
    maggiorenne_restituisce_go_kart(&pista, &go_kart);
    
    pthread_exit(NULL);
}

void * cliente_minorenne(void * t){
    
    minorenne_noleggia_go_kart(&pista);
    sleep(2);
    minorenne_restituisce_go_kart(&pista);
    
    pthread_exit(NULL);
}

void minorenne_noleggia_go_kart(Pista * p)
{
    pthread_mutex_lock(&p->lock);
    //condizione di sospensione
    while(p->maggiorenni_in_attesa > 0 || p->c1_in_pista_maggiorenni > 0 || ((p->c2_in_pista_maggiorenni + p->c2_in_pista_minorenni) == N_GO_KART_C2))
    {
        p->minorenni_in_attesa++;
         printf("Minorenne in attesa... Code: maggiorenni = %d, minorenni = %d\n", p->maggiorenni_in_attesa, p->minorenni_in_attesa);
        pthread_cond_wait(&p->coda_minorenni, &p->lock);
        p->minorenni_in_attesa--;
        
        printf("Minorenne risvegliato... Code: maggiorenni = %d, minorenni = %d\n", p->maggiorenni_in_attesa, p->minorenni_in_attesa);
    }
    
    p->c2_in_pista_minorenni++;
    printf("Minorenne in pista... Code: maggiorenni = %d, minorenni = %d\n", p->maggiorenni_in_attesa, p->minorenni_in_attesa);
    
    //risvegli
    if(p->maggiorenni_in_attesa > 0)
        pthread_cond_signal(&p->coda_maggiorenni);
    else if(p->minorenni_in_attesa > 0)
        pthread_cond_signal(&p->coda_minorenni);
    
    pthread_mutex_unlock(&p->lock);
    
}

void minorenne_restituisce_go_kart(Pista * p)
{
    pthread_mutex_lock(&p->lock);
    
    p->c2_in_pista_minorenni--;
    printf("Minorenne uscito dalla pista... Code: maggiorenni = %d, minorenni = %d\n", p->maggiorenni_in_attesa, p->minorenni_in_attesa);
    
    if(p->maggiorenni_in_attesa > 0)
        pthread_cond_signal(&p->coda_maggiorenni);
    else if(p->minorenni_in_attesa > 0)
        pthread_cond_signal(&p->coda_minorenni);
    
    pthread_mutex_unlock(&p->lock);
}

void maggiorenne_restituisce_go_kart(Pista * p, int * go_kart)
{
    pthread_mutex_lock(&p->lock);
    
    if(*go_kart == C1)
        p->c1_in_pista_maggiorenni--;
    else if(*go_kart == C2)
        p->c2_in_pista_maggiorenni--;
    
    printf("Maggiorenne esce dalla pista... Code: maggiorenni = %d, minorenni = %d\n", p->maggiorenni_in_attesa, p->minorenni_in_attesa);
    
    //risvegli
    if(p->maggiorenni_in_attesa > 0)
        pthread_cond_signal(&p->coda_maggiorenni);
    else if(p->minorenni_in_attesa > 0)
        pthread_cond_signal(&p->coda_minorenni);
    
    pthread_mutex_unlock(&p->lock);
}

void maggiorenne_noleggia_go_kart(Pista * p, int * go_kart)
{
    pthread_mutex_lock(&pista.lock);
    
    //se tutti i c1 sono in pista e i c2 sono tutti occupati mi sospendo
    while((p->c1_in_pista_maggiorenni == N_GO_KART_C1) || (p->c2_in_pista_maggiorenni + p->c2_in_pista_minorenni == N_GO_KART_C2 ))
    {
        //mi sospendo
        p->maggiorenni_in_attesa++;
        printf("Maggiorenni sospesi in coda... Code: maggiorenni = %d, minorenni = %d\n", p->maggiorenni_in_attesa, p->minorenni_in_attesa);
        pthread_cond_wait(&p->coda_maggiorenni, &p->lock);
        p->maggiorenni_in_attesa--;
        
        printf("Maggiorenni risvegliati... Code: maggiorenni = %d, minorenni = %d\n", p->maggiorenni_in_attesa, p->minorenni_in_attesa);
    }
    
    //maggiorenni entrati... cilindrata?
    if(p->c1_in_pista_maggiorenni < N_GO_KART_C1)
    {
        //prendo un C1
        p->c1_in_pista_maggiorenni++;
        *go_kart = C1;
    }
    else
    {
        //prendo un C2
        p->c2_in_pista_maggiorenni++;
        *go_kart = C2;
    }
    
    printf("Maggiorenne in pista... Code: maggiorenni = %d, minorenni = %d\n", p->maggiorenni_in_attesa, p->minorenni_in_attesa);
    
    //risvegli
    if(p->maggiorenni_in_attesa > 0)
        pthread_cond_signal(&p->coda_maggiorenni);
    else if(p->minorenni_in_attesa > 0)
        pthread_cond_signal(&p->coda_minorenni);
    
    pthread_mutex_unlock(&pista.lock);
}



void init_pista(Pista * pista)
{
    pthread_mutex_init(&pista->lock, NULL);
    pthread_cond_init(&pista->coda_minorenni, NULL);
    pthread_cond_init(&pista->coda_maggiorenni, NULL);
    
    pista->c1_in_pista_maggiorenni = 0;
    pista->c2_in_pista_maggiorenni = 0;
    pista->c2_in_pista_minorenni = 0;
    pista->minorenni_in_attesa = 0;
    pista->maggiorenni_in_attesa = 0;
}

int main(int argc, char *  argv[])
{
    int result;
    void * status;
    
    printf("Programma avviato.\n");
    
    init_pista(&pista);
    
    for(int i = 0; i < N_MAGGIORENNI; i++)
    {
        result = pthread_create(&maggiorenni[i], NULL, cliente_maggiorenne, (void *)(intptr_t) i);
        if(result)
        {
            printf("Errore nella creazione del thread %d con stato %d\n", i, result);
            exit(-1);
        }
    }
    
    for(int i = 0; i < N_MINORENNI; i++)
    {
        result = pthread_create(&minorenni[i], NULL, cliente_minorenne, (void *)(intptr_t) i);
        if(result)
        {
            printf("Errore nella creazione del thread %d con stato %d\n", i, result);
            exit(-1);
        }
    }
    
    //join dei threads
    for(int i = 0; i < N_MAGGIORENNI; i++)
    {
        result = pthread_join(maggiorenni[i], status);
        if(result)
        {
            printf("Errore nel join del thread %d con stato %d e risultato %d\n", i, result, (int)(intptr_t)status);
        }
    }
    
    for(int i = 0; i < N_MINORENNI; i++)
    {
        result = pthread_join(minorenni[i], status);
        if(result)
        {
            printf("Errore nel join del thread %d con stato %d e risultato %d\n", i, result, (int)(intptr_t)status);
        }
    }
    
    printf("Fine del programma.\n");
}