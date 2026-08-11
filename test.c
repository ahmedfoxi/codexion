#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


void    *worker(void *arg)
{
    int local = 10;

    printf("Thread %ld: local = %d, adress = %p\n",
            (long)arg, local, (void *)&local);

    return NULL;
}

int main()
{
    int v1 = 0;
    int v2 = 0;
    pthread_t t1, t2;

    v1 = pthread_create(&t1, NULL, worker, (void *)1);
    v2 = pthread_create(&t2, NULL, worker, (void *)2);

    printf("v1: %d\nv2: %d\n", v1, v2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("\n\nt1: %ld", t1);
    printf("\nt2: %ld", t2);
    // pthread_mutex_lock(&mutex);

    // counter++;

    // pthread_mutex_unlock(&mutex);
}
