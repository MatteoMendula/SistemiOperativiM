#include <stdio.h>
#include <pthread.h>

#define BUFFER_SIZE 16
#define MAX 20
#define OVER (-1)

typedef struct
{
    int buffer[BUFFER_SIZE];
    pthread_mutex_t lock;
    int readpos, writepos;
    int cont;
    pthread_cond_t notempty;
    pthread_cond_t notfull;
} prodcons;

void init(prodcons *b)
{
    pthread_mutex_init(&b->lock, NULL);
    pthread_cond_init(&b->notempty, NULL);
    pthread_cond_init(&b->notfull, NULL);
    
    b->cont = 0;
    b->readpos = 0;
    b->writepos = 0;
}

void inserisci(prodcons * b, int data)
{
    pthread_mutex_lock(&b->lock);
    
    while(b->cont == BUFFER_SIZE)
        pthread_cond_wait(&b->notfull, &b->lock);
    
    b->buffer[b->writepos] = data;
    b->cont++;
    b->writepos++;
    
    if(b->writepos >= BUFFER_SIZE)
        b->writepos = 0;
    
    pthread_cond_signal(&b->notempty);
    pthread_mutex_unlock(&b->lock);
}

int estrai(prodcons *b)
{
    int data;
    
    pthread_mutex_lock(&b->lock);
    while(b->cont == 0)
        pthread_cond_wait(&b->notempty, &b->lock);
    
    data = b->buffer[b->readpos];
    b->cont--;
    b->readpos++;
    
    if(b->readpos >= BUFFER_SIZE)
        b->readpos = 0;
    
    pthread_cond_signal(&b->notfull);
    pthread_mutex_unlock(&b->lock);
    
    return data;
}

prodcons buffer;

void *producer(void *data)
{
    int n;
    printf("Sono il thread produttore\n\n");
    
    for(n=0; n < MAX; n++)
    {
        printf("Thread produttore ----> %d\n",n);
        inserisci(&buffer, n);
    }
    
    inserisci(&buffer, OVER);
    return NULL;
}

void *consumer(void *data)
{
    int d;
    printf("Sono il thread consumatore\n\n");
    
    while(1)
    {
        d = estrai(&buffer);
        if(d == OVER)
            break;
        
        printf("Thread consumatore ----> %d\n", d);
    }
    return NULL;
}

int main()
{
    pthread_t th_a, th_b;
    void *retval;
    
    init(&buffer);
    
    pthread_create(&th_a, NULL, producer, 0);
    pthread_create(&th_b, NULL, consumer, 0);
    
    pthread_join(th_a, &retval);
    pthread_join(th_b, &retval);
    return 0;
}