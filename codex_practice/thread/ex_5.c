# include "ex.h"


void    *crim(void  *arg)
{
    int i;
    int *tmp;

    tmp = (int *)arg;
    while (i < 100000)
    {
        i++;
        (*tmp)++;
    }
    
    printf("%d\n", *tmp);
    return (NULL);
}

int main()
{
    pthread_t t1;
    pthread_t t2;
    int counter = 0;


    printf("==== main ====\n");
    pthread_create(&t1, NULL, crim, &counter);
    printf("Shared counter: %d\n", counter);
    // usleep(200);
    pthread_create(&t2, NULL, crim, &counter);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Shared counter: %d\n", counter);

}
