//
//  Template.c
//
//  Created by Marco Moschettini
//
//  Compilare con
//  gcc -D_REENTRANT -std=c99 -o Esame Esame.c -lpthread
//

#include "20120619.h"

void * lotto_standard(void * t)
{
    Lotto lotto;
    lotto.dimensione_lotto = N_LOTTO;
    lotto.tempo_cottura = rand() % 5;
    lotto.tipo = rand()%2; //tipo T1 o T2
    
    switch(lotto.tipo)
    {
        case T1:
            entra_standard_t1(&forno, &lotto);
            sleep(lotto.tempo_cottura);
            esce_standard_t1(&forno, &lotto);
            break;
        case T2:
            entra_standard_t2(&forno, &lotto);
            sleep(lotto.tempo_cottura);
            esce_standard_t2(&forno, &lotto);
            break;
    }
    pthread_exit(NULL);
}

void * lotto_speciale(void * t)
{
    Lotto lotto;
    lotto.dimensione_lotto = N_LOTTO;
    lotto.tempo_cottura = rand() % 5;
    lotto.tipo = rand() % 2; //T1 o T2
    
    switch(lotto.tipo)
    {
        case T1:
            entra_special_t1(&forno, &lotto);
            sleep(lotto.tempo_cottura);
            esce_special_t1(&forno, &lotto);
            break;
        case T2:
            entra_special_t2(&forno, &lotto);
            sleep(lotto.tempo_cottura);
            esce_special_t2(&forno, &lotto);
            break;
    }
    pthread_exit(NULL);
}

int totale_lotti_in_forno(Forno * f)
{
    return f->lotti_special_t1_forno + f->lotti_special_t2_forno + f->lotti_standard_t1_forno + f->lotti_standard_t2_forno;
}

void risveglio(Forno * f)
{
    if(f->lotti_standard_t1_in_coda > 0 || f->lotti_standard_t2_in_coda > 0)
        pthread_cond_signal(&f->standard_coda);
    else if(f->lotti_special_t1_in_coda > 0 || f->lotti_special_t2_in_coda > 0)
        pthread_cond_signal(&f->special_coda);
    
}

void entra_standard_t1(Forno * f, Lotto * l)
{
    pthread_mutex_lock(&f->lock);
    
    //condizione di blocco
    // - se il forno è pieno
    // - se in forno ci sono l'altro tipo di piastrelle
    while((totale_lotti_in_forno(f) == MAX_PIASTRELLE_FORNO) || (f->lotti_special_t2_forno > 0) || (f->lotti_standard_t2_forno))
    {
        printf("[Lotto standard T1] Sospendo\n");
        f->lotti_standard_t1_in_coda++;
        pthread_cond_wait(&f->standard_coda, &f->lock);
        printf("[Lotto standard T1] Mi risveglio\n");
        f->lotti_standard_t1_in_coda--;
    }
    
    //cose da fare quando mi sblocco
    f->lotti_standard_t1_forno++;
    printf("[Lotto standard T1] Entrato in forno\n");

    //risveglio secondo priorità
    risveglio(f);
    
    printf("[STATO] %d special t1, %d special t2, %d standard t1, %d standard t2 in forno. %d special t1, %d special t2, %d standard t1, %d standard t2 in coda\n", f->lotti_special_t1_forno, f->lotti_special_t2_forno, f->lotti_standard_t1_forno, f->lotti_standard_t2_forno,f->lotti_special_t1_in_coda, f->lotti_special_t2_in_coda, f->lotti_standard_t1_in_coda, f->lotti_standard_t2_in_coda);
    pthread_mutex_unlock(&f->lock);
}

void entra_special_t1(Forno * f, Lotto * l)
{
    pthread_mutex_lock(&f->lock);
    
    //condizione di blocco
    // - se il forno è pieno
    // - se c'è in coda uno standard
    // - se in forno ci sono l'altro tipo di piastrelle
    while((totale_lotti_in_forno(f) == MAX_PIASTRELLE_FORNO) || (f->lotti_standard_t1_in_coda > 0) || (f->lotti_standard_t2_in_coda > 0) || (f->lotti_special_t2_forno > 0) || (f->lotti_standard_t2_forno))
    {
        printf("[Lotto special T1] Sospendo\n");
        f->lotti_special_t1_in_coda++;
        pthread_cond_wait(&f->standard_coda, &f->lock);
        printf("[Lotto special T1] Mi risveglio\n");
        f->lotti_standard_t1_in_coda--;
    }
    
    //cose da fare quando mi sblocco
    f->lotti_standard_t1_forno++;
    printf("[Lotto standard T1] Entrato in forno\n");
    
    risveglio(f);

    printf("[STATO] %d special t1, %d special t2, %d standard t1, %d standard t2 in forno. %d special t1, %d special t2, %d standard t1, %d standard t2 in coda\n", f->lotti_special_t1_forno, f->lotti_special_t2_forno, f->lotti_standard_t1_forno, f->lotti_standard_t2_forno,f->lotti_special_t1_in_coda, f->lotti_special_t2_in_coda, f->lotti_standard_t1_in_coda, f->lotti_standard_t2_in_coda);
    //risveglio secondo priorità
    pthread_mutex_unlock(&f->lock);
}


void esce_standard_t1(Forno * f, Lotto * l)
{
    pthread_mutex_lock(&f->lock);
    
    f->lotti_standard_t1_forno--;
    printf("[Lotto standard T1] Uscito dal forno\n");
    
    //risveglio secondo priorità
    risveglio(f);

    printf("[STATO] %d special t1, %d special t2, %d standard t1, %d standard t2 in forno. %d special t1, %d special t2, %d standard t1, %d standard t2 in coda\n", f->lotti_special_t1_forno, f->lotti_special_t2_forno, f->lotti_standard_t1_forno, f->lotti_standard_t2_forno,f->lotti_special_t1_in_coda, f->lotti_special_t2_in_coda, f->lotti_standard_t1_in_coda, f->lotti_standard_t2_in_coda);
    pthread_mutex_unlock(&f->lock);
}

void esce_special_t1(Forno * f, Lotto * l)
{
    pthread_mutex_lock(&f->lock);
    
    f->lotti_special_t1_forno--;
    printf("[Lotto special T1] Uscito dal forno\n");
    
    //risveglio secondo priorità
    risveglio(f);
    printf("[STATO] %d special t1, %d special t2, %d standard t1, %d standard t2 in forno. %d special t1, %d special t2, %d standard t1, %d standard t2 in coda\n", f->lotti_special_t1_forno, f->lotti_special_t2_forno, f->lotti_standard_t1_forno, f->lotti_standard_t2_forno,f->lotti_special_t1_in_coda, f->lotti_special_t2_in_coda, f->lotti_standard_t1_in_coda, f->lotti_standard_t2_in_coda);
    pthread_mutex_unlock(&f->lock);
}

void entra_standard_t2(Forno * f, Lotto * l)
{
    pthread_mutex_lock(&f->lock);
    
    //condizione di blocco
    // - se il forno è pieno
    // - se in forno ci sono l'altro tipo di piastrelle
    while((totale_lotti_in_forno(f) == MAX_PIASTRELLE_FORNO) || (f->lotti_special_t1_forno > 0) || (f->lotti_standard_t1_forno))
    {
        printf("[Lotto standard T2] Sospendo\n");
        f->lotti_standard_t2_in_coda++;
        pthread_cond_wait(&f->standard_coda, &f->lock);
        printf("[Lotto standard T2] Mi risveglio\n");
        f->lotti_standard_t2_in_coda--;
    }
    
    //cose da fare quando mi sblocco
    f->lotti_standard_t2_forno++;
    printf("[Lotto standard T1] Entrato in forno\n");
    
    printf("[STATO] %d special t1, %d special t2, %d standard t1, %d standard t2 in forno. %d special t1, %d special t2, %d standard t1, %d standard t2 in coda\n", f->lotti_special_t1_forno, f->lotti_special_t2_forno, f->lotti_standard_t1_forno, f->lotti_standard_t2_forno,f->lotti_special_t1_in_coda, f->lotti_special_t2_in_coda, f->lotti_standard_t1_in_coda, f->lotti_standard_t2_in_coda);
    //risveglio secondo priorità
    risveglio(f);
    
    pthread_mutex_unlock(&f->lock);
}

void entra_special_t2(Forno * f, Lotto * l)
{
    pthread_mutex_lock(&f->lock);
    
    //condizione di blocco
    // - se il forno è pieno
    // - se c'è in coda uno standard
    // - se in forno ci sono l'altro tipo di piastrelle
    while((totale_lotti_in_forno(f) == MAX_PIASTRELLE_FORNO) || (f->lotti_standard_t1_in_coda > 0) || (f->lotti_standard_t2_in_coda > 0) || (f->lotti_special_t1_forno > 0) || (f->lotti_standard_t1_forno))
    {
        printf("[Lotto special T2] Sospendo\n");
        f->lotti_special_t2_in_coda++;
        pthread_cond_wait(&f->standard_coda, &f->lock);
        printf("[Lotto special T2] Mi risveglio\n");
        f->lotti_standard_t2_in_coda--;
    }
    
    //cose da fare quando mi sblocco
    f->lotti_standard_t2_forno++;
    printf("[Lotto standard T2] Entrato in forno\n");
    
    risveglio(f);

    printf("[STATO] %d special t1, %d special t2, %d standard t1, %d standard t2 in forno. %d special t1, %d special t2, %d standard t1, %d standard t2 in coda\n", f->lotti_special_t1_forno, f->lotti_special_t2_forno, f->lotti_standard_t1_forno, f->lotti_standard_t2_forno,f->lotti_special_t1_in_coda, f->lotti_special_t2_in_coda, f->lotti_standard_t1_in_coda, f->lotti_standard_t2_in_coda);
    //risveglio secondo priorità
    pthread_mutex_unlock(&f->lock);
}

void esce_standard_t2(Forno * f, Lotto * l)
{
    pthread_mutex_lock(&f->lock);
    
    f->lotti_standard_t2_forno--;
    printf("[Lotto standard T2] Uscito dal forno\n");
    
    //risveglio secondo priorità
    risveglio(f);

    printf("[STATO] %d special t1, %d special t2, %d standard t1, %d standard t2 in forno. %d special t1, %d special t2, %d standard t1, %d standard t2 in coda\n", f->lotti_special_t1_forno, f->lotti_special_t2_forno, f->lotti_standard_t1_forno, f->lotti_standard_t2_forno,f->lotti_special_t1_in_coda, f->lotti_special_t2_in_coda, f->lotti_standard_t1_in_coda, f->lotti_standard_t2_in_coda);
    pthread_mutex_unlock(&f->lock);
}

void esce_special_t2(Forno * f, Lotto * l)
{
    pthread_mutex_lock(&f->lock);
    
    f->lotti_special_t2_forno--;
    printf("[Lotto special T2] Uscito dal forno\n");
    
    //risveglio secondo priorità
    risveglio(f);

    printf("[STATO] %d special t1, %d special t2, %d standard t1, %d standard t2 in forno. %d special t1, %d special t2, %d standard t1, %d standard t2 in coda\n", f->lotti_special_t1_forno, f->lotti_special_t2_forno, f->lotti_standard_t1_forno, f->lotti_standard_t2_forno,f->lotti_special_t1_in_coda, f->lotti_special_t2_in_coda, f->lotti_standard_t1_in_coda, f->lotti_standard_t2_in_coda);
    pthread_mutex_unlock(&f->lock);
}

void init_forno(Forno * forno)
{
    pthread_mutex_init(&forno->lock, NULL);
    pthread_cond_init(&forno->standard_coda, NULL);
    pthread_cond_init(&forno->special_coda, NULL);
    forno->lotti_special_t1_in_coda = 0;
    forno->lotti_standard_t1_in_coda = 0;
    forno->lotti_special_t2_in_coda = 0;
    forno->lotti_standard_t2_in_coda = 0;
    forno->lotti_standard_t1_forno = 0;
    forno->lotti_special_t2_forno = 0;
    forno->lotti_special_t1_forno = 0;
    forno->lotti_special_t2_forno = 0;
}

int main(int argc, char *  argv[])
{
    int result;
    void * status;
    
    printf("Programma avviato.\n");
    
    init_forno(&forno);
    
    for(int i = 0; i < STANDARD_N; i++)
    {
        result = pthread_create(&lotti_standard[i], NULL, lotto_standard, (void *)(intptr_t) i);
        if(result)
        {
            printf("Errore nella creazione del thread %d con stato %d\n", i, result);
            exit(-1);
        }
    }
    
    for(int i = 0; i < SPECIAL_N; i++)
    {
        result = pthread_create(&lotti_special[i], NULL, lotto_speciale, (void *)(intptr_t) i);
        if(result)
        {
            printf("Errore nella creazione del thread %d con stato %d\n", i, result);
            exit(-1);
        }
    }
    
    //join dei threads
    for(int i = 0; i < STANDARD_N; i++)
    {
        result = pthread_join(lotti_standard[i], status);
        if(result)
        {
            printf("Errore nel join del thread %d con stato %d e risultato %d\n", i, result, (int)(intptr_t)status);
        }
    }
    
    for(int i = 0; i < SPECIAL_N; i++)
    {
        result = pthread_join(lotti_special[i], status);
        if(result)
        {
            printf("Errore nel join del thread %d con stato %d e risultato %d\n", i, result, (int)(intptr_t)status);
        }
    }
    
    printf("Fine del programma.\n");
}