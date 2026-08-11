#include "ex.h"


void    *worker(void *args)
{
    printf("work\n");
    return NULL;
}

int main()
{
    pthread_t   threads[5];
    int i;


    i = 0;
    while(i < 5)
    {
        printf("main\n");
        pthread_create(&threads[i], NULL, worker, NULL);
        pthread_join(threads[i],NULL);
        i++;
    }
}
