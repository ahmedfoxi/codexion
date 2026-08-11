# include "ex.h"


pthread_mutex_t mutex_t1;
pthread_mutex_t mutex_t2;

void *task(void *arg)
{
    int *n;
    int i = 0;
    
    n = (int *)arg;
    
    pthread_mutex_lock(&mutex_t1);
    while (i < 10000)
    {
        (*n)++;
        i++;
    }
    pthread_mutex_unlock(&mutex_t1);
    
    printf("t1: %d\n", *n);
    return (NULL);
}

void *task1(void *arg)
{
    int *n;
    int i = 0;
    
    n = (int *)arg;
    
    pthread_mutex_lock(&mutex_t2);
    while (i < 1000000)
    {
        (*n)++;
        i++;
    }
    pthread_mutex_unlock(&mutex_t2);
    
    printf("t2: %d\n", *n);
    return (NULL);
}

int main()
{
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_t t4;
    int counter_1 = 0;
    int counter_2 = 100;
    
    // mutex
    pthread_mutex_init(&mutex_t1, NULL);
    pthread_mutex_init(&mutex_t2, NULL);
    
    // thread
    pthread_create(&t1, NULL, task, &counter_1);
    pthread_create(&t2, NULL, task, &counter_1);
    pthread_create(&t3, NULL, task1, &counter_2);
    pthread_create(&t4, NULL, task1, &counter_2);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    pthread_mutex_destroy(&mutex_t1);
    pthread_mutex_destroy(&mutex_t2);

    printf("Counter: %d\n", counter_1);
    printf("Counter: %d", counter_2);
}
