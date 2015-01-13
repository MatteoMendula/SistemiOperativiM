/*
 * Un Ristorante è dotato di due sale da pranzo S1 e S2 nelle quali vengono accolti i clienti.
 * I clienti accedono al Ristorante a coppie di 2 persone.
 * Le coppie di clienti possono appartenere a 2 possibili categorie: Fumatori (F) o Non Fumatori (NF).
 * In particolare:
 *
 * • la sala S1 è dedicata solo a clienti NF; essa è attrezzata con N1 tavolini da 2 persone.
 * • La sala S2 può essere utilizzata sia da clienti F che NF. Essa è attrezzata con
 *
 * N2 tavolini da 2 persone. Per ovvii motivi, la presenza di clienti F impedisce a
 * clienti NF di accedere a S2, e viceversa.
 * Il gestore del Ristorante ha stabilito la seguente politica di controllo degli accessi:
 *
 * • Una coppia di NF otterrà un tavolo:
 *   1)In S1 se ci sono posti sufficienti nella sala S1;
 *   2)In S2 se non c’è posto in S1, se S2 non contiene clienti F e se c’è posto in S2.
 * • Una coppia di Fumatori entra nel Ristorante se nella sala S2 c’è posto, ed essa non contiene clienti NF.
 *
 * Si sviluppi un’applicazione concorrente in C/pthread basata sul concetto di Monitor,
 * che rappresenti le coppie di clienti del Ristorante come thread concorrenti.
 * In particolare, la soluzione deve realizzare una politica di sincronizzazione dei thread
 * che rispetti le specifiche date, ed inoltre i vincoli seguenti:
 * • nell’accesso al Ristorante le coppie NF abbiano la precedenza su quelle F.
 */

#include "Esame.h"


void fumatori_in_entrata(Ristorante * ristorante)
{
    pthread_mutex_lock(&ristorante->lock);
    
    //condizioni di sospensione in entrata
    while(ristorante->non_fumatori_in_attesa > 0 || ristorante->posti_occupati_sala2_non_fumatori > 0 || ristorante->posti_occupati_sala2_fumatori + ristorante->posti_occupati_sala2_non_fumatori < CAPIENZA_SALA2)
    {
        //mi sospendo
        ristorante->fumatori_in_attesa++;
        pthread_cond_wait(&(ristorante->fumatori), &(ristorante->lock));
        printf("Fumatori sospesi (sala1 liberi: %d - sala2 liberi: %d)\n",CAPIENZA_SALA1-ristorante->posti_occupati_sala1_non_fumatori, CAPIENZA_SALA2-(ristorante->posti_occupati_sala2_fumatori+ristorante->posti_occupati_sala2_non_fumatori));
        
        //risveglio
        ristorante->fumatori_in_attesa--;
        printf("Fumatori risvegliato (sala1 liberi: %d - sala2 liberi: %d)\n",CAPIENZA_SALA1-ristorante->posti_occupati_sala1_non_fumatori,CAPIENZA_SALA2-(ristorante->posti_occupati_sala2_fumatori + ristorante->posti_occupati_sala2_non_fumatori));
    }
    
    //prendo posto in sala
    ristorante->posti_occupati_sala2_fumatori++;
    printf("Fumatori entrati in S2 (sala1 liberi: %d - sala2 liberi: %d)\n",CAPIENZA_SALA1-ristorante->posti_occupati_sala1_non_fumatori,CAPIENZA_SALA2-(ristorante->posti_occupati_sala2_fumatori + ristorante->posti_occupati_sala2_non_fumatori));
    
    //risveglio un fumatore in fila
    if(ristorante->fumatori_in_attesa > 0)
        pthread_cond_signal(&(ristorante->fumatori));
    
    pthread_mutex_unlock(&ristorante->lock);
}

void fumatori_in_uscita(Ristorante * ristorante)
{
    pthread_mutex_lock(&ristorante->lock);
    
    ristorante->posti_occupati_sala2_fumatori--;
    printf("Fumatori usciti da S2 (sala1 liberi: %d - sala2 liberi: %d)\n",CAPIENZA_SALA1-ristorante->posti_occupati_sala1_non_fumatori,CAPIENZA_SALA2-(ristorante->posti_occupati_sala2_fumatori + ristorante->posti_occupati_sala2_non_fumatori));
    
    //in uscita do la precedenza ai non fumatori
    if(ristorante->non_fumatori_in_attesa > 0)
        pthread_cond_signal(&ristorante->non_fumatori);
    else
    {
        if(ristorante->fumatori_in_attesa > 0)
            pthread_cond_signal(&ristorante->fumatori);
    }
    
    pthread_mutex_unlock(&ristorante->lock);
}

void non_fumatori_in_entrata(Ristorante * ristorante, int * sala)
{
    pthread_mutex_lock(&ristorante->lock);
    
    //condizione di blocco
    while((ristorante->posti_occupati_sala1_non_fumatori == CAPIENZA_SALA1) && (ristorante->posti_occupati_sala2_fumatori > 0 || ristorante->posti_occupati_sala2_non_fumatori == CAPIENZA_SALA2))
    {
        printf("Non fumatori sospesi (sala1 liberi: %d - sala2 liberi: %d)\n",CAPIENZA_SALA1-ristorante->posti_occupati_sala1_non_fumatori,CAPIENZA_SALA2-(ristorante->posti_occupati_sala2_fumatori + ristorante->posti_occupati_sala2_non_fumatori));
        
        ristorante->non_fumatori_in_attesa++;
        pthread_cond_wait(&ristorante->non_fumatori, &ristorante->lock);
        ristorante->non_fumatori_in_attesa--;
        
        printf("Non fumatori risvegliati (sala1 liberi: %d - sala2 liberi: %d)\n",CAPIENZA_SALA1-ristorante->posti_occupati_sala1_non_fumatori,CAPIENZA_SALA2-(ristorante->posti_occupati_sala2_fumatori + ristorante->posti_occupati_sala2_non_fumatori));
    }
    
    //gruppo entrato! In quale sala li metto?
    if(ristorante->posti_occupati_sala1_non_fumatori < CAPIENZA_SALA1)
    {
        *sala = SALA1;
        ristorante->posti_occupati_sala1_non_fumatori++;
    }
    else
    {
        *sala = SALA2;
        ristorante->posti_occupati_sala2_non_fumatori++;
    }
    
    printf("Gruppo non fumatori entrato (sala1 liberi: %d - sala2 liberi: %d)\n",CAPIENZA_SALA1-ristorante->posti_occupati_sala1_non_fumatori,CAPIENZA_SALA2-(ristorante->posti_occupati_sala2_fumatori + ristorante->posti_occupati_sala2_non_fumatori));
    
    //risveglio gli altri in fila se ci sono (dando la precendeza ai non fumatori
    if(ristorante->non_fumatori_in_attesa > 0)
        pthread_cond_signal(&ristorante->non_fumatori);
    else if(ristorante->fumatori_in_attesa > 0)
        pthread_cond_signal(&ristorante->fumatori);
    
    
    pthread_mutex_unlock(&ristorante->lock);
}

void non_fumatori_in_uscita(Ristorante * ristorante, int * sala)
{
    pthread_mutex_lock(&ristorante->lock);
    
    if(*sala == SALA1)
    {
        ristorante->posti_occupati_sala1_non_fumatori--;
    }
    else if(*sala == SALA2)
    {
        ristorante->posti_occupati_sala2_non_fumatori--;
    }
    
    printf("Non fumatori usciti (sala1 liberi: %d - sala2 liberi: %d)\n",CAPIENZA_SALA1-ristorante->posti_occupati_sala1_non_fumatori,CAPIENZA_SALA2-(ristorante->posti_occupati_sala2_fumatori + ristorante->posti_occupati_sala2_non_fumatori));
    
    //libero le code
    
    if(ristorante->non_fumatori_in_attesa > 0)
        pthread_cond_signal(&ristorante->non_fumatori);
    else if(ristorante->fumatori_in_attesa > 0)
        pthread_cond_signal(&ristorante->fumatori);
    
    
    pthread_mutex_unlock(&ristorante->lock);
    
}

void * thread_fumatori(void * t)
{
    int tid = (int)(intptr_t) t;
    printf("Gruppo fumatori %d in entrata...\n", tid);
    sleep(2);
    
    fumatori_in_entrata(&ristorante);
    
    sleep(2);
    
    fumatori_in_uscita(&ristorante);
    
    printf("Gruppo fumatori %d in uscita...\n", tid);
    pthread_exit(NULL);
}

void * thread_non_fumatori(void * t)
{
    int sala;
    int tid = (int)(intptr_t) t;

    printf("Gruppo fumatori %d in entrata...\n", tid);
    sleep(1);
    
    non_fumatori_in_entrata(&ristorante, &sala);
    
    sleep(3);
    
    non_fumatori_in_uscita(&ristorante, &sala);
    
    pthread_exit(NULL);
}


void init_monitor(Ristorante * ristorante)
{
    
    pthread_mutex_init(&ristorante->lock, NULL);
    pthread_cond_init(&ristorante->fumatori, NULL);
    pthread_cond_init(&ristorante->non_fumatori, NULL);

    ristorante->posti_occupati_sala1_non_fumatori = 0;
    ristorante->posti_occupati_sala2_fumatori = 0;
    ristorante->posti_occupati_sala2_non_fumatori = 0;
    ristorante->fumatori_in_attesa = 0;
    ristorante->non_fumatori_in_attesa = 0;
}

int main(int argc, char *  argv[])
{
    int result;
    void * status;
    
    printf("Programma avviato.\n");
    
    init_monitor(&ristorante);
    
    for(int i = 0; i < MAX_FUMATORI; i++)
    {
        result = pthread_create(&fumatori[i], NULL, thread_fumatori, (void *)(intptr_t) i);
        if(result)
        {
            printf("Errore nella creazione del thread %d con stato %d\n", i, result);
            exit(-1);
        }
    }
    
    for(int i = 0; i < MAX_NON_FUMATORI; i++)
    {
        result = pthread_create(&non_fumatori[i], NULL, thread_non_fumatori, (void *)(intptr_t) i);
        if(result)
        {
            printf("Errore nella creazione del thread %d con stato %d\n", i, result);
            exit(-1);
        }
    }
    
    //avvio di ulteriori threads
    
    //join dei threads
    for(int i = 0; i < MAX_FUMATORI; i++)
    {
        result = pthread_join(fumatori[i], status);
        if(result)
        {
            printf("Errore nel join del thread %d con stato %d e risultato %d\n", i, result, (int)(intptr_t)status);
            exit(-1);
        }
    }
    
    for(int i = 0; i < MAX_NON_FUMATORI; i++)
    {
        result = pthread_join(non_fumatori[i], status);
        if(result)
        {
            printf("Errore nel join del thread %d con stato %d e risultato %d\n", i, result, (int)(intptr_t)status);
            exit(-1);
        }
    }
    
    printf("Fine del programma.\n");
}