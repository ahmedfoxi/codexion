# include "ex.h"


pthread_mutex_t mutex;// gloable variable


void    *crim(void  *arg)
{
    int i = 0;
    int *tmp;


    tmp = (int *)arg;
    pthread_mutex_lock(&mutex);
    while (i < 100000)
    {
        i++;
        (*tmp)++;
    }
    pthread_mutex_unlock(&mutex);
    
    printf("%d\n", *tmp);
    return (NULL);
}

int main()
{
    pthread_t t1;
    pthread_t t2;
    int counter = 0;


    printf("==== main ====\n");
    
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&t1, NULL, crim, &counter);
    pthread_create(&t2, NULL, crim, &counter);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex);
    

}
